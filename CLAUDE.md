# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A C++20 SCADA gateway (Linux) that polls PLCs over Modbus TCP (via libmodbus), republishes register values on an OPC UA server/client (via open62541), exposes them as JSON over a small REST API (cpp-httplib), and serves a browser dashboard (`www/`). A `scada/` layer implements process-control logic on top (temperature/power regs for what looks like a distillation/rectification rig — see register names like `Tkub`, `Tbuf`, `Tdef`, `P.dist/P.heat/P.rect`).

This project connects to **real physical Modbus PLCs** in some deployments — treat register writes (`rmode = "rw"`) with the same care as talking to live hardware, not a simulator.

## Build

```sh
make            # build a.out (default, no MB_MASTER/MB_SLAVE define)
make master     # build mb_master, defines -DMB_MASTER
make slave      # build mb_slave, defines -DMB_SLAVE
make debug      # -g -DDEBUG_FLAG, no optimization
make check      # ASan + UBSan + _GLIBCXX_DEBUG, no optimization
make fulldebug  # debug + check combined
make clean      # removes objects/binary, also runs `format`
```

- Compiler/std: g++, C++20, `-Wall -Wextra -Wpedantic -Wfatal-errors` plus extra warning groups (`-Wshadow -Wconversion -Wcast-align -Wduplicated-cond` …).
- Source dirs are auto-discovered: `include/`, `sources/`, `libs/**`, `scada/`. Adding a new `.cpp` file under one of these is picked up automatically by the Makefile — no need to list it anywhere.
- Object/dep files go to `tmp/obj/<path>` (`.o` + auto-generated `.d`).
- External deps are all expected to be installed system-wide (no vendoring/submodules): `libconfig++`, `libmodbus` (via pkg-config), `open62541`, `mbedtls`/`mbedx509`/`mbedcrypto`, `httplib` (`/usr/local/include/httplib.h`). `nlohmann/json` is installed but intentionally unused — JSON is hand-built via string concatenation (see `sources/reg_func.cpp`, `quote()`), per commit "Switch to simple string JSON".
- Formatting: `make format` (astyle, Linux style) is invoked automatically by `make clean`. Also available: `make kr`, `make clang`, `make google`.
- `make shared` builds `shared/libTomatLogger.so`, an older standalone build of the logger — separate from `libs/logger` used by the main binary.

### Running

```sh
./a.out [master|slave|scada] [loglevel]   # loglevel is a single digit 0-9
```

Reads config from `CFG_DIR/CFG_FILE` (`config.h`: `/var/tmp/conf/modbus.cfg`). On this machine `/var/tmp/conf` is a symlink to this repo's `conf/` directory — **that symlink must exist on any new machine** (`ln -s /path/to/repo/conf /var/tmp/conf`) before the binary can find its config.

Ctrl-C triggers a clean shutdown via `deinit_all()`. While running, the console UI takes single-key input to switch screens, change refresh speed, change log level, or hot-reload config (see `parse_char` in `sources/all_func.cpp`).

### Tests

No unit-test framework (no gtest/catch2) and no CI. `test/numbers/` is a standalone sandbox with its own `Makefile` for exercising the numeric type system (`Number_c`/`numeric_u`) manually.

## Architecture

**Three-layer split**, all wired together through global objects declared in `main.cpp` (`Cfg`, `REGmap`, `PLCvec`, `OPCclient`, `OPCs`, `Task`):

- `libs/` — standalone, reusable building blocks, each isolated in its own directory: `mb_plc` (Modbus master/slave wrapper), `reg` (`Reg_c`, see below), `number` (typed numeric union `Number_c`/`numeric_u`), `opc_server`/`opc_client` (open62541 wrappers), `scheduler` (`Task_c`/`Schedule_c` periodic-task runner), `console` (terminal control), `logger` (thread-safe queued logger with ANSI colors + syslog).
- `sources/` — glue code: config parsing (`cfg_master.cpp`, `cfg_scada.cpp`, libconfig++), the REST server (`net_func.cpp`), register sync loop (`reg_func.cpp`), periodic task registration (`tasks.cpp`), and process lifecycle (`all_func.cpp`: `init_all`/`deinit_all`/`parse_char`).
- `scada/` — the actual control logic (`p_dist.cpp`, `p_heat.cpp`, `p_rect.cpp`, `scada_power.cpp`, `scada_temps.cpp`), reading/writing SCADA registers by name via `ReadValue()`/similar.

**`Reg_c` (`libs/reg/reg_class.h`) is the central abstraction**: one register can simultaneously be backed by a Modbus PLC value, exposed as an OPC UA node, and readable/writable through the REST JSON API — `Reg_c` keeps these in sync and handles type conversion/formatting. All live registers are held by name in the global `map<string, Reg_c> REGmap`.

**Config is declarative and cascading** (libconfig, `@include`-based), rooted at `conf/modbus.cfg`:
- `plc_list_PLC` in `modbus.cfg` selects which PLC entries (defined in `mbplc.cfg`) are actually active; unused ones are commented out rather than deleted.
- `mbplc.cfg` defines each PLC (`ip`, `port`, `polling`, `timeout`) and `@include`s a `regs_*.cfg` file describing its Modbus registers.
- `scada.cfg` defines the SCADA-layer folder tree and `@include`s `scada_vars.cfg` (register defs, some with `rsource = "DEVICE.regname"` mapping to a PLC register, some computed/standalone with `rsource = "-"`) and `scada_conf.cfg` (runtime-tunable power thresholds: `P_MAX`, `P_WARM`, `P_HEAD`, `P_BODY`, `P_TAIL`, `P_DIST[/2/3]`, `P_BOIL`, consumed via `ReadValue("P_MAX")` etc. in `scada/`).
- Every SCADA register is initialized to a literal placeholder value `888` in `cfg_scada.cpp` (marked `// TODO: remove for production!` by the author) — seeing `888` across the board means "not yet configured/written," not a bug.
- Failed/uninitialized Modbus reads surface as raw all-1-bits patterns (`65535`, `8388608`, `4294967295`) rather than an explicit error code — the `www/` dashboard treats these as its `ERR_VALUES` set and `888` as `WARN_VALUES`.

**Periodic work is scheduler-driven**, not ad-hoc timers: `sources/tasks.cpp` registers callbacks (Modbus polling, OPC refresh, register-map refresh, heartbeat) into the global `Task` (`Schedule_c`), which the main loop in `main.cpp` drives each iteration alongside the console redraw.

**REST + dashboard**: `sources/net_func.cpp` mounts `./www` as static root (`svr.set_mount_point("/", "./www")`, cpp-httplib) and serves the live register snapshot as JSON at `/api/opc`. `www/index.html` and `www/device.html` are the dashboard frontend (grouped tree view + Chart.js line charts per register/group). Frontend JS dependencies must be **vendored locally in `www/`**, never linked from a CDN — the deployment target may have no internet access.

## Known open design questions (from `TODO`)

The master/slave/scada "roles" currently live in one config/codebase but aren't fully unified (see `TODO` file): open items include a single algorithm for register-write priority when multiple sources can write the same reg, where global/config variables like the `P_*` power thresholds actually get their real values from (currently just placeholder `888`), and reboot-safe storage for global state.

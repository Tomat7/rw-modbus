//
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <chrono>
#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <syslog.h>

#include "plc_class.h"

#define CAST_MILLIS duration_cast<milliseconds>


using namespace std;

// PLC::PLC() {}

int PLC::init(const char *_ip, int _port)
{
    rc = 0;

    if (_port == 0) {
        _port = tcp_port;
        _ip = ip_addr;
    }

    printf("+++ MB init: try to check for NULL: %s \n", _ip);

    if (ctx != NULL) {
        printf("+++ MB init: try to close: %s \n", _ip);
        modbus_close(ctx);
        printf("+++ MB init: try to free: %s \n", _ip);
        modbus_free(ctx);
        ctx = NULL;
    }

    printf("+++ MB init: try to NEW: %s \n", _ip);

    ctx = modbus_new_tcp(_ip, _port);
    if (ctx == NULL) {
        fprintf(stderr, "MB: error allocate ctx for %s:%d\n", _ip, _port);
        rc = -1;
    } else {
        rc = modbus_set_response_timeout(ctx, 0, mb_timeout);
        if (rc == -1)
            fprintf(stderr, "MB: set timeout failed: %s\n", modbus_strerror(errno));
    }

    return rc;
}

int PLC::connect()
{
    if (ctx == NULL) {
        init();
        if (rc == -1)
            return rc;
        printf("MB: new CTX - ok.\n");
    }

    rc = modbus_connect(ctx);
    if (rc == -1) {
        fprintf(stderr, "MB: connect err %s:%d: %s\n", ip_addr, tcp_port,
                modbus_strerror(errno));
        modbus_free(ctx);
        ctx = NULL;
    }

    return rc;
}

int PLC::read()
{
    rc = 0;

    connect();
    if (rc == -1) {
        mb_errors++;
        return rc;
    }

    uint16_t *mbregs = new uint16_t[nb_regs + 1];

    rc = modbus_read_registers(ctx, 0, nb_regs, mbregs);
    if (rc == -1) {
        fprintf(stderr, "MB: read error: %s \n", modbus_strerror(errno));
        mb_errors++;
        return rc;
    }

    for (int j = 0; j < nb_regs; ++j)
        regs[j].rvalue = mbregs[regs[j].raddr];

    mb_time = millis();

    modbus_close(ctx);
    delete[] mbregs;

    return 0;
}

int PLC::set_timeout()
{
    if (ctx == NULL)
        init();

    rc = modbus_set_response_timeout(ctx, 0, mb_timeout);
    if (rc == -1)
        fprintf(stderr, "MB: set timeout failed: %s\n", modbus_strerror(errno));

    return rc;
}

void PLC::deinit()
{
    if (ctx != NULL) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
}

uint64_t PLC::millis()
{
#define CAST_MILLIS duration_cast<milliseconds>

    using namespace std::chrono;
    uint64_t t, old = mb_time;
    t = CAST_MILLIS(system_clock::now().time_since_epoch()).count();
    printf("___dT: %ld  errors: %d\n", t - old, mb_errors);

    return t;
}

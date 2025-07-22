// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <cmath>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "./config.h"
#include "./libs.h"

cfg_t Cfg;
map<string, Reg_c> REGmap;
vector<PLC_c> PLCvec;
OPC::OpcClient_c OPCclient;
OpcServer_c OPCs;
Schedule_c Task /* (TASKS_NB_MAX) */;
// PLC_c Slave(MB_SLAVE_PORT);
// string PLC_folder = PLC_FOLDER;
// string SCADA_folder = OPC_SCADA_FOLDER;
// INotify IN(CFG_DIR);
// int timeout_sec = TIMEOUT_SEC;

const char* mode = "master";
int rc;

static void close_sigint(int dummy)
{
  LOGC("Exit by Ctrl-C. Bye.\n");
  deinit_all();
  restore_console();
  closelog();
  exit(dummy);
}

//  std::cout << "dbl_fmt: " << std::format("{}", 2.235435435) << "\n";
//  The "std::format" and "std::print" is real BULLSHIT! Never use it!

int main(int argc, char** argv)
{
  Timer t;

  std::set<string> Mode{MODBUS_MODES};
  log_level = LOG_LEVEL_DEFAULT;  // Need "work" logging // log_level = 3;

  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  signal(SIGINT, close_sigint);

  if (argc > 1) {
    if (Mode.count(string(argv[1])))
      mode = argv[1];
    else
      LOGA("Argument '%s' ignored.", argv[1]);
  }

  if (argc > 2) {
    char ch = *argv[2];
    if ((strlen(argv[2]) == 1) && (isdigit((char)ch))) {
      log_level = int((char)ch - '0');
      LOGA("LOG_LEVEL set to: %d", log_level);
    } else
      LOGA("Argument '%s' ignored.", argv[2]);
  }


  variant_t v;
  v = 3.1415926f;
  auto vv = v;
  printf("%f\n", std::get<float>(v));
  printf("%f\n", std::get<float>(vv));

  uint64_t U1 = UINT64_MAX - 15; //1234567890;
  float F1 = 3.1415926f;
  double D1 = PI; //3.1415926;

  printf("\nReady - 00 new myclass = typed_var\n");
  Value_c Ux = U1;
  Value_c Fx = F1;
  Value_c Dx = D1;

  printf("\nReady - 00 existing myclass = typed_var\n");
  Ux = U1;
  Fx = F1;
  Dx = D1;

  printf("\nReady - 00 typed_var = myclass\n");
  uint16_t U2 = Ux;
  float F2 = Fx;
  float D2 = Dx;

  printf("\nReady - 00 new myclass = myclass\n");
  Value_c Uz = Ux;
  Value_c Fz = Fx;
  Value_c Dz = Dx;

  printf("\nReady - 00 myclass = myclass\n");
  Uz = Ux;
  Fz = Fx;
  Dz = Dx;

  printf("\nReady - 00 existing myclass = MIXED typed_var\n");
  Uz = D1;
  Fz = Fx;
  Dz = U1;

  printf("\nReady - 0 CPP typed var2\n");
  printf("%i\n", U2);
  printf("%f\n", F2);
  printf("%f\n", D2);
  /*
    printf("\nReady - 1 myclass\n");
    printf("%i\n", Ux);
    printf("%f\n", Fx);
    printf("%i\n", Uz);
    printf("%f\n", Fz);
  */
  printf("\nReady - 1 myclass.type\n");
  printf("%i\n", Ux.ui16);
  printf("%f\n", Fx.fl);
  printf("%i\n", Uz.ui16);
  printf("%f\n", Fz.fl);
  printf("%lf\n", Dz.dbl);

  printf("\nReady - 2 (type)myclass\n");
  printf("u-u %i\n", (uint16_t)Ux);
  printf("f-f %f\n", (float)Fx);
  printf("d-d %lf\n", (double)Dz);
  printf("u-u %lf\n", (double)Uz);

  printf("u-f %f\n", (float)Uz);
  printf("f-i %i\n", (int16_t)Fz);
  printf("d-i %i\n", (int32_t)Dz);

  printf("\nReady - 3 myclass.c_str()\n");
  printf("%s\n", Ux.c_str());
  printf("%s\n", Fx.c_str());
  printf("%s\n", Dx.c_str());

  printf("\nReady - 4 (char*)myclass\n");
  printf("%s\n", (char*)Uz);
  printf("%s\n", (char*)Fz);
  printf("%s\n", (char*)Dz);


  printf("\nCompare -  myclass == UINT, print myclass\n");
  if (Dz == U1)
    printf("Dz==U1 %lf == %lu\n", Dz, U1);
  else
    printf("Dz!=U1 %lf != %lu\n", Dz, U1);

  printf("\nCompare -  myclass > UINT, print (char*)myclass\n");
  if (Dz > U1)
    printf("Dz>U1 %s > %lu\n", (char*)Dz, U1);
  else
    printf("Dz<=U1 %s <= %lu\n", (char*)Dz, U1);
// =======================================================

  printf("\nCompare -  myclass == myclass, print (char*)myclass\n");
  if (Fx == F1)
    printf("Fx==F1 %s == %f\n", (char*)Fx, F1);
  else
    printf("Fx!=F1 %s != %f\n", (char*)Fx, F1);

  if (Fx > F1)
    printf("Fx>F1 %s > %f\n", (char*)Fx, F1);
  else
    printf("Fx<=F1 %s <= %f\n", (char*)Fx, F1);

// =======================================================

  if (Dx == D1)
    printf("Dx==D1 %s == %lf\n", (char*)Dx, D1);
  else
    printf("Dx!=D1 %s != %lf\n", (char*)Dx, D1);

  if (Dx > D1)
    printf("Dx>D1 %s > %lf\n", (char*)Dx, D1);
  else
    printf("Dx<=D1 %s <= %lf\n", (char*)Dx, D1);

  if (Dx == D1)
    printf("Dx==D1 %.15lf == %.15lf\n", (double)Dx, D1);
  else
    printf("Dx!=D1 %s != %lf\n", (char*)Dx, D1);


  printf("=======================================================\n");
  uint64_t U3 = INT64_MAX - 4775807;
  uint32_t I3 = UINT32_MAX;
  double D3 = PI;
  float F3 = 3.1415926f;

  printf("=======================================================\n");
  // D3 = F3;
  Dx = D3;
  Ux = F3;


  printf("\nCompare -  CPP_var == CPP_var, print CPP_var\n");
  if (F3 == D3)
    printf("F3==D3 %f == %lf\n", F3, D3);
  else if (F3 > D3)
    printf("F3>D3 %f > %lf\n", F3, D3);
  else
    printf("F3<D3 %f < %lf\n", F3, D3);


  printf("\nCompare -  myclass == mayclass, print (char*)myclass\n");
  if (Dx == Ux)
    printf("Dx==Ux %s == %s\n", (char*)Dx, (char*)Ux);
  else if (Dx < Ux)
    printf("Dx<Ux %s < %s\n", (char*)Dx, (char*)Ux);
  else
    printf("Dx>Ux %s > %s\n", (char*)Dx, (char*)Ux);

  printf("================================================================\n");

  U3 = I3;
  Dx = U3;
  Ux = Dx;

  printf("\nCompare -  CPP_var == CPP_var, print CPP_var\n");
  if (I3 == U3)
    printf("I3==U3 %u == %lu\n", I3, U3);
  else
    printf("I3!=U3 %u != %lu\n", I3, U3);

  printf("\nCompare -  myclass == mayclass, print (char*)myclass\n");
  if (Dx == Ux)
    printf("Dx==Ux %s == %s\n", (char*)Dx, (char*)Ux);
  else
    printf("Dx!=Ux %s != %s\n", (char*)Dx, (char*)Ux);


  printf("===========================\n");


  D3 = U3;
  Dx = D3;
  Ux = Dx;

  printf("\nCompare -  CPP_var == CPP_var, print CPP_var\n");
  if (D3 == U3)
    printf("D3==U3 %.15lf == %lu\n", D3, U3);
  else
    printf("D3!=U3 %.15lf != %lu\n", D3, U3);

  printf("===========================\n");


  printf("\nCompare -  myclass == mayclass, print (type)myclass\n");

  if (Dx == Ux)
    printf("Dx==Ux %.15lf == %lu\n", (double)Dx, (uint64_t)Ux);
  else
    printf("Dx!=Ux %.15lf != %lu\n", (double)Dx, (uint64_t)Ux);

  printf("===========================\n");

  printf("\nCompare -  myclass == mayclass, print myclass.type\n");
  if (Dx == Ux)
    printf("Dx==Ux %.15lf == %lu\n", Dx.dbl, Ux.ui32);
  else
    printf("Dx!=Ux %.15lf != %lu\n", Dx.dbl, Ux.ui32);

  printf("\nCompare -  myclass == mayclass, print (char*)myclass\n");
  if (Dx == Ux)
    printf("Dx==Ux %s == %s\n", (char*)Dx, (char*)Ux);
  else
    printf("Dx!=Ux %s != %s\n", (char*)Dx, (char*)Ux);

  printf("===========================\n");

  printf("\nCompare -  myclass == CPP_var, print myclass.type\n");
  if (Dx == Ux)
    printf("Dx==Ux %.15lf == %lu\n", Dx.dbl, U3);
  else
    printf("Dx!=Ux %.15lf != %lu\n", Dx.dbl, U3);



// =======================================================

  wait_console(Cfg.timeout_sec * 3);

  init_all();

  OPCclient.init("opc.tcp://localhost:4840");
//  logger_set_queue(true);

  for (;;) {
    logger_set_queue(true);
    printf("%s", ESC_CLS);
    printf("%s", ESC_HOME);
    fflush(stdout);

    opc_client_();
    opc_server_();

    printf("\n");

    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");

    const char* x = nullptr;
    t.start(x);
    mb_print_summary();
    // mb_update();
    // LOGD("regdata_t size: %d", sizeof(regdata_t));
    // LOGD("P array size: %d", sizeof(P));
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    logger_set_queue(false);
    logger_flush();

    // Slave.handle_slave(timeout_sec * 1000000);

    int ch = read_console(0, Cfg.timeout_sec * 1000000);
    if (ch != -1)
      parse_char(ch);
    else
      printf("!\n");

    fflush(stdout);

    //    wait_console(TIMEOUT_SEC);
    //    t.start(x);
    //    t.sleep_ms(TMOUT);
    //    t.spent();
  }

  return (EXIT_SUCCESS);
}

// eof

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

const char* mode = "master";
int rc;

static void close_sigint(int dummy)
{
  LOGC("Exit by Ctrl-C. Bye.\n");
//  deinit_all();
//  restore_console();
//  closelog();
  exit(dummy);
}

//  std::cout << "dbl_fmt: " << std::format("{}", 2.235435435) << "\n";
//  The "std::format" and "std::print" is real BULLSHIT! Never use it!



int main(int argc, char** argv)
{
  log_level = LOG_LEVEL_DEFAULT;  // Need "work" logging // log_level = 3;

  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  signal(SIGINT, close_sigint);
  /*
    float ff = PI;
    void* _p = nullptr;
    _p = &ff;
    variant_t va;
    va = ff;
    char buff[50];
    snprintf(buff, STR_SIZE, "PI=%f, *_p=%f, v=%f", ff, *static_cast<float*>(_p), va);
    LOGx("buff= %s", buff);
    exit(EXIT_SUCCESS);
  */
  // ==============================================================
  // ==============================================================
  // ==============================================================

  variant_t v;
  v = 3.1415926f;
  auto vv = v;
  printf("%f\n", std::get<float>(v));
  printf("%f\n", std::get<float>(vv));

  uint64_t U1 = UINT64_MAX; // 18446744073709551615; //1234567890;
  int64_t I1 = INT64_MAX; // 18446744073709551615; //1234567890;
  uint32_t U32 = UINT32_MAX; // 18446744073709551615; //1234567890;
  int32_t I32 = INT32_MAX; // 18446744073709551615; //1234567890;


  long long LLX = 0;
  unsigned long long uLLX = 0;
  long LX = 0;
  unsigned long uLX = 0;
  printf("LLX size = %i, uLLX size = %i\n", sizeof(LLX), sizeof(uLLX));
  printf("LX size = %i, uLX size = %i\n", sizeof(LX), sizeof(uLX));

  printf("U64 size = %i, I64 size = %i\n", sizeof(U1), sizeof(I1));
  printf("U32 size = %i, I32 size = %i\n", sizeof(U32), sizeof(U32));

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

  printf("\nReady - 1 myclass\n");
  printf("%i\n", Ux);
  printf("%f\n", Fx);
  printf("%i\n", Uz);
  printf("%f\n", Fz);

  printf("\nReady - 1 myclass.type\n");
  printf("%i\n", Ux.ui16);
  printf("%f\n", Fx.fl);
  printf("%u\n", Uz.ui16);
  printf("%f\n", Fz.fl);
  printf("%lf\n", Dz.dbl);

  printf("\nReady - 2 (type)myclass\n");
  printf("u-u %i\n", (uint16_t)Ux);
  printf("f-f %f\n", (float)Fx);
  printf("d-d %f\n", (double)Dz);
  printf("u-u %i\n", (uint16_t)Uz);

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


  if (Dz == U1)
    printf("Dz==U1 %s == %lu\n", (char*)Dz, U1);
  else
    printf("Dz!=U1 %s != %lu\n", (char*)Dz, U1);

  if (Dz > U1)
    printf("Dz>U1 %s > %lu\n", (char*)Dz, U1);
  else
    printf("Dz<=U1 %s <= %lu\n", (char*)Dz, U1);
// =======================================================

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
    printf("Dx==D1 %19.9lf == %19.9lf\n", (double)Dx, D1);
  else
    printf("Dx!=D1 %s != %lf\n", (char*)Dx, D1);

// =======================================================


  return (EXIT_SUCCESS);
}

// eof

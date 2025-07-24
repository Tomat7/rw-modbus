// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
// https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c
#include <cmath>
#include <map>
#include <set>
#include <string>
//#include <variant>
#include <vector>
#include <cassert>

#include "./config.h"
#include "./libs.h"

/*   variant_t v = 3.1415926f;
  auto vv = v; */
uint64_t U1 = UINT64_MAX - 15; //1234567890;
int32_t I1 = INT32_MIN;
float F1 = 3.1415926f;
double D1 = PI;

void var_test0()
{
  printf("\n======= var_test0 =======\n");

  int i = 10;
  /*   assert(i > 5);

    assert(i > 15); */

  value_u v;
  void* p = &v;
  v.i32 = INT32_MIN;
  float f = *(float*)p;
  F1 = INT32_MIN;
  Value_c x = F1;

  printf("F1=%f, x=%f, (char*)x=%s)\n", F1, (double)x, (char*)x);
  printf("(int32_t)F1=%i, (int32_t)x=%i, (char*)x=%s\n", (int32_t)F1, (int32_t)x, (char*)x);

  printf("f = %f", f);

  /*
    LOGA("\n Variant_t === ");
    printf("%f\n", std::get<float>(v));
    printf("%f\n", std::get<float>(vv));
  */
  printf("\nReady - 00 myclassES = typed_varS \n");
  Value_c Ux = U1;
  Value_c Ix = I1;
  Value_c Fx = F1;
  Value_c Dx = D1;
  Value_c Sx = "qwertyui";

  printf("\nReady - 00 existing myclassES = typed_varS \n");
  Ux = U1;
  Ix = I1;
  Fx = F1;
  Dx = D1;
  Sx = "qwertyui";


  printf("\n======= Ready - 01 IF Fx vs U64 typed_var =======\n");
  U1 = UINT64_MAX - 15;
  //Fx = F1;

  F1 = INT32_MIN;


#define X1 I1
#define XX "U1"
#define Vx Fx
#define VV "Fx"
  Vx = F1;

  if (Vx == X1)
    printf("%s==%s %s == %i\n", VV, XX, (char*)Vx, X1);
  else
    printf("%s!=%s %s != %i\n", VV, XX, (char*)Vx, X1);

  if (Vx != X1)
    printf("%s!=%s %s != %i\n", VV, XX, (char*)Vx, X1);
  else
    printf("%s==%s %s == %i\n", VV, XX, (char*)Vx, X1);

  if (Vx > X1)
    printf("%s>%s %s > %i\n", VV, XX, (char*)Vx, X1);
  else
    printf("%s<=%s %s <= %i\n", VV, XX, (char*)Vx, X1);

  if (Vx < X1)
    printf("%s<%s %s < %i\n", VV, XX, (char*)Vx, X1);
  else
    printf("%s>=%s %s >= %i\n", VV, XX, (char*)Vx, X1);

}


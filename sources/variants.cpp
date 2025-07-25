// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
// https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c
#include <cmath>
#include <map>
#include <set>
#include <string>
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

float fx = 3.1415f;

Number_c get_val()
{
  return fx;
}
/*
  variant_t get_var()
  {
  return fx;
  }

*/
void var_test0()
{
  printf("\n======= var_test0 =======\n");
  //printf("Value_c:Float = %i\n", (int)get_val());
  //printf("Variant:Float = %f\n", get<float>(get_var()));
  long double ld = 0.0;
  double d = 0.0;

  printf("\n\nDBL size= %lu, LongDBL size= %lu\n\n\n", sizeof(d), sizeof(ld));

  uint16_t u16 = 12345;
  int16_t i16 = INT16_MIN;
  void* _p = &u16;

  printf("u16 as u64 = %u\n", *(uint16_t*)_p);
  __float128 F128 = U1;
  F128 +=2;
  uint64_t Uf128 = F128;
  Uf128++;
  printf("\nf128 size is: %lu, U1=%lu, F128=%lu \n\n\n", sizeof(F128), U1, Uf128);

  numeric_u v;
  void* p = &v;
  v.i32 = INT32_MIN;
  float f = *(float*)p;
  F1 = INT32_MIN;
  Number_c x = F1;

  printf("F1=%f, x=%f, (char*)x=%s)\n", F1, (double)x, (char*)x);
  printf("(int32_t)F1=%i, (int32_t)x=%i, (char*)x=%s\n", (int32_t)F1, (int32_t)x, (char*)x);

  printf("f = %f", f);

  /*
    LOGA("\n Variant_t === ");
    printf("%f\n", std::get<float>(v));
    printf("%f\n", std::get<float>(vv));
  */
  printf("\nReady - 00 myclassES = typed_varS \n");
  Number_c Ux = U1;
  Number_c Ix = I1;
  Number_c Fx = F1;
  Number_c Dx = D1;
  Number_c Sx = "qwertyui";

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


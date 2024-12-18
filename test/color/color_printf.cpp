#include <stdio.h>
#include <unistd.h>
#include "macros.h"

/* #define BLU_RED "\033[1;94;41m"
#define BLU_GRE "\033[5;94;42m"
#define OLDCOLOR "\033[0;0;0m"

#define RED "\033[0;91m"
#define BLU "\033[0;94m"
#define GRE "\033[0;92m"
#define YEL "\033[0;93m"
#define PUR "\033[0;95m"

#define NEWCOLOR0 "\033[0;38;05;49;48;05;129m"
#define NEWCOLOR1 "\033[1;38;05;49;48;05;129m" */

// "\033[$A;38;05;$B;48;05;$C""m Фон $C\t\c"

int main()
{


/*   printf("normal NRM-NRM " NRM  " no BOLD " C_GRY " BOLD " NRM "\n");
  printf("standard STD   " STD_ " no BOLD " STD_B " BOLD " NRM "\n");
  printf("light-grey GRL " GRL_ " STAND   " GRL_B "bold STAND " NRM "\n");
  printf("dark-grey GRD  " GRD_ " no BOLD " GRD_B " BOLD " NRM "\n") */;

   printf("WHI GRY BLC  " C_WHI " WHI " C_GRY " GRY " C_BLC " BLC " NRM "\n");
   printf("WHIbGRYbBLCb " C_WHIB" WHIB " C_GRYB" GRYB " C_BLCB" BLCB" NRM "\n");
   
  printf("C_DARK " C_DARK " DARK Norm BOLD High " NRM "\n");
  printf("C_NORM " C_NORM " DARK Norm BOLD High " NRM "\n");
  printf("C_BOLD " C_BOLD " DARK Norm BOLD High " NRM "\n");
  printf("C_HIGH " C_HIGH " DARK Norm BOLD High " NRM "\n");
  printf("C_HIGH2 " C_HIGH2 " DARK Norm BOLD High " NRM "\n");

  printf("C_RED  " C_RED  " DARK Norm BOLD High " NRM "\n");
  printf("C_REDB " C_REDB " DARK Norm BOLD High " NRM "\n");
  printf("C_REDD " C_REDD " DARK Norm BOLD High " NRM "\n");
  printf("C_REDH " C_REDH " DARK Norm BOLD High " NRM "\n");
  
  /*
    printf("red-light " RED_L " no BOLD " RED_LB " BOLD " OLDCOLOR "\n");
    printf("red-dark  " RED_D " no BOLD " RED_DB " BOLD " OLDCOLOR "\n");

    printf("new color " NEWCOLOR0 " no BOLD " NEWCOLOR1 " BOLD " OLDCOLOR "\n");

    printf("%sred\n", KRED);
    printf("%sgreen\n", KGRN);
    printf("%syellow\n", KYEL);
    printf("%sblue\n", KBLU);
    printf("%smagenta\n", KMAG);
    printf("%scyan\n", KCYN);
    printf("%swhite\n", KWHT);
    printf("%snormal\n", KNRM);
  */
  /*
      while
      for
  */
  return 0;
}

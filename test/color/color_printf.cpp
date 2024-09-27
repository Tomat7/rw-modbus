#include <stdio.h>
#include <unistd.h>

#define BLU_RED "\033[1;94;41m"
#define BLU_GRE "\033[5;94;42m"
#define OLDCOLOR "\033[0;0;0m"

#define RED "\033[0;91m"
#define BLU "\033[0;94m"
#define GRE "\033[0;92m"
#define YEL "\033[0;93m"
#define PUR "\033[0;95m"

#define WHI_ "\033[0;97m"
#define WHI_B "\033[1;97m"

#define STD_ "\033[0;39m"
#define STD_B "\033[1;39m"

#define GRL_ "\033[0;37m"
#define GRL_B "\033[1;37m"

#define GRD_ "\033[0;90m"
#define GRD_B "\033[1;90m"

#define COLOR1 "\033[1;162;159m"
#define COLOR2 "\033[0;0;0m"

#define RED_L "\033[0;91m"
#define RED_LB "\033[1;91m"
#define RED_D "\033[0;31m"
#define RED_DB "\033[1;31m"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define NEWCOLOR0 "\033[0;38;05;49;48;05;129m"
#define NEWCOLOR1 "\033[1;38;05;49;48;05;129m"

// "\033[$A;38;05;$B;48;05;$C""m Фон $C\t\c"

int main() {
  if (isatty(STDOUT_FILENO))
    printf("white     " WHI_ " no BOLD " WHI_B " BOLD  " OLDCOLOR "\n");
  else
    printf("This is pipe, no colors!\n");

  printf("standart  " STD_ " no BOLD " STD_B " BOLD " OLDCOLOR "\n");
  //    printf("light-grey" GRL_ " STAND " GRL_B "bold STAND " OLDCOLOR "\n");
  printf("dark-grey " GRD_ " no BOLD " GRD_B " BOLD " OLDCOLOR "\n");

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

  /*
      while
      for
  */
  return 0;
}

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

#define GRL_ "\033[1;37m"
#define GRL_B "\033[1;37m"

#define GRD_ "\033[1;90m"
#define GRD_B "\033[1;90m"

#define COLOR1 "\033[1;162;159m"
#define COLOR2 "\033[0;0;0m"


int main()
{
    if (isatty(STDOUT_FILENO))
        printf(WHI_ "Regular WHITE " WHI_ "Bold WHITE " OLDCOLOR "\n");
    else
        printf("This is pipe, no colors!\n");

    printf(STD_ "Regular WHITE " STD_B "Bold WHITE " OLDCOLOR "\n");
    printf(GRL_ "Regular WHITE " GRL_B "Bold WHITE " OLDCOLOR "\n");
    printf(GRD_ "Regular WHITE " GRD_B "Bold WHITE " OLDCOLOR "\n");

/*
    while
    for
*/
        return 0;
}
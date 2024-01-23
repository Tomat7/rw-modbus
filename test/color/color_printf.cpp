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

#define WH0 "\033[0;97m"
#define WH1 "\033[1;97m"

#define COLOR1 "\033[1;162;159m"
#define COLOR2 "\033[0;0;0m"


int main()
{
    if (isatty(STDOUT_FILENO))
        printf(WH0 "White " WH1 "Yellow " OLDCOLOR "\n");
    else
        printf("This is pipe, no colors!\n");



while
	for
		
return 0;
}

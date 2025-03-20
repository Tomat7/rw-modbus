#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#define PING_PROG "ping"
#define PING_OPT " -c5"
#define PING_ADDR " 1.1.1.1"
#define PING_OUTFILE "/tmp/ping.txt"

 
int main()
{
    std::string cmdline = PING_PROG + std::string(PING_OPT) + PING_ADDR + ">" +PING_OUTFILE;
//    std::system("ping -c5 1.1.1.1 >OUT_FILE"); // executes the UNIX command "ls -l >test.txt"
    std::system(cmdline.c_str()); // executes the UNIX command "ls -l >test.txt"
    std::cout << std::ifstream(PING_OUTFILE).rdbuf();
}

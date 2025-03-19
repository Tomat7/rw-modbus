#include <cstdlib>
#include <fstream>
#include <iostream>

#define OUT_FILE "/tmp/ping.txt"
 
int main()
{
    std::system("ping -c5 1.1.1.1 >OUT_FILE"); // executes the UNIX command "ls -l >test.txt"
    std::cout << std::ifstream("OUT_FILE").rdbuf();
}

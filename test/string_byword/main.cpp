#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#define LS_OUTFILE "ls.tmp"
#define LS_CMD "ls"
#define LS_EXT " bin/*.cpp"
using namespace std;

int main()
{
    string line_str = "a line of text to iterate through";
    string word;
    istringstream iss1(line_str, istringstream::in);

    while (iss1 >> word)
    {
        printf("%s\n", word.c_str()); // Do something on `word` here...
    }


    std::string cmdline = LS_CMD + string(LS_EXT) + (">") + LS_OUTFILE;
    std::system(cmdline.c_str()); // executes the UNIX command 

    std::cout << "===== All file in one" << endl;
    std::cout << std::ifstream(LS_OUTFILE).rdbuf();

    std::stringstream ss;
    ss << std::ifstream(LS_OUTFILE).rdbuf();

    // istringstream iss(ss.str(), istringstream::in);
    // stringstream iss(ss.str());

    std::cout << "===== Word by word" << endl;
    while (ss >> word)
    {
        printf("%s\n", word.c_str()); // Do something on `word` here...
    }
}


/* 
{
    std::string str("Split me by whitespaces");
    std::string buf;           // Have a buffer string
    std::stringstream ss(str); // Insert the string into a stream

    std::vector<std::string> tokens; // Create vector to hold our words

    while (ss >> buf)
        tokens.push_back(buf);

    return 0;
}

 */
// ====================================================
/* 
{
    string line = "a line of text to iterate through";
    string word;

    istringstream iss(line, istringstream::in);

    while (iss >> word)
    {
        printf("%s\n", word.c_str()); // Do something on `word` here...
    }
}
 */

// eof

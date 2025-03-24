#include <bits/stdc++.h>
#include <cassert>
#include <exception>
#include <iostream>

#define CPP_OPTIONS " -Wall -Werror -Wfatal-errors -O2 -std=c++17"
#define CPP_COMPILER "g++ "

int main (const int argc, const char **argv) {
try {
    assert (argc == 2);
    const std::string filename = (const std::string) argv [1];
    const std::string progname = "./" + filename + ".elf";
    std::string cmd_line = CPP_COMPILER + filename;
    cmd_line += CPP_OPTIONS + std::string(" -o ") + progname;
    //; // -L/usr/lib/x86_64-linux-gnu";
    //const std::string command = cmd_begin + end;

    std::cout << "Compiling file using " << cmd_line << '\n';
    assert (std::system ((const char *) cmd_line.c_str ()) == 0);

    std::cout << "Running file " << progname << '\n';
    assert (std::system ((const char *) progname.c_str()) == 0);

    return 0; }
catch (std::exception const& e) { std::cerr << e.what () << '\n'; std::terminate (); }
catch (...) { std::cerr << "Found an unknown exception." << '\n'; std::terminate (); } }

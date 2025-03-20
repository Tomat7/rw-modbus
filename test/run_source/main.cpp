#include <bits/stdc++.h>
#include <cassert>
#include <exception>
#include <iostream>

int main (const int argc, const char **argv) {
try {
    assert (argc == 2);
    const std::string filename = (const std::string) argv [1];
    const std::string begin = "g++ " + filename;
    const std::string end = " -Wall -Werror -Wfatal-errors -O2 -std=c++17 -o a.elf"; // -L/usr/lib/x86_64-linux-gnu";
    const std::string command = begin + end;
    std::cout << "Compiling file using " << command << '\n';

    assert (std::system ((const char *) command.c_str ()) == 0);
    std::cout << "Running file a.elf" << '\n';
    assert (std::system ((const char *) "./a.elf") == 0);

    return 0; }
catch (std::exception const& e) { std::cerr << e.what () << '\n'; std::terminate (); }
catch (...) { std::cerr << "Found an unknown exception." << '\n'; std::terminate (); } }

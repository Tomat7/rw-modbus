
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <format>


union value_u {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64 = 0;
  int64_t dt;
  float fl;
  double dbl;
};

struct value_s {
//    std::string str;
  char ch_arr[256] = { 0 };
  uint8_t sz8;
  size_t sz;
  value_u val;
  /* data */
};


int main()
{
  std::string filename = "Test.b";
  int step_wr = 0;
  int step_rd = 0;
  size_t str_sz = 0;

  try {
    std::string s = "string_name1234567890ABCDEF";
//        std::string str2 (64, ' ');
    std::ofstream out_f;
    double d = 3.1415926535897932384626433832795;
    value_u val;
    value_s vs;
    val.dbl = d * 7;

//        vs.str = std::to_string(d);
//        vs.sz = s.capacity();
//        std::cout << vs.sz << vs.str << "\n";
//        s.reserve(123);
//        vs.sz = s.capacity();
//        std::cout << vs.sz << vs.str << "\n";
//        str_sz = vs.sz;

    std::string s_dbl = std::format("{}", val.dbl);
    strcpy(vs.ch_arr, std::to_string(val.dbl).c_str());
    strcpy(vs.ch_arr, s_dbl.c_str());
    vs.sz8 = (uint8_t)strlen(vs.ch_arr);
    std::cout << "size8: " << std::format("{}", vs.sz8) << "\n";
    std::cout << "dbl: " << val.dbl << "\n";
    std::cout << "dbl_fmt: " << std::format("{}", val.dbl) << "\n";
    std::cout << "ch_arr: " << vs.ch_arr << "\n";
    // return 0;

    step_wr = 1;
    out_f.open(filename, std::ios::binary);
    // std::cout << step_wr << " - OPEN (out) done\n";

    if (out_f.is_open())
      std::cout << step_wr << " + OPEN (out) done\n";
    else
      std::cout << step_wr << " - NOT OPEN (out)! \n";

    out_f.exceptions(std::ifstream::failbit); // may throw
    // return 0;
    /*
            step_wr = 2;
            out_f.write(reinterpret_cast<char *>(&d), sizeof d); // binary output
            std::cout << "2 + WRITE double done\n";
            out_f.exceptions(out_f.failbit); // may throw
    */
    step_wr = 2;
    char c = ';';
    out_f << vs.sz8; // c << "str_len:" << strlen(vs.arr); // text output
    std::cout << step_wr << " + START size of arr done\n";
    out_f.exceptions(std::ifstream::failbit); // may thro
    out_f << c;
    // return 0;

    step_wr = 3;
    out_f.write(reinterpret_cast<char*>(vs.ch_arr), vs.sz8);  // binary output
    std::cout << step_wr << " + WRITE char_arr done\n";
    out_f.exceptions(out_f.failbit); // may throw
    out_f << c;
    // return 0;

    step_wr = 4;
    out_f.write(reinterpret_cast<char*>(&val), sizeof val);  // binary output
    std::cout << step_wr << " + WRITE val_union done\n";
    out_f.exceptions(out_f.failbit/* std::ifstream::failbit */); // may throw
    out_f << c;
    // return 0;

    step_wr = 5;
    //    s = std::to_string(d);
    //    std::cout << s << "\n";
    //    out_f << /* "float" << d << */ /* "int" << */ 123 << c << s << '\n'; // text output
    //    std::cout << step_wr << " + WRITE out_stream done\n";
    //    out_f.exceptions(std::ifstream::failbit); // may throw
    // return 0;

    std::cout << "+++ finish!\n";
  } catch (const std::ios_base::failure &fail) {
    // handle exception here
    std::cout << step_wr << " - OUT exception: "
              << fail.what() << " code: "
              << fail.code() << '\n';
  }

  // return 0;

  // filename = "Test.x";
  // read back
  try {
    double d = 0.0;
    char c;
    //value_u val;
    value_s vs;
    std::ifstream in_f;

    step_rd = 1;
    in_f.open(filename, std::ios::binary);

    if (in_f.is_open())
      std::cout << step_rd << " + OPEN (out) done\n";
    else
      std::cout << step_rd << " - NOT OPEN (out)! \n";

    in_f.exceptions(std::ifstream::failbit); // may throw
    /*
            step_rd = 2;
            in_f.read(reinterpret_cast<char *>(&d), sizeof d);
            in_f.exceptions(std::ifstream::failbit); // may throw
            std::cout << "+++ read back: " << d << ' '; */

    /*
            step_rd = 3;
            in_f.read(reinterpret_cast<char *>(&val), sizeof val);
            in_f.exceptions(std::ifstream::failbit); // may throw
            std::cout << "+ val.dbl: " << val.dbl << ' ';
    */

    step_rd = 2;
    in_f.read(reinterpret_cast<char*>(&vs.sz8), sizeof vs.sz8);
    in_f.exceptions(std::ifstream::failbit); // may throw
    std::cout << step_rd << " + READ size of arr: " << vs.sz8 << "\n";
//        std::cout << "+ str: " << vs.str << ": " << vs.val.dbl << ' ';

    step_rd = 3;
    in_f >> c;
    in_f.exceptions(std::ifstream::failbit); // may throw
    std::cout << step_rd << " + DELIMITER1 done" << c << "\n";

    step_rd = 4;
    in_f.read(reinterpret_cast<char*>(vs.ch_arr), vs.sz8);  // binary output
    std::cout << step_rd << " + READ char_arr done\n";
    in_f.exceptions(in_f.failbit); // may throw
    std::cout << vs.ch_arr << "\n";

    step_rd = 5;
    in_f >> c;
    in_f.exceptions(std::ifstream::failbit); // may throw
    std::cout << step_rd << " + DELIMITER2 done" << c << "\n";

    step_rd = 6;
    in_f.read(reinterpret_cast<char*>(&vs.val), sizeof vs.val);  // binary output
    std::cout << step_rd << " + READ val_union done\n";
    in_f.exceptions(in_f.failbit/* std::ifstream::failbit */); // may throw
    std::cout << " +++ DOUBLE value: " << vs.val.dbl << "\n";
    std::cout << " +++ DOUBLE__frmt: " << std::format("{}", vs.val.dbl) << "\n";

//      int n;
//      std::string s1, s2, s3;
//      std::string s4 = "3.1415926";
//      char c;
//      float f = 0.0f;

    //std::cout << s1 << ' ' << f << ' ' << s2 << ' ' << n << ' ' << s3 << '\n';
//        double dd = stod(s4);
//        std::cout << n << ' ' << s3 << ' ' << dd << '\n';


  } catch (const std::ios_base::failure &fail) {
    // handle exception here
    std::cout << step_rd << " - IN exception: "
              << fail.what() << " code: "
              << fail.code() << '\n';
  }

  return 0;
}

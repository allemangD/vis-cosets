#include "files.hpp"

#include <fstream>
#include <streambuf>

std::string read_all_text(const std::string &file) {
   std::ifstream f(file);
   std::string text;

   f.seekg(0, std::ios::end);
   text.reserve(f.tellg());
   f.seekg(0, std::ios::beg);

   text.assign((std::istreambuf_iterator<char>(f)),
      std::istreambuf_iterator<char>());

   return text;
}

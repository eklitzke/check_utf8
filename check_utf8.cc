// Check if a file is valid UTF-8, and return 0 if it is and 1 if not. This
// reads the whole file into memory to perform the check, so beware using it
// with large files.

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

bool is_valid_utf8(const std::string &src) {
  int n = 0;
  for (const char &i : src) {
    int c = static_cast<int>(i) & 0xFF;
    if (n) {
      if ((c & 0xC0) != 0x80) {
        return false;
      }
      n--;
      continue;
    }
    if (c < 0x80) {
      n = 0; // 0bbbbbbb
    } else if ((c & 0xE0) == 0xC0) {
      n = 1; // 110bbbbb
    } else if ((c & 0xF0) == 0xE0) {
      n = 2; // 1110bbbb
    } else if ((c & 0xF8) == 0xF0) {
      n = 3; // 11110bbb
    } else if ((c & 0xFC) == 0xF8) {
      n = 4; // 111110bb
    } else if ((c & 0xFE) == 0xFC) {
      n = 5; // 1111110b
    } else {
      return false;
    }
  }
  return n == 0;
}

int main(int argc, char **argv) {
    std::ifstream ifs(argv[1]);
    ifs.seekg(0, std::ifstream::end);
    int length = ifs.tellg();
    if (length < 0) {
        std::cerr << "failed to tellg\n";
        return 1;
    }
    ifs.seekg(0, std::istream::beg);

    std::unique_ptr<char[]> buf(new char[length]);
    ifs.read(buf.get(), length);
    ifs.close();

    return is_valid_utf8(buf.get()) == true ? 0 : 1;
}

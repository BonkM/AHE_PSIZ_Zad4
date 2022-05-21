#include <iostream>
#include <string>

int main(int argc, char** argv) {
  if (argc < 3 || argc % 2 == 0) {
    std::cerr << "Usage: gen <byte 1> <byte 1 count> <byte 2> <byte 2 count> ... <byte n> <byte n count>" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; i += 2) {
    std::string b_str(argv[i]), bc_str(argv[i+1]);
    uint8_t b = b_str.size() == 0 ? 0 : b_str[0];
    unsigned long long bc = std::stoull(bc_str);
    for (unsigned long long i = 0; i < bc; i++) {
      std::cout << (char)b;
    }
  }

  return 0;
}

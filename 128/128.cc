#include <iostream>
#include <string>

std::string Format(int x) {
  const char hex_digits[] = "0123456789ABCDEF";
  int hex[] = {(x >> 4) & 0x0f, x & 0x0f};
  char hex_str[] = {hex_digits[hex[0]], hex_digits[hex[1]], '\0'};
  return std::string(hex_str);
}

int main() {
  const int g = 34943;

  for (std::string line; std::getline(std::cin, line) && line[0] != '#';) {
    int m = 0;
    for (const auto c : line)
      m = ((m << 8) + c) % g;
    int m2 = m << 16;
    int crc = (g - m2 % g) % g;
    std::cout << Format(crc >> 8) << ' ' << Format(crc & 0xff) << std::endl;
  }

  return 0;
}

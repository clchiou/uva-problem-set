#include <cassert>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <map>
#include <string>
#include <iostream>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

#define LOG  \
  if (DEBUG) \
  std::cerr

struct PriceInfo {
  std::string locality;
  int price;  // cent per minute.
};

typedef std::map<std::string, PriceInfo> PriceInfoMap;

std::string::const_iterator Find(std::string::const_iterator pos,
                                 std::string::const_iterator end,
                                 bool (*pred)(char c)) {
  while (pos != end && !pred(*pos)) {
    ++pos;
  }
  return pos;
}

void Print(const std::string& number,
           const std::string& locality,
           const std::string& subscriber,
           int duration,
           int price,
           int cost) {
  char buf[32];

  std::cout << std::left << std::setw(16) << number;
  std::cout << std::left << std::setw(25) << locality;
  std::cout << std::right << std::setw(10) << subscriber;
  std::cout << std::right << std::setw(5) << duration;

  if (price >= 0) {
    snprintf(buf, sizeof(buf), "%.2f", price / 100.0);
  } else {
    buf[0] = '\0';
  }
  std::cout << std::right << std::setw(6) << buf;

  snprintf(buf, sizeof(buf), "%.2f", cost / 100.0);
  std::cout << std::right << std::setw(7) << buf;

  std::cout << std::endl;
}

int main() {
  PriceInfoMap infos;
  {
    std::string line;
    while (std::getline(std::cin, line) && line != "000000") {
      auto IsSpace = [](char c) { return static_cast<bool>(isspace(c)); };
      auto IsDollarSign = [](char c) { return c == '$'; };

      auto pos = line.cbegin();
      auto end = line.cend();
      auto next = Find(pos, end, IsSpace);
      assert(next != end);
      std::string code(pos, next);

      pos = ++next;
      next = Find(pos, end, IsDollarSign);
      assert(next != end);
      PriceInfo info{std::string(pos, next),
                     atoi(std::string(next + 1, end).c_str())};

      LOG << "code=" << code << ' ' << "locality=" << info.locality << ' '
          << "price=" << info.price << std::endl;
      infos[code] = info;
    }
  }

  {
    std::string number;
    int duration;
    while (std::cin >> number && number != "#") {
      assert(number.size() > 0);
      std::cin >> duration;
      LOG << "number=" << number << ' ' << "duration=" << duration << std::endl;

      // Test and print for IDD or STD.
      {
        bool found = false;
        std::string prefix;
        prefix.reserve(number.size());
        for (auto pos = number.cbegin() + 1; pos != number.cend(); ++pos) {
          prefix.assign(number.cbegin(), pos);
          LOG << "prefix=" << prefix << std::endl;
          auto pair = infos.find(prefix);
          if (pair == infos.end()) {
            continue;
          }
          std::string subscriber = number.substr(pos - number.cbegin());
          // Is it IDD?
          if (prefix.size() >= 2 && prefix[0] == '0' && prefix[1] == '0') {
            if (!(4 <= subscriber.size() && subscriber.size() <= 10)) {
              continue;
            }
          }
          // Is it STD?
          if (prefix.size() >= 2 && prefix[0] == '0' && prefix[1] != '0') {
            if (!(4 <= subscriber.size() && subscriber.size() <= 7)) {
              continue;
            }
          }

          const auto& info = pair->second;
          Print(number,
                info.locality,
                subscriber,
                duration,
                info.price,
                info.price * duration);
          found = true;
          break;
        }
        if (found) {
          continue;
        }
      }

      // Test and print for local.
      if (number[0] != '0') {
        Print(number, "Local", number, duration, 0, 0);
        continue;
      }

      // Test and print for unknown.
      Print(number, "Unknown", "", duration, -1, -100);
    }
  }

  return 0;
}

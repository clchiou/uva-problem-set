#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

int ComputeHash(const int words[], int num_words) {
  int c = 1;
  while (1) {
    int c_candidate = 0;
    for (int i = 0; i < num_words; i++)
      for (int j = i + 1; j < num_words; j++)
        if (c / words[i] % num_words == c / words[j] % num_words) {
          int c_next = std::min((c / words[i] + 1) * words[i],
                                (c / words[j] + 1) * words[j]);
          c_candidate = std::max(c_candidate, c_next);
        }
    if (c_candidate == 0)
      break;
    c = c_candidate;
  }
  return c;
}

int Convert(const std::string& word) {
  int code = 0;
  for (char c : word)
    code = (code << 5) + c - 'a' + 1;
  return code;
}

int main() {
  for (std::string line; std::getline(std::cin, line);) {
    int words[13], num_words = 0;
    std::stringstream sin(line);
    for (std::string word; sin >> word;)
      words[num_words++] = Convert(word);
    std::sort(words, words + num_words);
    std::cout << line << std::endl
              << ComputeHash(words, num_words) << std::endl
              << std::endl;
  }
  return 0;
}

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

struct PosRecord {
  std::vector<std::string>::const_iterator line;
  std::string::const_iterator begin, end;
};

void Strip(std::string* str);
void ToLower(std::string* str);
std::string::const_iterator Until(std::string::const_iterator first,
                                  std::string::const_iterator last,
                                  int (*predicate)(int));

int main() {
  std::vector<std::string> lines;
  std::set<std::string> ignore_words, keywords;
  std::map<std::string, std::vector<PosRecord>> pos_table;

  for (std::string line; std::getline(std::cin, line);) {
    Strip(&line);
    if (line == "::") {
      break;
    }
    ToLower(&line);
    if (DEBUG) {
      std::cerr << "  ignore=\"" << line << '\"' << std::endl;
    }
    ignore_words.insert(line);
  }

  for (std::string line; std::getline(std::cin, line);) {
    ToLower(&line);
    lines.push_back(line);
  }

  for (auto l = lines.cbegin(); l != lines.cend(); ++l) {
    for (auto end = l->cend(), i = Until(l->cbegin(), end, isalpha), j = i;
         i != end;
         i = Until(j, end, isalpha)) {
      j = Until(i, end, isspace);
      std::string word(i, j);
      if (ignore_words.find(word) != ignore_words.cend()) {
        continue;
      }
      keywords.insert(word);
      PosRecord pos = {l, i, j};
      pos_table[word].push_back(pos);
    }
  }

  for (const auto& keyword : keywords) {
    for (const auto& pos : pos_table[keyword]) {
      std::string line;
      auto i = std::back_inserter(line);
      std::copy(pos.line->cbegin(), pos.begin, i);
      std::transform(pos.begin, pos.end, i, toupper);
      std::copy(pos.end, pos.line->cend(), i);
      std::cout << line << std::endl;
    }
  }

  return 0;
}

void Strip(std::string* str) {
  if (str->empty()) {
    return;
  }
  auto i = str->cbegin();
  auto j = str->cend() - 1;
  while (i != j && isspace(*i)) {
    ++i;
  }
  while (i != j && isspace(*j)) {
    --j;
  }
  *str = std::string(i, j + 1);
}

void ToLower(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(), tolower);
}

std::string::const_iterator Until(std::string::const_iterator first,
                                  std::string::const_iterator last,
                                  int (*predicate)(int)) {
  while (first != last) {
    if (predicate(*first)) {
      break;
    }
    ++first;
  }
  return first;
}

#include <algorithm>
#include <bitset>
#include <cctype>
#include <iostream>
#include <memory>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

class UsedLetters {
 public:
  bool used(char c) const { return letters_[index(c)]; }
  void used(char c, bool value) { letters_[index(c)] = value; }

 private:
  static int index(char c) {
    assert(isalpha(c));
    if (isupper(c))
      return c - 'A';
    else
      return c - 'a' + 26;
  }

  std::bitset<52> letters_;
};

void ComputePermutation_Impl(const char* word,
                             int word_size,
                             int depth,
                             bool* used,
                             char* output) {
  if (depth == word_size) {
    std::cout << output << std::endl;
    return;
  }

  UsedLetters used_letters;
  for (int i = 0; i < word_size; i++) {
    char c = word[i];
    if (used[i] || used_letters.used(c))
      continue;
    used[i] = true;
    used_letters.used(c, true);
    output[depth] = c;
    ComputePermutation_Impl(word, word_size, depth + 1, used, output);
    used[i] = false;
  }
}

void ComputePermutation(const std::string& word) {
  std::unique_ptr<bool[]> used_ptr(new bool[word.size()]);
  bool* used = used_ptr.get();
  std::fill(used, used + word.size(), false);

  std::unique_ptr<char[]> output_ptr(new char[word.size() + 1]);
  char* output = output_ptr.get();
  std::fill(output, output + word.size() + 1, '\0');

  ComputePermutation_Impl(word.c_str(), word.size(), 0, used, output);
}

// NOTE: The problem requires a "zigzag" ordering:
//   A < a < B < b < C < c < ... < Z < z
bool IsLessThan(char p, char q) {
  char p_lower = tolower(p);
  char q_lower = tolower(q);
  if (p_lower == q_lower)
    return isupper(p);
  else
    return p_lower < q_lower;
}

int main() {
  int num_words;
  std::cin >> num_words;
  while (num_words--) {
    std::string word;
    std::cin >> word;
    std::sort(word.begin(), word.end(), IsLessThan);
    ComputePermutation(word);
  }
  return 0;
}

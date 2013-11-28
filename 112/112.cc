#include <cctype>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

enum {
  LEFT_PARENTHESES = '(',
  RIGHT_PARENTHESES = ')',
};

static std::istream& GetChar(std::istream& in, char* c_out) {
  char c = '\0';
  while (in.get(c) && isspace(c))
    ;
  *c_out = c;
  return in;
}

static std::istream& GetInt(std::istream& in, int* i_out) {
  int i = 0, sign = 1;
  char c;
  while (GetChar(in, &c)) {
    if (c == '-') {
      sign = -1;
    } else if (isdigit(c)) {
      i = i * 10 + c - '0';
    } else {
      in.unget();
      break;
    }
  }
  *i_out = sign * i;
  return in;
}

class Tree {
 private:
  Tree(int value, Tree* left, Tree* right)
      : value_(value), left_(left), right_(right) {}

 public:
  ~Tree() {
    delete left_;
    delete right_;
  }

  static Tree* Parse(std::istream& in) {
    Tree* tree = NULL;
    char c;

    GetChar(in, &c);
    assert(c == LEFT_PARENTHESES);

    GetChar(in, &c);
    if (c != RIGHT_PARENTHESES) {
      assert(c == '-' || isdigit(c));
      int value;
      in.unget();
      GetInt(in, &value);
      Tree* left = Parse(in);
      Tree* right = Parse(in);
      tree = new Tree(value, left, right);
      GetChar(in, &c);
    }

    assert(c == RIGHT_PARENTHESES);

    return tree;
  }

  static bool FindSumOfPaths(Tree* tree, int sum) {
    // Special case of empty tree.
    if (!tree)
      return false;
    return FindSumOfPathsImpl(tree, sum, 0);
  }

 private:
  static bool FindSumOfPathsImpl(Tree* tree, int sum, int partial_sum) {
    partial_sum += tree->value_;

    if (!tree->left_ && !tree->right_)
      return sum == partial_sum;

    if (tree->left_ && FindSumOfPathsImpl(tree->left_, sum, partial_sum))
      return true;

    if (tree->right_ && FindSumOfPathsImpl(tree->right_, sum, partial_sum))
      return true;

    return false;
  }

  int value_;
  Tree* left_;
  Tree* right_;
};

int main() {
  int sum;
  while (GetInt(std::cin, &sum)) {
    Tree* tree = Tree::Parse(std::cin);
    if (Tree::FindSumOfPaths(tree, sum))
      std::cout << "yes" << std::endl;
    else
      std::cout << "no" << std::endl;
    delete tree;
  }
  return 0;
}

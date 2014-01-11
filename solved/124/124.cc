#include <algorithm>
#include <bitset>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int MAX_NUM_VARIABLES = 20;
const int MAX_VARIABLE = 26;  // Variable index of 'a' to 'z'.

class Variable {
  friend std::ostream& operator<<(std::ostream& out, const Variable& var);

 public:
  static Variable FromIndex(int index);

  Variable() : var_(0) {}
  explicit Variable(char var) : var_(var) {}

  bool operator<(const Variable& other) const {
    assert('a' <= var_ && var_ <= 'z');
    assert('a' <= other.var_ && other.var_ <= 'z');
    return var_ < other.var_;
  }

  bool operator==(const Variable& other) const {
    assert('a' <= var_ && var_ <= 'z');
    assert('a' <= other.var_ && other.var_ <= 'z');
    return var_ == other.var_;
  }

  int index() const {
    assert('a' <= var_ && var_ <= 'z');
    return var_ - 'a';
  }

 private:
  char var_;
};

class Variables {
  friend std::istream& operator>>(std::istream& in, Variables& vars);
  friend std::ostream& operator<<(std::ostream& out, const Variables& vars);

 public:
  Variables() : num_vars_(0) {}

  int num_vars() const { return num_vars_; }
  const Variable* begin() const { return vars_; }
  const Variable* end() const { return vars_ + num_vars_; }

 private:
  int num_vars_;
  Variable vars_[MAX_NUM_VARIABLES];
};

class Constraints {
  friend std::istream& operator>>(std::istream& in, Constraints& constraints);
  friend std::ostream& operator<<(std::ostream& out,
                                  const Constraints& constraints);

 public:
  Constraints();

  bool at(const Variable& x, const Variable& y) const {
    return at(x.index(), y.index());
  }

  bool at(int i, int j) const {
    assert(0 <= i && i < MAX_VARIABLE && 0 <= j && j < MAX_VARIABLE);
    return constraints_[i][j];
  }

 private:
  int num_vars_;
  bool constraints_[MAX_VARIABLE][MAX_VARIABLE];
};

// main

std::ostream& operator<<(std::ostream& out, const std::set<Variable>& vars) {
  for (const auto& x : vars)
    out << x;
  return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<Variable>& vars) {
  for (const auto& x : vars)
    out << x;
  return out;
}

void Enumerate_Impl(const Variables& vars,
                    const Constraints& constraints,
                    std::bitset<MAX_VARIABLE>* is_var_used,
                    std::set<Variable>* free_vars,
                    std::vector<Variable>* output_vars) {
  if (output_vars->size() == static_cast<size_t>(vars.num_vars())) {
    std::cout << *output_vars << std::endl;
    return;
  }

  for (const auto& free_var : *free_vars) {
    assert(!(*is_var_used)[free_var.index()]);

    output_vars->push_back(free_var);
    (*is_var_used)[free_var.index()] = true;

    std::set<Variable> child_free_vars(*free_vars);
    child_free_vars.erase(
        std::find(child_free_vars.begin(), child_free_vars.end(), free_var));
    for (const auto& y : vars) {
      if ((*is_var_used)[y.index()])
        continue;
      bool is_free = true;
      for (const auto& x : vars)
        if (!(*is_var_used)[x.index()] && constraints.at(x, y)) {
          is_free = false;
          break;
        }
      if (is_free)
        child_free_vars.insert(y);
    }

    Enumerate_Impl(
        vars, constraints, is_var_used, &child_free_vars, output_vars);

    output_vars->pop_back();
    (*is_var_used)[free_var.index()] = false;
  }
}

void Enumerate(const Variables& vars, const Constraints& constraints) {
  std::bitset<MAX_VARIABLE> is_var_used;
  std::vector<Variable> output_vars;
  std::set<Variable> free_vars;
  for (const auto& y : vars) {
    bool is_free_var = true;
    for (const auto& x : vars)
      if (constraints.at(x, y)) {
        is_free_var = false;
        break;
      }
    if (is_free_var)
      free_vars.insert(y);
  }
  Enumerate_Impl(vars, constraints, &is_var_used, &free_vars, &output_vars);
}

int main() {
  Variables vars;
  Constraints constraints;
  bool first = true;
  while (std::cin >> vars >> constraints) {
    if (!first)
      std::cout << std::endl;
    first = false;
    Enumerate(vars, constraints);
  }
  return 0;
}

// class Variable

std::ostream& operator<<(std::ostream& out, const Variable& var) {
  assert('a' <= var.var_ && var.var_ <= 'z');
  return out << var.var_;
}

Variable Variable::FromIndex(int index) {
  assert(0 <= index && index < MAX_VARIABLE);
  return Variable('a' + index);
}

// class Variables

std::istream& operator>>(std::istream& in, Variables& vars) {
  std::string line;
  if (std::getline(in, line)) {
    vars.num_vars_ = 0;
    std::stringstream sin(line);
    for (std::string var; sin >> var; vars.num_vars_++) {
      assert(vars.num_vars_ < MAX_NUM_VARIABLES);
      assert(var.size() == 1);
      vars.vars_[vars.num_vars_] = Variable(var[0]);
    }
    std::sort(vars.vars_, vars.vars_ + vars.num_vars_);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Variables& vars) {
  bool first = true;
  for (const auto& x : vars) {
    (first ? out : out << ' ') << x;
    first = false;
  }
  out << std::endl;
  return out;
}

// class Constraints

std::istream& operator>>(std::istream& in, Constraints& constraints) {
  std::string line;
  if (std::getline(in, line)) {
    Constraints* cons = new (&constraints) Constraints();
    std::stringstream sin(line);
    for (std::string s, t; sin >> s >> t;) {
      assert(s.size() == 1 && t.size() == 1 && s[0] != t[0]);
      Variable x(s[0]), y(t[0]);
      cons->constraints_[x.index()][y.index()] = true;
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Constraints& constraints) {
  bool first = true;
  for (int i = 0; i < MAX_VARIABLE; i++)
    for (int j = 0; j < MAX_VARIABLE; j++)
      if (constraints.at(i, j)) {
        (first ? out : out << ' ') << Variable::FromIndex(i) << ' '
                                   << Variable::FromIndex(j);
        first = false;
      }
  out << std::endl;
  return out;
}

Constraints::Constraints() {
  for (int i = 0; i < MAX_VARIABLE; i++)
    std::fill(constraints_[i], constraints_[i] + MAX_VARIABLE, false);
}

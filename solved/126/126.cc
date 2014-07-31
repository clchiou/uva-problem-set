#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

struct Term {
  Term(int coef_, int xexp_, int yexp_);
  int coef, xexp, yexp;
};

typedef std::vector<Term> Terms;

class Polynomial {
 public:
  explicit Polynomial(const std::string& expr);
  explicit Polynomial(const Terms& terms);

  Terms::const_iterator begin() const;
  Terms::const_iterator end() const;

 private:
  Terms terms_;
};

bool operator<(const Term& lhs, const Term& rhs);

Terms ParseTerms(const std::string& expr);

void ParseInt(const std::string& str, int default_value, int* out);

std::ostream& operator<<(std::ostream& out, const Term& term);
std::ostream& operator<<(std::ostream& out, const Polynomial& polynomial);

Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);

void WriteChar(char c, std::string* str, std::string* co_str);
void WriteString(const std::string& v, std::string* str, std::string* co_str);
void WriteInt(int n, std::string* str, std::string* co_str);

int main() {
  for (std::string str0, str1; std::cin >> str0 >> str1 && str0 != "#\n";) {
    Polynomial polynomial0(str0);
    Polynomial polynomial1(str1);
    Polynomial polynomial = polynomial0 * polynomial1;
    if (DEBUG) {
      std::cerr << "p0 =" << polynomial0 << std::endl;
      std::cerr << "p1 =" << polynomial1 << std::endl;
      std::cerr << "p0 * p1 =" << polynomial << std::endl;
    }

    std::string base_line, exp_line;
    bool first = true;
    for (const Term& term : polynomial) {
      assert(term.coef != 0);
      if (first) {
        if (term.xexp == 0 && term.yexp == 0) {
          WriteInt(term.coef, &base_line, &exp_line);
        } else if (term.coef == -1) {
          WriteChar('-', &base_line, &exp_line);
        } else if (term.coef < -1 || 1 < term.coef) {
          WriteInt(term.coef, &base_line, &exp_line);
        } else {
          assert(term.coef == 1);
        }
      } else {
        WriteChar(' ', &base_line, &exp_line);
        WriteChar(term.coef < 0 ? '-' : '+', &base_line, &exp_line);
        WriteChar(' ', &base_line, &exp_line);
        int abs_coef = abs(term.coef);
        if (term.xexp == 0 && term.yexp == 0) {
          WriteInt(abs_coef, &base_line, &exp_line);
        } else if (abs_coef > 1) {
          WriteInt(abs_coef, &base_line, &exp_line);
        } else {
          assert(abs_coef == 1);
        }
      }
      first = false;

      if (term.xexp >= 1) {
        WriteChar('x', &base_line, &exp_line);
      }
      if (term.xexp >= 2) {
        WriteInt(term.xexp, &exp_line, &base_line);
      }
      if (term.yexp >= 1) {
        WriteChar('y', &base_line, &exp_line);
      }
      if (term.yexp >= 2) {
        WriteInt(term.yexp, &exp_line, &base_line);
      }
    }

    std::cout << exp_line << std::endl;
    std::cout << base_line << std::endl;
  }
  return 0;
}

void WriteChar(char c, std::string* str, std::string* co_str) {
  str->push_back(c);
  co_str->push_back(' ');
}

void WriteString(const std::string& v, std::string* str, std::string* co_str) {
  for (char c : v) {
    WriteChar(c, str, co_str);
  }
}

void WriteInt(int n, std::string* str, std::string* co_str) {
  WriteString(std::to_string(n), str, co_str);
}

Term::Term(int coef_, int xexp_, int yexp_)
    : coef(coef_), xexp(xexp_), yexp(yexp_) {}

bool operator<(const Term& lhs, const Term& rhs) {
  if (lhs.xexp != rhs.xexp) {
    return lhs.xexp > rhs.xexp;
  } else {
    return lhs.yexp < rhs.yexp;
  }
}

Terms ParseTerms(const std::string& expr) {
  Terms terms;
  enum State { STATE_BEGIN, STATE_COEF, STATE_EXP, } state = STATE_BEGIN;
  int coef = 1, xexp = 0, yexp = 0;
  char exp = '\0';
  std::string buffer;
  for (auto i = expr.cbegin(), j = i;; ++i) {
    if (i == expr.cend() || *i == '\n') {
      if (state == STATE_COEF) {
        ParseInt(buffer.assign(j, i), 1, &coef);
        terms.push_back(Term(coef, xexp, yexp));
      } else {
        assert(state == STATE_EXP);
        ParseInt(buffer.assign(j, i), 1, exp == 'x' ? &xexp : &yexp);
        terms.push_back(Term(coef, xexp, yexp));
      }
      break;
    }
    assert(!isspace(*i));
    switch (state) {
      case STATE_BEGIN:
        coef = 1;
        xexp = 0;
        yexp = 0;
        if (*i == '+' || *i == '-' || isdigit(*i)) {
          j = i;
          state = STATE_COEF;
        } else if (*i == 'x' || *i == 'y') {
          exp = *i;
          j = i + 1;
          state = STATE_EXP;
        } else {
          assert(false);
        }
        break;
      case STATE_COEF:
        if (*i == '+' || *i == '-') {
          ParseInt(buffer.assign(j, i), 1, &coef);
          terms.push_back(Term(coef, xexp, yexp));
          --i;
          state = STATE_BEGIN;
        } else if (*i == 'x' || *i == 'y') {
          ParseInt(buffer.assign(j, i), 1, &coef);
          exp = *i;
          j = i + 1;
          state = STATE_EXP;
        } else {
          assert(isdigit(*i));
        }
        break;
      case STATE_EXP:
        // NOTE: Exponents are positive integers.
        if (*i == '+' || *i == '-') {
          ParseInt(buffer.assign(j, i), 1, exp == 'x' ? &xexp : &yexp);
          terms.push_back(Term(coef, xexp, yexp));
          --i;
          state = STATE_BEGIN;
        } else if (*i == 'x' || *i == 'y') {
          ParseInt(buffer.assign(j, i), 1, exp == 'x' ? &xexp : &yexp);
          exp = *i;
          j = i + 1;
          state = STATE_EXP;
        } else {
          assert(isdigit(*i));
        }
        break;
      default:
        assert(false);
    }
  }
  return terms;
}

void ParseInt(const std::string& str, int default_value, int* out) {
  if (str.empty()) {
    *out = default_value;
  } else if (str == "+") {
    *out = 1;
  } else if (str == "-") {
    *out = -1;
  } else {
    *out = std::stoi(str);
  }
}

Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
  std::map<std::pair<int, int>, int> term_map;
  for (const auto& lhs_term : lhs) {
    for (const auto& rhs_term : rhs) {
      int coef = lhs_term.coef * rhs_term.coef;
      int xexp = lhs_term.xexp + rhs_term.xexp;
      int yexp = lhs_term.yexp + rhs_term.yexp;
      term_map[std::make_pair(xexp, yexp)] += coef;
    }
  }
  Terms terms;
  auto convert = [](const std::pair<std::pair<int, int>, int>& term) {
    return Term(term.second, term.first.first, term.first.second);
  };
  std::transform(
      term_map.cbegin(), term_map.cend(), std::back_inserter(terms), convert);
  return Polynomial(terms);
}

Polynomial::Polynomial(const Terms& terms) {
  std::copy_if(terms.cbegin(),
               terms.cend(),
               std::back_inserter(terms_),
               [](const Term& term) { return term.coef != 0; });
  std::sort(terms_.begin(), terms_.end());
}

Polynomial::Polynomial(const std::string& expr)
    : Polynomial(ParseTerms(expr)) {}

Terms::const_iterator Polynomial::begin() const { return terms_.cbegin(); }
Terms::const_iterator Polynomial::end() const { return terms_.cend(); }

std::ostream& operator<<(std::ostream& out, const Term& term) {
  return out << term.coef << 'x' << term.xexp << 'y' << term.yexp;
}

std::ostream& operator<<(std::ostream& out, const Polynomial& polynomial) {
  for (const auto& term : polynomial) {
    out << ' ' << term;
  }
  return out;
}

#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

#ifdef ONLINE_JUDGE
struct NullStream {};
template <typename T>
NullStream& operator<<(NullStream& out, T const&) {
  return out;
}
NullStream& operator<<(NullStream& out, std::ostream&(std::ostream&)) {
  return out;
}
static NullStream NullLogStream;
#define LOG \
  if (0)    \
  NullLogStream
#else
#define LOG std::cerr
#endif

enum TokenKind {
  KIND_NONE,
  KIND_A,
  KIND_MOD,
  KIND_BA,
  KIND_DA,
  KIND_LA,
  KIND_NAME,
  KIND_PREDA,
};

struct Token {
  explicit Token(std::string string_);
  TokenKind kind;
  std::string string;
};

bool IsVowel(char c) {
  switch (c) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return true;
    default:
      return false;
  }
}

TokenKind ToTokenKind(const std::string& token) {
  if (!IsVowel(token.back())) {
    return KIND_NAME;
  }
  if (token.size() == 1 && IsVowel(token.front())) {
    return KIND_A;
  }
  if (token.size() == 2 && token.front() == 'g' && IsVowel(token.back())) {
    return KIND_MOD;
  }
  if (token.size() == 2 && token.front() == 'b' && IsVowel(token.back())) {
    return KIND_BA;
  }
  if (token.size() == 2 && token.front() == 'd' && IsVowel(token.back())) {
    return KIND_DA;
  }
  if (token.size() == 2 && token.front() == 'l' && IsVowel(token.back())) {
    return KIND_LA;
  }
  if (token.size() == 5 &&
      ((!IsVowel(token[0]) && !IsVowel(token[1]) && IsVowel(token[2]) &&
        !IsVowel(token[3]) && IsVowel(token[4])) ||
       (!IsVowel(token[0]) && IsVowel(token[1]) && !IsVowel(token[2]) &&
        !IsVowel(token[3]) && IsVowel(token[4])))) {
    return KIND_PREDA;
  }
  // The problem says:
  //   You can assume that all words will be correctly formed.
  // But this is not very accurate.  Input data have illegal strings and
  // they have to be treated as names.
  return KIND_NAME;
}

std::ostream& operator<<(std::ostream& out, TokenKind kind) {
  switch (kind) {
    case KIND_NONE:
      out << "KIND_NONE";
      break;
    case KIND_A:
      out << "KIND_A";
      break;
    case KIND_MOD:
      out << "KIND_MOD";
      break;
    case KIND_BA:
      out << "KIND_BA";
      break;
    case KIND_DA:
      out << "KIND_DA";
      break;
    case KIND_LA:
      out << "KIND_LA";
      break;
    case KIND_NAME:
      out << "KIND_NAME";
      break;
    case KIND_PREDA:
      out << "KIND_PREDA";
      break;
  }
  return out;
}

Token::Token(std::string string_)
    : kind(ToTokenKind(string_)), string(std::move(string_)) {}

std::ostream& operator<<(std::ostream& out, const Token& token) {
  return out << "Token(" << token.kind << ", " << token.string << ")";
}

typedef std::vector<Token> Tokens;
typedef std::vector<Token>::const_iterator Iterator;
typedef std::tuple<bool, Iterator> Result;

#define B Result result_

#define R(okay_, pos_)                          \
  do {                                          \
    result_ = std::make_tuple((okay_), (pos_)); \
    goto exit;                                  \
  } while (0)

#define E                              \
  exit:                                \
  if (DEBUG && std::get<0>(result_)) { \
    LOG << ' ' << __func__;            \
  }                                    \
  return result_

Result Match(Iterator pos, Iterator end, TokenKind kind) {
  B;
  if (pos != end && pos->kind == kind) {
    LOG << ' ' << *pos;
    R(true, ++pos);
  } else {
    R(false, pos);
  }
  E;
}

Result ParsePredString(Iterator pos, Iterator end) {
  // PredString = KIND_PREDA | KIND_PREDA PredString
  B;
  bool okay;
  std::tie(okay, pos) = Match(pos, end, KIND_PREDA);
  if (!okay) {
    R(false, pos);
  }
  while (okay) {
    std::tie(okay, pos) = Match(pos, end, KIND_PREDA);
  }
  R(true, pos);
  E;
}

Result ParsePredName(Iterator pos, Iterator end) {
  B;
  bool okay;
  Iterator p;
  // PredName = KIND_LA PredString
  p = pos;
  do {
    std::tie(okay, p) = Match(p, end, KIND_LA);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParsePredString(p, end);
    if (okay) {
      R(true, p);
    }
  } while (0);
  // PredName = KIND_NAME
  p = pos;
  std::tie(okay, p) = Match(p, end, KIND_NAME);
  if (okay) {
    R(true, p);
  }
  R(false, pos);
  E;
}

Result ParsePreds(Iterator pos, Iterator end) {
  // Preds = PredString | PredString KIND_A Preds
  B;
  bool okay;
  std::tie(okay, pos) = ParsePredString(pos, end);
  if (!okay) {
    R(false, pos);
  }
  while (true) {
    Iterator p = pos;
    std::tie(okay, p) = Match(p, end, KIND_A);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParsePredString(p, end);
    if (!okay) {
      break;
    }
    pos = p;
  }
  R(true, pos);
  E;
}

Result ParsePredClaim(Iterator pos, Iterator end) {
  B;
  bool okay;
  Iterator p;
  // PredClaim = PredName KIND_BA Preds END
  p = pos;
  do {
    std::tie(okay, p) = ParsePredName(p, end);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = Match(p, end, KIND_BA);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParsePreds(p, end);
    if (okay && p == end) {
      R(true, p);
    }
  } while (0);
  // PredClaim = KIND_DA Preds END
  p = pos;
  do {
    std::tie(okay, p) = Match(p, end, KIND_DA);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParsePreds(p, end);
    if (okay && p == end) {
      R(true, p);
    }
  } while (0);
  R(false, pos);
  E;
}

Result ParseVerbPred(Iterator pos, Iterator end) {
  // VerbPred = MOD PredString
  B;
  bool okay;
  Iterator p = pos;
  do {
    std::tie(okay, p) = Match(p, end, KIND_MOD);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParsePredString(p, end);
    if (okay) {
      R(true, p);
    }
  } while (0);
  R(false, pos);
  E;
}

Result ParseStatement(Iterator pos, Iterator end) {
  B;
  bool okay;
  Iterator p;
  // Statement = PredName VerbPred [PredName] END
  p = pos;
  do {
    std::tie(okay, p) = ParsePredName(p, end);
    if (!okay) {
      break;
    }
    std::tie(okay, p) = ParseVerbPred(p, end);
    if (!okay) {
      break;
    }
    if (p == end) {
      R(true, p);
    }
    std::tie(okay, p) = ParsePredName(p, end);
    if (okay && p == end) {
      R(true, p);
    }
  } while (0);
  R(false, pos);
  E;
}

Result ParseSentence(Iterator pos, Iterator end) {
  B;
  bool okay;
  Iterator p;
  // Sentence = Statement | PredClaim
  std::tie(okay, p) = ParseStatement(pos, end);
  if (okay) {
    R(true, p);
  }
  std::tie(okay, p) = ParsePredClaim(pos, end);
  if (okay) {
    R(true, p);
  }
  R(false, pos);
  E;
}

int main() {
  for (std::string string; std::cin >> string && string != "#";) {
    if (string == ".") {  // Corner case.
      std::cout << "Bad!" << std::endl;
      continue;
    }
    Tokens tokens{Token(string)};
    while (std::cin >> string && string != ".") {
      if (string.back() == '.') {
        string.pop_back();
        tokens.push_back(Token(string));
        break;
      } else {
        tokens.push_back(Token(string));
      }
    }
    if (DEBUG) {
      std::cerr << "Tokens:";
      for (const auto& token : tokens) {
        std::cerr << " " << token;
      }
      std::cerr << std::endl;
    }
    LOG << "Parse:";
    bool okay = std::get<0>(ParseSentence(tokens.cbegin(), tokens.cend()));
    LOG << std::endl;
    if (okay) {
      std::cout << "Good" << std::endl;
    } else {
      std::cout << "Bad!" << std::endl;
    }
  }
  return 0;
}

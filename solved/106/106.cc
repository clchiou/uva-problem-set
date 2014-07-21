#include <algorithm>
#include <bitset>
#include <iostream>

enum { MAX_UPPER_LIMIT = 1000000, };

// Generate coprime pairs (base, coprime).
class CoprimeSequence {
 public:
  explicit CoprimeSequence(int base);

  bool Next();
  int base() const;
  int coprime() const;

 private:
  int base_;
  int coprime_;
};

class PrimitivePythagoreanTriples {
 public:
  explicit PrimitivePythagoreanTriples(int upper_limit);

  bool Next();
  int a() const;
  int b() const;
  int c() const;

 private:
  static int ComputeLeastValue(int x);
  bool NextBase();

  CoprimeSequence sequence_;
  int upper_limit_;
  // Use long long to avoid overflow.
  long long a_, b_, c_;
};

bool IsOddNumber(int n) { return n & 1; }
bool IsEvenNumber(int n) { return !IsOddNumber(n); }

void MinMax(long long* p, long long* q) {
  if (*p > *q) {
    std::swap(*p, *q);
  }
}

int ComputeGcd(int a, int b);

int main() {
  std::bitset<MAX_UPPER_LIMIT + 1> is_part_of_any_pythagorean_triple;
  for (int upper_limit = 0; std::cin >> upper_limit;) {
    int num_primitive_pythagorean_triples = 0;
    is_part_of_any_pythagorean_triple.reset();

    PrimitivePythagoreanTriples ppt(upper_limit);
    while (ppt.Next()) {
      num_primitive_pythagorean_triples++;
      int a = ppt.a(), b = ppt.b(), c = ppt.c();
      for (int k = 1; k * c <= upper_limit; k++) {
        is_part_of_any_pythagorean_triple.set(k * a);
        is_part_of_any_pythagorean_triple.set(k * b);
        is_part_of_any_pythagorean_triple.set(k * c);
      }
    }

    std::cout << num_primitive_pythagorean_triples << ' '
              << upper_limit - is_part_of_any_pythagorean_triple.count()
              << std::endl;
  }
  return 0;
}

CoprimeSequence::CoprimeSequence(int base)
    : base_(base), coprime_(IsEvenNumber(base) ? -1 : 0) {}

bool CoprimeSequence::Next() {
  do {
    coprime_ += 2;
  } while (ComputeGcd(base_, coprime_) > 1);
  return true;
}

int CoprimeSequence::base() const { return base_; }
int CoprimeSequence::coprime() const { return coprime_; }

// The least value of 'a' that Euclid's formula can generate.
int PrimitivePythagoreanTriples::ComputeLeastValue(int x) {
  return std::min({// Case m = x: Let n = 1 or 2.
                   x * x - 2 * 2, 2 * x * 1,
                   // Case n = x: Let m = n + 1.
                   (x + 1) * (x + 1) - x * x, 2 * (x + 1) * x, });
}

PrimitivePythagoreanTriples::PrimitivePythagoreanTriples(int upper_limit)
    : sequence_(2), upper_limit_(upper_limit), a_(0), b_(0), c_(0) {}

bool PrimitivePythagoreanTriples::Next() {
  if (ComputeLeastValue(sequence_.base()) > upper_limit_) {
    return false;
  }

  // Move to the next coprime pair.
  if (!sequence_.Next()) {
    return NextBase();
  }

  // Use Euclid's formula for generating (primitive) Pythagorean triples
  // such that a < b < c <= upper_limit.  Use long long to avoid overflow.
  long long n = sequence_.base(), m = sequence_.coprime();
  MinMax(&n, &m);
  long long a = m * m - n * n, b = 2 * m * n, c = m * m + n * n;
  MinMax(&a, &b);

  if (c > upper_limit_) {
    return NextBase();
  }

  a_ = a;
  b_ = b;
  c_ = c;
  return true;
}

// Move to the next base value.
bool PrimitivePythagoreanTriples::NextBase() {
  sequence_ = CoprimeSequence(sequence_.base() + 2);
  return Next();
}

int PrimitivePythagoreanTriples::a() const { return a_; }
int PrimitivePythagoreanTriples::b() const { return b_; }
int PrimitivePythagoreanTriples::c() const { return c_; }

// Use Euclidean algorithm for the greatest common divisior.
int ComputeGcd(int a, int b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

class PrimeSequence {
 public:
  PrimeSequence();

  void Begin();
  int Next();

 private:
  bool IsPrime(int n) const;

  std::vector<int> primes_;
  std::vector<int>::const_iterator pos_, end_;
};

typedef std::vector<std::pair<int, int>> Factors;

std::ostream& operator<<(std::ostream& out, const Factors& factors) {
  bool first = true;
  for (const auto prime_factor : factors) {
    if (!first) {
      out << " * ";
    }
    if (prime_factor.second > 1) {
      out << "pow(" << prime_factor.first << ", " << prime_factor.second << ")";
    } else {
      out << prime_factor.first;
    }
    first = false;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& array) {
  bool first = true;
  for (const auto element : array) {
    if (!first) {
      out << ", ";
    }
    out << element;
    first = false;
  }
  return out;
}

void PrimeFactorize(int n, Factors* prime_factors) {
  static PrimeSequence prime_sequence;
  prime_sequence.Begin();
  while (n > 1) {
    int p = prime_sequence.Next();
    if (p * p > n) {
      break;
    }
    int m = 0;
    while (n % p == 0) {
      n /= p;
      m++;
    }
    if (m > 0) {
      prime_factors->push_back(std::make_pair(p, m));
    }
  }
  if (n > 1) {
    prime_factors->push_back(std::make_pair(n, 1));
  }
}

int Power(int base, int exponent) {
  int result = 1;
  while (exponent) {
    if (exponent & 1) {
      result *= base;
    }
    base *= base;
    exponent >>= 1;
  }
  return result;
}

void ComputeFactors_Impl(const Factors::const_iterator pos,
                         const Factors::const_iterator end,
                         int factor,
                         std::vector<int>* factors) {
  if (pos == end) {
    factors->push_back(factor);
    return;
  }
  const Factors::const_iterator next_pos = pos + 1;
  for (int exponent = 0; exponent <= pos->second;
       exponent++, factor *= pos->first) {
    ComputeFactors_Impl(next_pos, end, factor, factors);
  }
}

void ComputeFactors(int n, std::vector<int>* factors) {
  Factors prime_factors;
  PrimeFactorize(n, &prime_factors);
  ComputeFactors_Impl(prime_factors.begin(), prime_factors.end(), 1, factors);
  std::sort(factors->begin(), factors->end());
  if (DEBUG) {
    std::cerr << "factors(" << n << ") =";
    for (const auto factor : *factors) {
      std::cerr << ' ' << factor;
    }
    std::cerr << std::endl;
  }
}

void ComputeCommonFactors(const std::vector<int>& numbers,
                          std::vector<int>* common_factors_out) {
  std::vector<int>* common_factors;
  std::vector<int>* common_factors_next;
  std::vector<int> factors, factors1, factors2;
  common_factors = &factors1;
  common_factors_next = &factors2;
  for (const auto n : numbers) {
    ComputeFactors(n, &factors);
    if (common_factors->empty()) {
      std::copy(
          factors.begin(), factors.end(), std::back_inserter(*common_factors));
    } else {
      common_factors_next->clear();
      std::set_intersection(factors.begin(),
                            factors.end(),
                            common_factors->begin(),
                            common_factors->end(),
                            std::back_inserter(*common_factors_next));
      std::swap(common_factors, common_factors_next);
    }
    factors.clear();
  }
  common_factors_out->clear();
  std::copy(common_factors->begin(),
            common_factors->end(),
            std::back_inserter(*common_factors_out));
}

void ComputePowers(int n, std::vector<std::pair<int, int>>* powers) {
  Factors prime_factors;
  PrimeFactorize(n, &prime_factors);
  if (DEBUG) {
    std::cerr << n << " = " << prime_factors << std::endl;
  }

  std::vector<int> exponents;
  for (const auto prime_factor : prime_factors) {
    exponents.push_back(prime_factor.second);
  }
  ComputeCommonFactors(exponents, &exponents);
  if (DEBUG) {
    std::cerr << "common_factors(...) = " << exponents << std::endl;
  }

  for (const auto exponent : exponents) {
    int base = 1;
    for (const auto prime_factor : prime_factors) {
      base *= Power(prime_factor.first, prime_factor.second / exponent);
    }
    powers->push_back(std::make_pair(base, exponent));
  }
}

void ComputeParameters(int height_initial_cat,
                       int number_worker_cats,
                       int* n_out,
                       int* m_out) {
  std::vector<std::pair<int, int>> n_powers;
  ComputePowers(number_worker_cats, &n_powers);
  if (DEBUG) {
    std::cerr << "powers(" << number_worker_cats << ") = ";
    bool first = true;
    for (const auto n_power : n_powers) {
      if (!first) {
        std::cerr << " or ";
      }
      std::cerr << "pow(" << n_power.first << ", " << n_power.second << ")";
      first = false;
    }
    std::cerr << std::endl;
  }

  int n = 0, m = 0;
  for (const auto n_power : n_powers) {
    // NOTE: (n + 1) ^ (m + 1) might overflow.
    // Because n >= 1, we have 2 * (n + 1) ^ m <= (n + 1) ^ (m + 1).
    // So, just test 2 * (n + 1) ^ m.
    int h = Power(n_power.first + 1, n_power.second);
    if (h <= height_initial_cat && height_initial_cat < 2 * h) {
      if (h > Power(n + 1, m)) {
        n = n_power.first;
        m = n_power.second;
      }
    }
  }
  if (DEBUG) {
    std::cerr << "n = " << n << ", m = " << m << std::endl;
  }
  *n_out = n;
  *m_out = m;
}

int main() {
  int height_initial_cat, number_worker_cats;
  while (std::cin >> height_initial_cat >> number_worker_cats &&
         height_initial_cat && number_worker_cats) {
    if (height_initial_cat == 1) {
      std::cout << "0 1" << std::endl;
      continue;
    }
    if (number_worker_cats == 1) {
      // n == 1
      int m = 0;
      for (int h = height_initial_cat; h != 1; h /= 2, m++)
        ;
      std::cout << m << ' ' << height_initial_cat * 2 - 1 << std::endl;
      continue;
    }
    // Let h = (n + 1) ^ m,
    //   h <= height_initial_cat < min{2 * h, h * (n + 1)},
    // and
    //   number_of_worker_cats = n ^ m
    int n, m;
    ComputeParameters(height_initial_cat, number_worker_cats, &n, &m);
    int number_nonworker_cats = 0;
    for (int i = 0, num_cats = 1; i <= m - 1; i++, num_cats *= n) {
      number_nonworker_cats += num_cats;
    }
    int total_hight_cats = 0;
    for (int i = 0, h = height_initial_cat, num_cats = 1; i <= m;
         i++, h /= n + 1, num_cats *= n) {
      h = std::max(1, h);
      total_hight_cats += h * num_cats;
    }
    std::cout << number_nonworker_cats << ' ' << total_hight_cats << std::endl;
  }
  return 0;
}

PrimeSequence::PrimeSequence() {
  primes_.push_back(2);
  primes_.push_back(3);
}

void PrimeSequence::Begin() {
  pos_ = primes_.begin();
  end_ = primes_.end();
}

int PrimeSequence::Next() {
  if (pos_ != end_) {
    return *pos_++;
  }
  int n = *primes_.rbegin();
  for (;;) {
    n += 2;
    if (IsPrime(n)) {
      break;
    }
  }
  primes_.push_back(n);
  return n;
}

bool PrimeSequence::IsPrime(int n) const {
  for (int p : primes_) {
    if (p * p > n)
      break;
    if (n % p == 0)
      return false;
  }
  return true;
}

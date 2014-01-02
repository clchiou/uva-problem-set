#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int MAX_NUM_CURRENCIES = 20;

class ConversionTable {
 public:
  static int NumCurrencies;

  friend std::istream& operator>>(std::istream& in, ConversionTable& table);
  friend std::ostream& operator<<(std::ostream& out,
                                  const ConversionTable& table);

  ConversionTable();
  ConversionTable(const ConversionTable& table);

  void Fill(double value);

  double at(int i, int j) const {
    return const_cast<ConversionTable*>(this)->at(i, j);
  }

  double& at(int i, int j) {
    assert(0 <= i && i < NumCurrencies && 0 <= j && j < NumCurrencies);
    return conversion_table_[i][j];
  }

 private:
  double conversion_table_[MAX_NUM_CURRENCIES][MAX_NUM_CURRENCIES];
};

struct Arbitrage {
  int num_transactions, start;
  int predecessors[MAX_NUM_CURRENCIES][MAX_NUM_CURRENCIES][MAX_NUM_CURRENCIES];
  ConversionTable output_table;
};

void ComputeArbitrage(const ConversionTable& table, Arbitrage* arb);

int currency_id(int index) { return index + 1; }

int main() {
  ConversionTable table;

  while (std::cin >> ConversionTable::NumCurrencies >> table) {
    Arbitrage arb;
    ComputeArbitrage(table, &arb);
    if (arb.num_transactions <= ConversionTable::NumCurrencies - 1) {
      int sequence[MAX_NUM_CURRENCIES];
      int currency = arb.start;
      for (int tx = arb.num_transactions - 1; tx >= 0; tx--) {
        currency = arb.predecessors[tx][arb.start][currency];
        sequence[tx] = currency;
      }
      std::cout << currency_id(arb.start);
      for (int tx = 0; tx < arb.num_transactions; tx++)
        std::cout << ' ' << currency_id(sequence[tx]);
      std::cout << ' ' << currency_id(arb.start) << std::endl;
    } else {
      std::cout << "no arbitrage sequence exists" << std::endl;
    }
  }

  return 0;
}

void Iterate(const ConversionTable& t0,
             const ConversionTable& table,
             Arbitrage* arb,
             ConversionTable* t1) {
  for (int i = 0; i < ConversionTable::NumCurrencies; i++) {
    for (int j = 0; j < ConversionTable::NumCurrencies; j++) {
      for (int k = 0; k < ConversionTable::NumCurrencies; k++) {
        double r = t0.at(i, k) * table.at(k, j);
        if (r > t1->at(i, j)) {
          t1->at(i, j) = r;
          arb->predecessors[arb->num_transactions][i][j] = k;
        }
      }
    }
  }
  arb->num_transactions++;
}

void ComputeArbitrage(const ConversionTable& table, Arbitrage* arb) {
  // Use modified Bellman-Ford algorithm on all vertices to compute sequence
  // of arbitrage.  Since exchange rates are positive, there will be no
  // negative cycles; so skip the check.
  ConversionTable t0(table), t1;
  arb->num_transactions = 0;
  arb->start = -1;
  while (arb->num_transactions <= ConversionTable::NumCurrencies - 1) {
    Iterate(t0, table, arb, &t1);
    for (arb->start = 0; arb->start < ConversionTable::NumCurrencies;
         arb->start++) {
      if (t1.at(arb->start, arb->start) > 1.01)
        break;
    }
    if (arb->start < ConversionTable::NumCurrencies)
      break;
    t0 = t1;
    t1.Fill(0.0);
  }
  arb->output_table = t1;
}

int ConversionTable::NumCurrencies;

std::istream& operator>>(std::istream& in, ConversionTable& table) {
  assert(ConversionTable::NumCurrencies > 0 &&
         ConversionTable::NumCurrencies <= MAX_NUM_CURRENCIES);
  for (int i = 0; i < ConversionTable::NumCurrencies; i++) {
    for (int j = 0; j < ConversionTable::NumCurrencies; j++) {
      if (i == j)
        table.conversion_table_[i][j] = 1.0;
      else
        in >> table.conversion_table_[i][j];
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const ConversionTable& table) {
  assert(ConversionTable::NumCurrencies > 0 &&
         ConversionTable::NumCurrencies <= MAX_NUM_CURRENCIES);
  for (int i = 0; i < ConversionTable::NumCurrencies; i++) {
    for (int j = 0; j < ConversionTable::NumCurrencies; j++) {
      if (j > 0)
        out << ' ';
      out << table.conversion_table_[i][j];
    }
    out << std::endl;
  }
  return out;
}

ConversionTable::ConversionTable() {
  for (int i = 0; i < MAX_NUM_CURRENCIES; i++) {
    std::fill(
        conversion_table_[i], conversion_table_[i] + MAX_NUM_CURRENCIES, 0.0);
  }
}

void ConversionTable::Fill(double value) {
  assert(0 < NumCurrencies && NumCurrencies <= MAX_NUM_CURRENCIES);
  for (int i = 0; i < NumCurrencies; i++) {
    std::fill(
        conversion_table_[i], conversion_table_[i] + NumCurrencies, value);
  }
}

ConversionTable::ConversionTable(const ConversionTable& table) {
  assert(0 < NumCurrencies && NumCurrencies <= MAX_NUM_CURRENCIES);
  for (int i = 0; i < NumCurrencies; i++) {
    std::copy(table.conversion_table_[i],
              table.conversion_table_[i] + NumCurrencies,
              conversion_table_[i]);
  }
}

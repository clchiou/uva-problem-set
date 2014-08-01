#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

struct Card {
  static const Card ZERO;

  Card();
  explicit operator bool() const;

  char suit, face;
};

std::istream& operator>>(std::istream& in, Card& card);
std::ostream& operator<<(std::ostream& out, const Card& card);

bool operator==(const Card& lhs, const Card& rhs);
bool operator!=(const Card& lhs, const Card& rhs);

const Card Card::ZERO;

int main() {
  for (Card deck[52]; std::cin >> deck[0] && deck[0];) {
    for (int i = 1; i < 52; i++) {
      std::cin >> deck[i];
    }
    if (DEBUG) {
      std::cerr << "DECK:";
      for (int i = 0; i < 52; i++) {
        std::cerr << ' ' << deck[i];
      }
      std::cerr << std::endl;
    }

    std::vector<Card> piles[52];
    std::vector<Card>* last = piles;
    for (const auto& new_card : deck) {
      assert(last != piles + 52 + 1);
      if (DEBUG) {
        for (auto i = piles; i != last; ++i) {
          std::cerr << " (" << std::setw(2) << std::right << i->size() << ")"
                    << i->back();
        }
        std::cerr << " | " << new_card << std::endl;
      }

      (last++)->push_back(new_card);
      bool has_been_moved;
      do {
        has_been_moved = false;
        for (auto i = piles; i != last; ++i) {
          assert(!i->empty());
          for (auto j : {i - 3, i - 1}) {
            if (j < piles) {
              continue;
            }
            assert(!j->empty());
            const Card& card_i = i->back();
            const Card& card_j = j->back();
            if (card_i.suit == card_j.suit || card_i.face == card_j.face) {
              j->push_back(card_i);
              i->pop_back();
              if (i->empty()) {
                for (auto k = i; k + 1 != last; ++k) {
                  *k = *(k + 1);
                }
                (--last)->clear();
              }
              has_been_moved = true;
              break;
            }
          }
          if (has_been_moved) {
            break;
          }
        }
      } while (has_been_moved);
    }

    int num_piles = last - piles;
    assert(num_piles > 0);

    std::cout << num_piles << " pile" << (num_piles == 1 ? "" : "s")
              << " remaining:";
    for (auto i = piles; i != last; ++i) {
      std::cout << ' ' << i->size();
    }
    std::cout << std::endl;
  }
  return 0;
}

Card::Card() : suit('\0'), face('\0') {}

std::istream& operator>>(std::istream& in, Card& card) {
  std::string str;
  if (!(in >> str) || str == "#") {
    card = Card::ZERO;
    return in;
  }
  assert(str.size() == 2);
  card.face = str[0];
  card.suit = str[1];
  return in;
}

std::ostream& operator<<(std::ostream& out, const Card& card) {
  return out << card.face << card.suit;
}

bool operator==(const Card& lhs, const Card& rhs) {
  return lhs.suit == rhs.suit && lhs.face == rhs.face;
}

bool operator!=(const Card& lhs, const Card& rhs) { return !(lhs == rhs); }

Card::operator bool() const { return *this != ZERO; }

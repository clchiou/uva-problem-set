#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

class Card {
  friend std::istream& operator>>(std::istream& in, Card& card);
  friend std::ostream& operator<<(std::ostream& out, const Card& card);

 public:
  enum Suit {
    NONE,
    CLUB,
    DIAMOND,
    HEART,
    SPADE
  };

  Card() : Card(NONE, 0) {};
  Card(Suit suit, int face) : suit_(suit), face_(face) {}

  Suit suit() const {
    assert(suit_ != NONE);
    return suit_;
  }

  int face() const {
    assert(face_ != 0);
    return face_;
  }

  bool operator<(const Card& other) const {
    assert(face_ != 0 && other.face_ != 0);
    return face_ < other.face_;
  }

 private:
  Suit suit_;
  int face_;
};

struct HandPattern {
  // NOTE: Hand is assumed to be sorted.
  typedef bool (*MatchingFunc)(const Card hand[5]);
  std::string hand_name;
  MatchingFunc match;
};

void CountDuplicates(const Card hand[5],
                     std::pair<int, int> duplicates[5],
                     int* num_duplicates) {
  *num_duplicates = 0;
  for (int i = 0; i < 5; i++) {
    int j;
    for (j = 0; j < *num_duplicates; j++)
      if (duplicates[j].first == hand[i].face()) {
        duplicates[j].second++;
        break;
      }
    if (j >= *num_duplicates) {
      assert(*num_duplicates < 5);
      duplicates[*num_duplicates].first = hand[i].face();
      duplicates[*num_duplicates].second = 1;
      (*num_duplicates)++;
    }
  }
}

bool IsHighestCard(const Card[5]) {
  return true;
}

bool IsOnePair(const Card hand[5]) {
  // One of W W X Y Z, W X X Y Z, W X Y Y Z, or W X Y Z Z.
  std::pair<int, int> duplicates[5];
  int num_duplicates;
  CountDuplicates(hand, duplicates, &num_duplicates);
  return num_duplicates == 4;
}

bool IsTwoPairs(const Card hand[5]) {
  // One of X X Y Y Z, X X Y Z Z, or X Y Y Z Z.
  std::pair<int, int> duplicates[5];
  int num_duplicates;
  CountDuplicates(hand, duplicates, &num_duplicates);
  if (num_duplicates != 3)
    return false;
  return duplicates[0].second == 2 ||
         duplicates[1].second == 2 ||
         duplicates[2].second == 2;
}

bool IsThreeOfAKind(const Card hand[5]) {
  // One of X X X Y Z, X Y Y Y Z, or X Y Z Z Z.
  std::pair<int, int> duplicates[5];
  int num_duplicates;
  CountDuplicates(hand, duplicates, &num_duplicates);
  if (num_duplicates != 3)
    return false;
  return duplicates[0].second == 3 ||
         duplicates[1].second == 3 ||
         duplicates[2].second == 3;
}

bool IsFourOfAKind(const Card hand[5]) {
  // Either X Y Y Y Y or X X X X Y.
  std::pair<int, int> duplicates[5];
  int num_duplicates;
  CountDuplicates(hand, duplicates, &num_duplicates);
  if (num_duplicates != 2)
    return false;
  return duplicates[0].second == 4 || duplicates[1].second == 4;
}

bool IsFullHouse(const Card hand[5]) {
  // Either X X Y Y Y or X X X Y Y.
  std::pair<int, int> duplicates[5];
  int num_duplicates;
  CountDuplicates(hand, duplicates, &num_duplicates);
  if (num_duplicates != 2)
    return false;
  return duplicates[0].second == 3 || duplicates[1].second == 3;
}

bool IsFlush(const Card hand[5]) {
  for (int i = 1; i < 5; i++)
    if (hand[0].suit() != hand[i].suit())
      return false;
  return true;
}

bool IsStraight(const Card hand[5]) {
  // Special case of Ace.
  if (hand[0].face() == 1 &&
      hand[1].face() == 10 &&
      hand[2].face() == 11 &&
      hand[3].face() == 12 &&
      hand[4].face() == 13)
    return true;
  for (int i = 1; i < 5; i++)
    if (hand[i-1].face() + 1 != hand[i].face())
      return false;
  return true;
}

bool IsStraightFlush(const Card hand[5]) {
  return IsFlush(hand) && IsStraight(hand);
}

void DiscardAndDraw(const Card hand[5],
                    const Card deck[5],
                    int discard_bits,
                    Card new_hand[5]) {
  for (int i = 0, j = 0; i < 5; i++) {
    if (discard_bits & (1 << i))
      new_hand[i] = deck[j++];
    else
      new_hand[i] = hand[i];
  }
  std::sort(new_hand, new_hand + 5);
}

void ComputeBestHand(const Card hand[5],
                     const Card deck[5],
                     std::string* best_hand_name) {
  // NOTE: This is ordered by hand value.
  const HandPattern hand_patterns[] = {
    {"straight-flush", IsStraightFlush},
    {"four-of-a-kind", IsFourOfAKind},
    {"full-house", IsFullHouse},
    {"flush", IsFlush},
    {"straight", IsStraight},
    {"three-of-a-kind", IsThreeOfAKind},
    {"two-pairs", IsTwoPairs},
    {"one-pair", IsOnePair},
    {"highest-card", IsHighestCard},
  };

  int best_hand = sizeof(hand_patterns) / sizeof(hand_patterns[0]);
  for (int discard_bits = 0; discard_bits < (1 << 5); discard_bits++) {
    Card new_hand[5];
    DiscardAndDraw(hand, deck, discard_bits, new_hand);
    for (int hand = 0; hand < best_hand; hand++)
      if (hand_patterns[hand].match(new_hand)) {
        best_hand = hand;
        break;
      }
    if (best_hand == 0)  // We can't be any better.
      break;
  }

  *best_hand_name = hand_patterns[best_hand].hand_name;
}

int main() {
  for (Card hand[5], deck[5];
       std::cin >> hand[0] >> hand[1] >> hand[2] >> hand[3] >> hand[4]
                >> deck[0] >> deck[1] >> deck[2] >> deck[3] >> deck[4];) {
    std::string best_hand;
    ComputeBestHand(hand, deck, &best_hand);

    std::cout << "Hand: ";
    for (const auto& card : hand)
      std::cout << card << ' ';
    std::cout << "Deck: ";
    for (const auto& card : deck)
      std::cout << card << ' ';
    std::cout << "Best hand: " << best_hand << std::endl;
  }
  return 0;
}

std::istream& operator>>(std::istream& in, Card& card) {
  std::string card_str;
  if (in >> card_str) {
    assert(card_str.size() == 2);
    Card::Suit suit;
    int face;
    if (card_str[0] == 'A') {
      face = 1;
    } else if (card_str[0] == 'T') {
      face = 10;
    } else if (card_str[0] == 'J') {
      face = 11;
    } else if (card_str[0] == 'Q') {
      face = 12;
    } else if (card_str[0] == 'K') {
      face = 13;
    } else if ('2' <= card_str[0] && card_str[0] <= '9') {
      face = card_str[0] - '0';
    } else {
      assert(false);
    }
    if (card_str[1] == 'C') {
      suit = Card::CLUB;
    } else if (card_str[1] == 'D') {
      suit = Card::DIAMOND;
    } else if (card_str[1] == 'H') {
      suit = Card::HEART;
    } else if (card_str[1] == 'S') {
      suit = Card::SPADE;
    } else {
      assert(false);
    }
    card.suit_ = suit;
    card.face_ = face;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Card& card) {
  if (card.face() == 1) {
    out << 'A';
  } else if (card.face() == 10) {
    out << 'T';
  } else if (card.face() == 11) {
    out << 'J';
  } else if (card.face() == 12) {
    out << 'Q';
  } else if (card.face() == 13) {
    out << 'K';
  } else if (2 <= card.face() && card.face() <= 9) {
    out << card.face();
  } else {
    assert(false);
  }
  if (card.suit() == Card::CLUB) {
    out << 'C';
  } else if (card.suit() == Card::DIAMOND) {
    out << 'D';
  } else if (card.suit() == Card::HEART) {
    out << 'H';
  } else if (card.suit() == Card::SPADE) {
    out << 'S';
  } else {
    assert(false);
  }
  return out;
}

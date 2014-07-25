#include <algorithm>
#include <cassert>
#include <iostream>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif  // ONLINE_JUDGE
};

enum { MAX_NUM_EVENTS = 20, };

class Rankings {
 public:
  explicit Rankings(int num_events);

  void SetRanking(int event, int ranking);

  bool IsBefore(int event1, int event2) const;

  int num_events() const;
  int to_event(int ranking) const;

 private:
  int rankings_[MAX_NUM_EVENTS];
  int num_events_;
};

std::istream& operator>>(std::istream& in, Rankings& rankings);
std::ostream& operator<<(std::ostream& out, const Rankings& rankings);

bool ReadArray(int* begin, int* end, std::istream& in);
void PrintArray(const int* begin, const int* end, std::ostream& out);

int main() {
  int num_events;
  std::cin >> num_events;

  Rankings correct_rankings(num_events);
  std::cin >> correct_rankings;
  if (DEBUG) {
    std::cerr << correct_rankings;
  }

  int rankings[MAX_NUM_EVENTS];
  int events[MAX_NUM_EVENTS];
  int max_scores[MAX_NUM_EVENTS];
  while (ReadArray(rankings, rankings + num_events, std::cin)) {
    for (int event = 1; event <= num_events; event++) {
      events[rankings[event - 1] - 1] = event;
    }
    if (DEBUG) {
      PrintArray(events, events + num_events, std::cerr);
    }

    for (int i = 0; i < num_events; i++) {
      max_scores[i] = 1;
    }
    for (int i = num_events - 2; i >= 0; i--) {
      for (int j = i + 1; j < num_events; j++) {
        if (correct_rankings.IsBefore(events[i], events[j])) {
          max_scores[i] = std::max(max_scores[i], max_scores[j] + 1);
        }
      }
    }
    PrintArray(max_scores, max_scores + num_events, std::cerr);

    std::cout << *std::max_element(max_scores, max_scores + num_events)
              << std::endl;
  }

  return 0;
}

std::istream& operator>>(std::istream& in, Rankings& rankings) {
  for (int event = 1; event <= rankings.num_events(); event++) {
    int ranking;
    in >> ranking;
    rankings.SetRanking(event, ranking);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Rankings& rankings) {
  out << rankings.num_events() << std::endl;
  for (int ranking = 1; ranking <= rankings.num_events(); ranking++) {
    if (ranking > 1) {
      out << ' ';
    }
    out << rankings.to_event(ranking);
  }
  out << std::endl;
  return out;
}

bool ReadArray(int* begin, int* end, std::istream& in) {
  while (begin != end) {
    if (!(in >> *begin)) {
      return false;
    }
    ++begin;
  }
  return true;
}

void PrintArray(const int* begin, const int* end, std::ostream& out) {
  for (const int* i = begin; i != end; ++i) {
    if (i != begin) {
      out << ' ';
    }
    out << *i;
  }
  out << std::endl;
}

Rankings::Rankings(int num_events) : num_events_(num_events) {
  for (int event = 1; event <= num_events_; event++) {
    rankings_[event - 1] = 0;
  }
}

void Rankings::SetRanking(int event, int ranking) {
  assert(1 <= event && event <= num_events_);
  assert(1 <= ranking && ranking <= num_events_);
  rankings_[event - 1] = ranking;
}

bool Rankings::IsBefore(int event1, int event2) const {
  assert(1 <= event1 && event1 <= num_events_);
  assert(1 <= event2 && event2 <= num_events_);
  return rankings_[event1 - 1] < rankings_[event2 - 1];
}

int Rankings::num_events() const { return num_events_; }

int Rankings::to_event(int ranking) const {
  assert(1 <= ranking && ranking <= num_events_);
  for (int event = 1; event <= num_events_; event++) {
    if (rankings_[event - 1] == ranking) {
      return event;
    }
  }
  return 0;
}

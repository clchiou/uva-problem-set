#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

#define LOG  \
  if (DEBUG) \
  std::cerr

enum RateType { NONE = -1, DAY, EVENING, NIGHT, NUM_RATE_TYPES };

struct RatePeriodEntry {
  RateType type;
  int start_timestamp;
  int end_timestamp;
};

struct ChargingScheduleEntry {
  char step;
  double rates[NUM_RATE_TYPES];
};

int Timestamp(int hour, int minute) { return hour * 60 + minute; }

// Intersection of right-exclusive ranges.
bool Intersect(int p_left,
               int p_right,
               int q_left,
               int q_right,
               int* r_left,
               int* r_right) {
  int left = std::max(p_left, q_left);
  int right = std::min(p_right, q_right);
  if (left >= right) {
    return false;
  }
  *r_left = left;
  *r_right = right;
  return true;
}

int main() {
  const RateType rate_types[] = {DAY, EVENING, NIGHT};

  const RatePeriodEntry rate_periods[] = {
    {NIGHT,   Timestamp( 0, 0), Timestamp( 8, 0)},
    {DAY,     Timestamp( 8, 0), Timestamp(18, 0)},
    {EVENING, Timestamp(18, 0), Timestamp(22, 0)},
    {NIGHT,   Timestamp(22, 0), Timestamp(32, 0)},
    {DAY,     Timestamp(32, 0), Timestamp(42, 0)},
    {EVENING, Timestamp(42, 0), Timestamp(46, 0)},
    {NIGHT,   Timestamp(46, 0), Timestamp(56, 0)},
  };

  const ChargingScheduleEntry schedules[] = {
    {'A', {0.10, 0.06, 0.02}},
    {'B', {0.25, 0.15, 0.05}},
    {'C', {0.53, 0.33, 0.13}},
    {'D', {0.87, 0.47, 0.17}},
    {'E', {1.44, 0.80, 0.30}},
  };

  for (std::string line; std::getline(std::cin, line) && line != "#";) {
    char step;
    std::string phone_number;
    int start_timestamp, end_timestamp;
    {
      std::string step_str;
      int start_hour, start_minute, end_hour, end_minute;
      std::stringstream(line) >> step_str >> phone_number >> start_hour >>
          start_minute >> end_hour >> end_minute;
      assert(step_str.length() == 1);
      step = step_str.front();
      start_timestamp = Timestamp(start_hour, start_minute);
      end_timestamp = Timestamp(end_hour, end_minute);
      if (end_timestamp <= start_timestamp) {
        end_timestamp += Timestamp(24, 0);
      }
      LOG << "step=" << step << std::endl;
      LOG << "phone_number=" << phone_number << std::endl;
      LOG << "start=" << start_timestamp / 60 << ':' << start_timestamp % 60
          << std::endl;
      LOG << "end=" << end_timestamp / 60 << ':' << end_timestamp % 60
          << std::endl;
    }

    const ChargingScheduleEntry* schedule = nullptr;
    for (const auto& sched : schedules) {
      if (sched.step == step) {
        schedule = &sched;
        break;
      }
    }
    assert(schedule != nullptr);

    int minutes[NUM_RATE_TYPES];
    ::memset(minutes, 0, sizeof(minutes));
    for (const auto& rate_period : rate_periods) {
      int left, right;
      if (Intersect(rate_period.start_timestamp,
                    rate_period.end_timestamp,
                    start_timestamp,
                    end_timestamp,
                    &left,
                    &right)) {
        minutes[rate_period.type] += right - left;
      }
    }

    double cost = 0;
    for (auto rate_type : rate_types) {
      cost += minutes[rate_type] * schedule->rates[rate_type];
    }

    ::printf("%10s%6d%6d%6d%3c%8.2f\n",
             phone_number.c_str(),
             minutes[DAY],
             minutes[EVENING],
             minutes[NIGHT],
             step,
             cost);
  }

  return 0;
}

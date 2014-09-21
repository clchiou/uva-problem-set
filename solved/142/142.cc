#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

enum {
  MIN_COORDINATE = 0,
  MAX_COORDINATE = 499,
  COORDINATE_RANGE = MAX_COORDINATE - MIN_COORDINATE + 1,
};

int ToIndex(int coordinate) { return coordinate - MIN_COORDINATE; }

int ComputeDistance(int x1, int y1, int x2, int y2) {
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int main() {
  char regions[COORDINATE_RANGE][COORDINATE_RANGE];
  char region = 'A';
  memset(regions, 0, sizeof(regions));

  std::vector<std::pair<int, int>> icons;      // (x, y)
  std::vector<std::pair<int, int>> distances;  // (index, distance)
  auto less_than = [](const std::pair<int, int>& d1,
                      const std::pair<int, int>& d2) {
    return d1.second < d2.second ||
           (d1.second == d2.second && d1.first < d2.first);
  };

  std::string type;
  while (std::cin >> type && type != "#") {
    if (type == "I") {
      int x, y;
      std::cin >> x >> y;
      icons.push_back(std::make_pair(x, y));
    } else if (type == "R") {
      int x1, y1, x2, y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
          regions[ToIndex(x)][ToIndex(y)] = region;
        }
      }
      region++;
    } else if (type == "M") {
      int mouse_x, mouse_y;
      std::cin >> mouse_x >> mouse_y;
      if (regions[ToIndex(mouse_x)][ToIndex(mouse_y)] != '\0') {
        std::cout << regions[ToIndex(mouse_x)][ToIndex(mouse_y)] << std::endl;
      } else {
        distances.clear();
        for (auto i = icons.cbegin(); i != icons.cend(); ++i) {
          int x = i->first;
          int y = i->second;
          if (regions[ToIndex(x)][ToIndex(y)] == '\0') {
            int index = i - icons.cbegin();
            int distance = ComputeDistance(mouse_x, mouse_y, x, y);
            distances.push_back(std::make_pair(index, distance));
          }
        }
        assert(!distances.empty());  // At least one visible icon.
        std::sort(distances.begin(), distances.end(), less_than);
        int distance = distances.front().second;
        for (auto i = distances.cbegin(); i != distances.cend(); ++i) {
          if (i->second != distance) {
            break;
          }
          char buffer[8];
          snprintf(buffer, sizeof(buffer), "%3d", i->first + 1);
          std::cout << buffer;
        }
        std::cout << std::endl;
      }
    } else {
      assert(false);
    }
  }
  return 0;
}

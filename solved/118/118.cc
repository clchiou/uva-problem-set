#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int SIZE = 50 + 1;

enum Orientation {
  NORTH,
  EAST,
  SOUTH,
  WEST
};

std::istream& operator>>(std::istream& in, Orientation& orientation);
std::ostream& operator<<(std::ostream& out, Orientation orientation);

Orientation ParseOrientation(std::string orientation);

enum Instruction {
  LEFT,
  RIGHT,
  FORWARD
};

Instruction ParseInstruction(char instruction);

struct Point {
  int x, y;
  friend std::istream& operator>>(std::istream& in, Point& point);
};

class RobotPosition {
 public:
  friend std::istream& operator>>(std::istream& in, RobotPosition& pos);
  friend std::ostream& operator<<(std::ostream& out, const RobotPosition& pos);

  RobotPosition() : x_(0), y_(0), orientation_(NORTH) {}
  RobotPosition(int x, int y, Orientation orientation)
      : x_(x), y_(y), orientation_(orientation) {}

  RobotPosition TurnLeft() const;
  RobotPosition TurnRight() const;
  RobotPosition MoveForward() const;

  bool IsInside(const Point& point) const;

  int x() const { return x_; }
  int y() const { return y_; }

 private:
  int x_, y_;
  Orientation orientation_;
};

int main() {
  Point max_coord;
  std::cin >> max_coord;

  bool scent[SIZE][SIZE];
  for (int y = 0; y <= max_coord.y; y++)
    std::fill(scent[y], scent[y] + max_coord.x + 1, false);

  for (RobotPosition pos; std::cin >> pos;) {
    std::string instructions;
    std::getline(std::cin, instructions);
    bool lost = false;
    for (std::string::const_iterator i = instructions.begin();
         !lost && i != instructions.end();
         ++i) {
      RobotPosition new_pos;
      switch (ParseInstruction(*i)) {
        case LEFT:
          pos = pos.TurnLeft();
          break;
        case RIGHT:
          pos = pos.TurnRight();
          break;
        case FORWARD:
          new_pos = pos.MoveForward();
          if (new_pos.IsInside(max_coord)) {
            pos = new_pos;
          } else if (!scent[pos.x()][pos.y()]) {
            scent[pos.x()][pos.y()] = true;
            lost = true;
          }
          break;
      }
    }
    std::cout << pos;
    if (lost)
      std::cout << " LOST";
    std::cout << std::endl;
  }

  return 0;
}

std::istream& operator>>(std::istream& in, Orientation& orientation) {
  std::string orientation_str;
  if (in >> orientation_str) {
    orientation = ParseOrientation(orientation_str);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, Orientation orientation) {
  switch (orientation) {
    case NORTH:
      out << 'N';
      break;
    case EAST:
      out << 'E';
      break;
    case SOUTH:
      out << 'S';
      break;
    case WEST:
      out << 'W';
      break;
  }
  return out;
}

Orientation ParseOrientation(std::string orientation) {
  assert(orientation.size() == 1);
  if (orientation == "N")
    return NORTH;
  if (orientation == "E")
    return EAST;
  if (orientation == "S")
    return SOUTH;
  assert(orientation == "W");
  return WEST;
}

Instruction ParseInstruction(char instruction) {
  if (instruction == 'L')
    return LEFT;
  if (instruction == 'R')
    return RIGHT;
  assert(instruction == 'F');
  return FORWARD;
}

std::istream& operator>>(std::istream& in, Point& point) {
  std::string line;
  std::getline(in, line);
  std::stringstream(line) >> point.x >> point.y;
  return in;
}

std::istream& operator>>(std::istream& in, RobotPosition& pos) {
  std::string line;
  if (std::getline(in, line)) {
    std::stringstream(line) >> pos.x_ >> pos.y_ >> pos.orientation_;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const RobotPosition& pos) {
  out << pos.x_ << ' ' << pos.y_ << ' ' << pos.orientation_;
  return out;
}

RobotPosition RobotPosition::TurnLeft() const {
  switch (orientation_) {
    case NORTH:
      return RobotPosition(x_, y_, WEST);
    case EAST:
      return RobotPosition(x_, y_, NORTH);
    case SOUTH:
      return RobotPosition(x_, y_, EAST);
    case WEST:
      return RobotPosition(x_, y_, SOUTH);
  }
  assert(false);
  return RobotPosition();
}

RobotPosition RobotPosition::TurnRight() const {
  switch (orientation_) {
    case NORTH:
      return RobotPosition(x_, y_, EAST);
    case EAST:
      return RobotPosition(x_, y_, SOUTH);
    case SOUTH:
      return RobotPosition(x_, y_, WEST);
    case WEST:
      return RobotPosition(x_, y_, NORTH);
  }
  assert(false);
  return RobotPosition();
}

RobotPosition RobotPosition::MoveForward() const {
  int dx = 0, dy = 0;
  switch (orientation_) {
    case NORTH:
      dx = 0;
      dy = 1;
      break;
    case EAST:
      dx = 1;
      dy = 0;
      break;
    case SOUTH:
      dx = 0;
      dy = -1;
      break;
    case WEST:
      dx = -1;
      dy = 0;
      break;
  }
  assert(dx || dy);
  return RobotPosition(x_ + dx, y_ + dy, orientation_);
}

bool RobotPosition::IsInside(const Point& point) const {
  return 0 <= x_ && x_ <= point.x && 0 <= y_ && y_ <= point.y;
}

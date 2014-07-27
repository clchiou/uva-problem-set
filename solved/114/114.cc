#include <cassert>
#include <iostream>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif
};

enum { MAX_GRID_SIZE = 51, };

struct Bumper {
  int x, y, value, cost;
};

struct PingballMachine {
  bool IsHittingWall(int x, int y) const;
  bool IsHittingBumper(int x, int y, int* value, int* cost) const;

  int width, height, num_bumpers, cost_wall;
  Bumper bumpers[MAX_GRID_SIZE][MAX_GRID_SIZE];
};

enum Direction {
  DIRECTION_NONE = -1,
  DIRECTION_RIGHT,
  DIRECTION_UP,
  DIRECTION_LEFT,
  DIRECTION_DOWN,
};

bool operator<(const Bumper& p, const Bumper& q);
bool operator==(const Bumper& p, const Bumper& q);

std::istream& operator>>(std::istream& in, Bumper& b);
std::istream& operator>>(std::istream& in, PingballMachine& m);
std::istream& operator>>(std::istream& in, Direction& dir);

std::ostream& operator<<(std::ostream& out, const Bumper& b);
std::ostream& operator<<(std::ostream& out, const PingballMachine& m);
std::ostream& operator<<(std::ostream& out, Direction dir);

void Move(int x, int y, Direction dir, int* next_x, int* next_y);
Direction TurnRight(Direction dir);

int main() {
  PingballMachine machine;
  std::cin >> machine;
  if (DEBUG) {
    std::cerr << machine;
  }

  int x, y, lifetime, total_points = 0;
  Direction dir;
  while (std::cin >> x >> y >> dir >> lifetime) {
    if (DEBUG) {
      std::cerr << "Ball: x=" << x << " y=" << y << " dir=" << dir
                << " lifetime=" << lifetime << std::endl;
    }

    int points = 0;
    while (lifetime > 0) {
      if (DEBUG) {
        std::cerr << " x=" << x << " y=" << y << " dir=" << dir
                  << " lifetime=" << lifetime << " points=" << points
                  << std::endl;
      }
      int next_x, next_y, value, cost;
      Move(x, y, dir, &next_x, &next_y);
      lifetime--;
      if (lifetime <= 0) {
        break;
      }
      // Could a bumper on the edge (i.e. wall)?
      if (machine.IsHittingBumper(next_x, next_y, &value, &cost)) {
        if (DEBUG) {
          std::cerr << " Hit: Bumper value=" << value << " cost=" << cost
                    << std::endl;
        }
        dir = TurnRight(dir);
        points += value;
        lifetime -= cost;
      } else if (machine.IsHittingWall(next_x, next_y)) {
        if (DEBUG) {
          std::cerr << " Hit: Wall" << std::endl;
        }
        dir = TurnRight(dir);
        lifetime -= machine.cost_wall;
      } else {
        x = next_x;
        y = next_y;
      }
    }

    total_points += points;
    std::cout << points << std::endl;
  }
  std::cout << total_points << std::endl;

  return 0;
}

bool PingballMachine::IsHittingWall(int x, int y) const {
  return x == 1 || x == width || y == 1 || y == height;
}

bool PingballMachine::IsHittingBumper(int x,
                                      int y,
                                      int* value,
                                      int* cost) const {
  const Bumper& b = bumpers[x - 1][y - 1];
  if (b.x == -1 || b.y == -1) {
    return false;
  }
  *value = b.value;
  *cost = b.cost;
  return true;
}

bool operator<(const Bumper& p, const Bumper& q) {
  if (p.x != q.x) {
    return p.x < q.x;
  } else {
    return p.y < q.y;
  }
}

bool operator==(const Bumper& p, const Bumper& q) {
  return p.x == q.x && p.y == q.y;
}

std::istream& operator>>(std::istream& in, Bumper& b) {
  return in >> b.x >> b.y >> b.value >> b.cost;
}

std::istream& operator>>(std::istream& in, PingballMachine& m) {
  if (!(in >> m.width >> m.height >> m.cost_wall >> m.num_bumpers)) {
    m.width = m.height = m.cost_wall = m.num_bumpers = 0;
    return in;
  }
  for (int i = 0; i < m.width; i++) {
    for (int j = 0; j < m.height; j++) {
      m.bumpers[i][j].x = -1;
      m.bumpers[i][j].y = -1;
    }
  }
  for (int i = 0; i < m.num_bumpers; i++) {
    Bumper b;
    if (!(in >> b)) {
      m.width = m.height = m.cost_wall = m.num_bumpers = 0;
      return in;
    }
    m.bumpers[b.x - 1][b.y - 1] = b;
  }
  return in;
}

std::istream& operator>>(std::istream& in, Direction& dir) {
  int d;
  if (!(in >> d)) {
    return in;
  }
  switch (d) {
    case 0:
      dir = DIRECTION_RIGHT;
      break;
    case 1:
      dir = DIRECTION_UP;
      break;
    case 2:
      dir = DIRECTION_LEFT;
      break;
    case 3:
      dir = DIRECTION_DOWN;
      break;
    default:
      assert(false);
      break;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Bumper& b) {
  return out << "Bumper: x=" << b.x << " y=" << b.y << " value=" << b.value
             << " cost=" << b.cost;
}

std::ostream& operator<<(std::ostream& out, const PingballMachine& m) {
  out << "PingballMachine: width=" << m.width << " height=" << m.height
      << " cost_wall=" << m.cost_wall << std::endl;
  for (int i = 0; i < m.width; i++) {
    for (int j = 0; j < m.height; j++) {
      const Bumper& b = m.bumpers[i][j];
      if (b.x != -1 && b.y != -1) {
        out << b << std::endl;
      }
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, Direction dir) {
  switch (dir) {
    case DIRECTION_RIGHT:
      out << 'R';
      break;
    case DIRECTION_UP:
      out << 'U';
      break;
    case DIRECTION_LEFT:
      out << 'L';
      break;
    case DIRECTION_DOWN:
      out << 'D';
      break;
    default:
      assert(false);
      break;
  }
  return out;
}

void Move(int x, int y, Direction dir, int* next_x, int* next_y) {
  switch (dir) {
    case DIRECTION_RIGHT:
      *next_x = x + 1;
      *next_y = y;
      break;
    case DIRECTION_UP:
      *next_x = x;
      *next_y = y + 1;
      break;
    case DIRECTION_LEFT:
      *next_x = x - 1;
      *next_y = y;
      break;
    case DIRECTION_DOWN:
      *next_x = x;
      *next_y = y - 1;
      break;
    default:
      assert(false);
      break;
  }
}

Direction TurnRight(Direction dir) {
  switch (dir) {
    case DIRECTION_RIGHT:
      return DIRECTION_DOWN;
    case DIRECTION_UP:
      return DIRECTION_RIGHT;
    case DIRECTION_LEFT:
      return DIRECTION_UP;
    case DIRECTION_DOWN:
      return DIRECTION_LEFT;
    default:
      assert(false);
      return DIRECTION_NONE;
  }
}

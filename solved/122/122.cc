#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <vector>

enum { MAX_NODES = 256, };

struct Node {
  std::string path;
  int value;
};

struct Tree {
  typedef std::vector<Node>::const_iterator const_iterator;

  bool IsComplete() const;
  const_iterator Find(const_iterator first,
                      const_iterator end,
                      const std::string& path) const;

  std::vector<Node> nodes;
};

bool operator<(const Node& p, const Node& q);
std::istream& operator>>(std::istream& in, Tree& tree);
std::ostream& operator<<(std::ostream& out, const Tree& tree);

int main() {
  Tree tree;
  while (std::cin >> tree) {
    if (tree.IsComplete()) {
      std::cout << tree << std::endl;
    } else {
      std::cout << "not complete" << std::endl;
    }
  }
  return 0;
}

bool operator<(const Node& p, const Node& q) {
  if (p.path.size() != q.path.size()) {
    return p.path.size() < q.path.size();
  }
  for (int i = 0; i < (int)p.path.size(); i++) {
    if (p.path[i] != q.path[i]) {
      return p.path[i] == 'L';
    }
  }
  return false;
}

bool Tree::IsComplete() const {
  // Check if a node is specified more than once.
  for (const_iterator i = nodes.cbegin(), j = i + 1; j != nodes.cend();
       ++i, ++j) {
    if (i->path == j->path) {
      return false;
    }
  }

  // Special case for root.
  if (nodes[0].path != "") {
    return false;
  }

  bool completed[MAX_NODES];
  for (int i = 0; i < (int)nodes.size(); i++) {
    completed[i] = false;
  }

  for (const_iterator i = nodes.cend() - 1; i > nodes.cbegin(); --i) {
    if (completed[i - nodes.cbegin()]) {
      continue;
    }

    for (const_iterator found, last = i; last != nodes.cbegin(); last = found) {
      std::string parent_path(last->path.begin(), last->path.end() - 1);
      found = Find(nodes.cbegin(), last, parent_path);
      if (found == last) {
        return false;
      }
      completed[found - nodes.cbegin()] = true;
    }
  }

  return true;
}

Tree::const_iterator Tree::Find(const_iterator first,
                                const_iterator last,
                                const std::string& path) const {
  Node node;
  node.path = path;
  const_iterator i = std::lower_bound(first, last, node);
  return (i != last && i->path == path) ? i : last;
}

std::istream& operator>>(std::istream& in, Tree& tree) {
  enum State { STATE_NONE, STATE_VALUE, STATE_COMMA, STATE_PATH, STATE_END, };

  tree.nodes.clear();

  State state = STATE_NONE;
  Node node;
  for (char c; in.get(c);) {
    switch (state) {
      case STATE_NONE:
        if (c == '(') {
          state = STATE_VALUE;
        } else {
          assert(isspace(c));
        }
        break;
      case STATE_VALUE:
        if (c == ')') {
          state = STATE_END;
        } else {
          assert(isspace(c) || isdigit(c));
          in.unget();
          in >> node.value;
          state = STATE_COMMA;
        }
        break;
      case STATE_COMMA:
        if (c == ',') {
          state = STATE_PATH;
        } else {
          assert(isspace(c));
        }
        break;
      case STATE_PATH:
        if (c == ')') {
          tree.nodes.push_back(node);
          node.path.clear();
          state = STATE_NONE;
        } else if (!isspace(c)) {
          node.path.push_back(c);
        }
        break;
      default:
        assert(false);
        break;
    }
    if (state == STATE_END) {
      break;
    }
  }
  std::sort(tree.nodes.begin(), tree.nodes.end());

  return in;
}

std::ostream& operator<<(std::ostream& out, const Tree& tree) {
  bool first = true;
  for (const auto& node : tree.nodes) {
    if (!first) {
      out << ' ';
    }
    first = false;

    out << node.value;
  }
  return out;
}

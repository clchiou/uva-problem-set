#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <tuple>

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

class Graph {
 public:
  Graph();
  void Connect(char p, char q);
  bool IsConnected(char p, char q) const;

 private:
  bool graph_[26][26];
};

std::string::const_iterator Find(std::string::const_iterator pos,
                                 std::string::const_iterator end,
                                 bool (*predicate)(char c)) {
  while (pos != end && !predicate(*pos)) {
    ++pos;
  }
  return pos;
}

std::string ComputeAlphabet(std::string string) {
  bool has_letters[26];
  memset(has_letters, 0, sizeof(has_letters));
  for (char c : string) {
    if (isupper(c)) {
      has_letters[c - 'A'] = true;
    }
  }
  std::string alphabet;
  for (int i = 0; i < 26; i++) {
    if (has_letters[i]) {
      alphabet.push_back('A' + i);
    }
  }
  return alphabet;
}

Graph::Graph() { memset(graph_, 0, sizeof(graph_)); }

void Graph::Connect(char p, char q) {
  graph_[p - 'A'][q - 'A'] = graph_[q - 'A'][p - 'A'] = true;
}

bool Graph::IsConnected(char p, char q) const {
  return graph_[p - 'A'][q - 'A'];
}

void SolveImpl(const std::string& alphabet,
               const Graph& graph,
               int depth,
               bool used[],
               char partial_ordering[],
               char ordering[],
               int* bandwidth) {
  if (depth == static_cast<int>(alphabet.size())) {
    int new_bandwidth = 0;
    for (int i = 0; i < depth; i++) {
      for (int j = i + 1; j < depth; j++) {
        if (graph.IsConnected(partial_ordering[i], partial_ordering[j])) {
          new_bandwidth = std::max(new_bandwidth, j - i);
        }
      }
    }
    if (new_bandwidth < *bandwidth) {
      strcpy(ordering, partial_ordering);
      *bandwidth = new_bandwidth;
    }
    return;
  }

  for (int i = 0; i < static_cast<int>(alphabet.size()); i++) {
    if (!used[i]) {
      used[i] = true;
      partial_ordering[depth] = alphabet[i];
      SolveImpl(alphabet,
                graph,
                depth + 1,
                used,
                partial_ordering,
                ordering,
                bandwidth);
      used[i] = false;
    }
  }
}

std::tuple<std::string, int> Solve(const std::string& alphabet,
                                   const Graph& graph) {
  int bandwidth = 26;
  bool used[26];
  char partial_ordering[26];
  char ordering[26];
  memset(used, 0, sizeof(used));
  memset(partial_ordering, 0, sizeof(partial_ordering));
  memset(ordering, 0, sizeof(ordering));
  SolveImpl(alphabet, graph, 0, used, partial_ordering, ordering, &bandwidth);
  return std::make_tuple(std::string(ordering), bandwidth);
}

int main() {
  for (std::string line; std::getline(std::cin, line) && line != "#";) {
    std::string alphabet = ComputeAlphabet(line);
    LOG << "alphabet=" << alphabet << std::endl;

    Graph graph;
    {
      auto IsUpper = [](char c) { return static_cast<bool>(isupper(c)); };
      auto IsColon = [](char c) { return c == ':'; };
      auto pos = line.cbegin();
      auto end = line.cend();
      while (true) {
        pos = Find(pos, end, IsUpper);
        if (pos == end) {
          break;
        }
        char p = *pos, q;
        pos = Find(pos, end, IsColon);
        assert(pos != end);
        pos = Find(pos, end, IsUpper);
        assert(pos != end);
        do {
          q = *pos++;
          if (isupper(q)) {
            graph.Connect(p, q);
          }
        } while (q != ';' && pos != end);
      }
      if (DEBUG) {
        LOG << ' ' << alphabet << std::endl;
        for (char p : alphabet) {
          LOG << p;
          for (char q : alphabet) {
            LOG << static_cast<int>(graph.IsConnected(p, q));
          }
          LOG << std::endl;
        }
      }
    }

    std::string ordering;
    int bandwidth;
    std::tie(ordering, bandwidth) = Solve(alphabet, graph);
    for (char c : ordering) {
      std::cout << c << ' ';
    }
    std::cout << "-> " << bandwidth << std::endl;
  }
  return 0;
}

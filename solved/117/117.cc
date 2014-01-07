#include <algorithm>
#include <iostream>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

const int NUM_VERTICES = 26;

class Graph {
 public:
  friend std::istream& operator>>(std::istream& in, Graph& graph);
  friend std::ostream& operator<<(std::ostream& in, const Graph& graph);

  Graph();

  int weight(int i, int j) const {
    return const_cast<Graph*>(this)->weight(i, j);
  }
  int& weight(int i, int j) { return weights_[i][j]; }
  int degree(int i) const;

 private:
  int weights_[NUM_VERTICES][NUM_VERTICES];
};

bool IsOddNumber(int n) { return n % 2; }

void ComputeShortestPath(const Graph& graph, Graph* distance) {
  for (int i = 0; i < NUM_VERTICES; i++)
    for (int j = 0; j < NUM_VERTICES; j++)
      distance->weight(i, j) = graph.weight(i, j);

  for (int k = 0; k < NUM_VERTICES; k++)
    for (int i = 0; i < NUM_VERTICES; i++)
      for (int j = 0; j < NUM_VERTICES; j++) {
        if (distance->weight(i, k) == 0 || distance->weight(k, j) == 0)
          continue;
        int d = distance->weight(i, k) + distance->weight(k, j);
        if (distance->weight(i, j) == 0 || d < distance->weight(i, j)) {
          distance->weight(i, j) = d;
          distance->weight(j, i) = d;
        }
      }
}

int Solve(const Graph& graph) {
  int num_odd_degree_vertices = 0, odd_degree_vertices[2];
  for (int i = 0; i < NUM_VERTICES; i++)
    if (IsOddNumber(graph.degree(i)))
      odd_degree_vertices[num_odd_degree_vertices++] = i;

  int sum = 0;
  for (int i = 0; i < NUM_VERTICES; i++)
    for (int j = i + 1; j < NUM_VERTICES; j++)
      sum += graph.weight(i, j);

  if (num_odd_degree_vertices == 0)
    return sum;

  assert(num_odd_degree_vertices == 2);

  if (odd_degree_vertices[0] == odd_degree_vertices[1])
    return sum;

  Graph distance;
  ComputeShortestPath(graph, &distance);
  return sum + distance.weight(odd_degree_vertices[0], odd_degree_vertices[1]);
}

int main() {
  for (Graph graph; std::cin >> graph;)
    std::cout << Solve(graph) << std::endl;
  return 0;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
  for (int i = 0; i < NUM_VERTICES; i++)
    std::fill(graph.weights_[i], graph.weights_[i] + NUM_VERTICES, 0);
  for (std::string route; (in >> route) && route != "deadend";) {
    int i = route[0] - 'a';
    int j = route[route.size() - 1] - 'a';
    graph.weights_[i][j] = route.size();
    graph.weights_[j][i] = route.size();
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  for (int i = 0; i < NUM_VERTICES; i++)
    for (int j = i + 1; j < NUM_VERTICES; j++)
      if (graph.weights_[i][j])
        out << static_cast<char>('a' + i) << ' ' << static_cast<char>('a' + j)
            << ' ' << graph.weights_[i][j] << std::endl;
  return out;
}

Graph::Graph() {
  for (int i = 0; i < NUM_VERTICES; i++)
    std::fill(weights_[i], weights_[i] + NUM_VERTICES, 0);
}

int Graph::degree(int i) const {
  return NUM_VERTICES - std::count(weights_[i], weights_[i] + NUM_VERTICES, 0);
}

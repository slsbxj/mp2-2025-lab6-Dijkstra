#ifndef GRAPH_H
#define GRAPH_H

#include "dheap.h"
#include <climits>
#include <string>
#include <vector>

struct Edge {
  int to;
  int weight;
  Edge(int t = 0, int w = 0) : to(t), weight(w) {}
};

struct Vertex {
  int id;
  std::string name;
  Vertex(int i = 0, const std::string &n = "") : id(i), name(n) {}
};

struct DijkstraResult {
  std::vector<int> distances;
  std::vector<int> previous;
  std::vector<bool> visited;
};

class Graph {
private:
  std::vector<std::vector<Edge>> adjList;
  std::vector<Vertex> vertices;
  bool directed;

  bool checkConnectivity() const;
  bool checkNonNegativeWeights() const;

public:
  Graph(bool isDirected = false);
  ~Graph() {}

  void addVertex(int id, const std::string &name = "");
  void addEdge(int from, int to, int weight);
  void removeEdge(int from, int to);
  void removeVertex(int id);
  void generateRandom(int vertexCount, int edgeCount, int minWeight = 1,
                      int maxWeight = 100);

  DijkstraResult dijkstra(int startVertex, int dHeapDegree = 2);

  std::vector<int> getPath(int start, int target,
                           const DijkstraResult &result) const;
  std::string getPathString(int start, int target,
                            const DijkstraResult &result) const;
  int getDistance(int target, const DijkstraResult &result) const;

  bool isReadyForDijkstra() const;
  std::string getErrorMessage() const;

  void printGraph() const;
  std::string toString() const;

  int getVertexCount() const;
  int getEdgeCount() const;
  std::vector<Vertex> getVertices() const;
  const std::vector<std::vector<Edge>> &getAdjList() const;
  bool isDirectedGraph() const { return directed; }
};

#endif
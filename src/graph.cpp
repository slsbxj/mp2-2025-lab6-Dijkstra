#include "graph.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>

Graph::Graph(bool isDirected) : directed(isDirected) {}

void Graph::addVertex(int id, const std::string &name) {
  for (size_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i].id == id)
      return;
  }
  if (id >= (int)adjList.size()) {
    adjList.resize(id + 1);
  }
  vertices.push_back(Vertex(id, name.empty() ? std::to_string(id) : name));
}

void Graph::addEdge(int from, int to, int weight) {
  if (weight < 0) {
    throw std::invalid_argument("Weight cannot be negative for Dijkstra");
  }

  int maxId = from > to ? from : to;
  if (maxId >= (int)adjList.size()) {
    adjList.resize(maxId + 1);
  }

  bool hasFrom = false, hasTo = false;
  for (size_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i].id == from)
      hasFrom = true;
    if (vertices[i].id == to)
      hasTo = true;
  }
  if (!hasFrom)
    addVertex(from, std::to_string(from));
  if (!hasTo)
    addVertex(to, std::to_string(to));

  bool edgeFound = false;
  for (size_t i = 0; i < adjList[from].size(); ++i) {
    if (adjList[from][i].to == to) {
      if (weight < adjList[from][i].weight) {
        adjList[from][i].weight = weight;
      }
      edgeFound = true;
      break;
    }
  }
  if (!edgeFound) {
    adjList[from].push_back(Edge(to, weight));
  }

  if (!directed) {
    bool reverseFound = false;
    for (size_t i = 0; i < adjList[to].size(); ++i) {
      if (adjList[to][i].to == from) {
        if (weight < adjList[to][i].weight) {
          adjList[to][i].weight = weight;
        }
        reverseFound = true;
        break;
      }
    }
    if (!reverseFound) {
      adjList[to].push_back(Edge(from, weight));
    }
  }
}

void Graph::removeEdge(int from, int to) {
  if (from >= (int)adjList.size() || to >= (int)adjList.size())
    return;

  for (size_t i = 0; i < adjList[from].size(); ++i) {
    if (adjList[from][i].to == to) {
      adjList[from].erase(adjList[from].begin() + i);
      break;
    }
  }

  if (!directed) {
    for (size_t i = 0; i < adjList[to].size(); ++i) {
      if (adjList[to][i].to == from) {
        adjList[to].erase(adjList[to].begin() + i);
        break;
      }
    }
  }
}

void Graph::removeVertex(int id) {
  if (id >= (int)adjList.size())
    return;

  for (size_t i = 0; i < adjList.size(); ++i) {
    for (size_t j = 0; j < adjList[i].size(); ++j) {
      if (adjList[i][j].to == id) {
        adjList[i].erase(adjList[i].begin() + j);
        --j;
      }
    }
  }

  adjList[id].clear();

  for (size_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i].id == id) {
      vertices.erase(vertices.begin() + i);
      break;
    }
  }
}

void Graph::generateRandom(int vertexCount, int edgeCount, int minWeight,
                           int maxWeight) {
  adjList.clear();
  vertices.clear();
  srand((unsigned int)time(NULL));

  for (int i = 0; i < vertexCount; ++i) {
    addVertex(i, "V" + std::to_string(i));
  }

  std::vector<bool> connected(vertexCount, false);
  connected[0] = true;
  int connectedCount = 1;

  while (connectedCount < vertexCount) {
    int u = rand() % vertexCount;
    int v = rand() % vertexCount;
    if (connected[u] != connected[v]) {
      int weight = minWeight + (rand() % (maxWeight - minWeight + 1));
      addEdge(u, v, weight);
      connected[u] = true;
      connected[v] = true;
      connectedCount++;
    }
  }

  int maxPossibleEdges = vertexCount * (vertexCount - 1) / 2;
  int targetEdges =
      (edgeCount > maxPossibleEdges) ? maxPossibleEdges : edgeCount;

  std::set<std::pair<int, int>> existingEdges;
  for (int i = 0; i < vertexCount; ++i) {
    for (const auto &edge : adjList[i]) {
      if (i < edge.to) {
        existingEdges.insert({i, edge.to});
      }
    }
  }

  while ((int)existingEdges.size() < targetEdges) {
    int u = rand() % vertexCount;
    int v = rand() % vertexCount;
    if (u != v) {
      int a = std::min(u, v);
      int b = std::max(u, v);
      if (existingEdges.find({a, b}) == existingEdges.end()) {
        int weight = minWeight + (rand() % (maxWeight - minWeight + 1));
        addEdge(u, v, weight);
        existingEdges.insert({a, b});
      }
    }
  }
}

bool Graph::checkConnectivity() const {
  if (vertices.empty())
    return false;

  int n = (int)adjList.size();
  if (n == 0)
    return false;

  std::vector<bool> visited(n, false);
  std::queue<int> q;

  int start = vertices[0].id;
  q.push(start);
  visited[start] = true;

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (size_t i = 0; i < adjList[v].size(); ++i) {
      int to = adjList[v][i].to;
      if (to >= 0 && to < n && !visited[to]) {
        visited[to] = true;
        q.push(to);
      }
    }
  }

  for (size_t i = 0; i < vertices.size(); ++i) {
    int id = vertices[i].id;
    if (id >= 0 && id < n && !visited[id])
      return false;
  }
  return true;
}

bool Graph::checkNonNegativeWeights() const {
  for (size_t i = 0; i < adjList.size(); ++i) {
    for (size_t j = 0; j < adjList[i].size(); ++j) {
      if (adjList[i][j].weight < 0)
        return false;
    }
  }
  return true;
}

bool Graph::isReadyForDijkstra() const {
  if (vertices.empty())
    return false;
  if (!checkConnectivity())
    return false;
  if (!checkNonNegativeWeights())
    return false;
  return true;
}

std::string Graph::getErrorMessage() const {
  if (vertices.empty())
    return "Граф пуст";
  if (!checkConnectivity())
    return "Граф не является связным";
  if (!checkNonNegativeWeights())
    return "Граф содержит отрицательные веса рёбер";
  return "OK";
}

DijkstraResult Graph::dijkstra(int startVertex, int dHeapDegree) {
  DijkstraResult result;
  int n = (int)adjList.size();
  if (n == 0)
    return result;

  const int INF = INT_MAX;
  result.distances.assign(n, INF);
  result.previous.assign(n, -1);
  result.visited.assign(n, false);

  if (startVertex < 0 || startVertex >= n)
    return result;

  result.distances[startVertex] = 0;

  DHeap<std::pair<int, int>, std::less<std::pair<int, int>>> heap(dHeapDegree);
  heap.push(std::make_pair(0, startVertex));

  while (!heap.empty()) {
    std::pair<int, int> top = heap.top();
    int dist = top.first;
    int v = top.second;
    heap.pop();

    if (result.visited[v])
      continue;
    result.visited[v] = true;

    for (size_t i = 0; i < adjList[v].size(); ++i) {
      const Edge &e = adjList[v][i];
      if (e.to < 0 || e.to >= n)
        continue;

      int newDist = dist + e.weight;
      if (newDist < result.distances[e.to]) {
        result.distances[e.to] = newDist;
        result.previous[e.to] = v;
        heap.push(std::make_pair(newDist, e.to));
      }
    }
  }
  return result;
}

std::vector<int> Graph::getPath(int start, int target,
                                const DijkstraResult &result) const {
  std::vector<int> path;
  if (target < 0 || target >= (int)result.distances.size())
    return path;
  if (result.distances[target] == INT_MAX)
    return path;

  for (int v = target; v != -1 && v != start; v = result.previous[v]) {
    path.push_back(v);
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

std::string Graph::getPathString(int start, int target,
                                 const DijkstraResult &result) const {
  if (target < 0 || target >= (int)result.distances.size()) {
    return "Нет пути (вершина не существует)";
  }

  std::vector<int> path = getPath(start, target, result);
  if (path.empty() || result.distances[target] == INT_MAX) {
    return "Нет пути";
  }

  std::stringstream ss;
  for (size_t i = 0; i < path.size(); ++i) {
    if (i > 0)
      ss << " -> ";
    ss << path[i];
  }
  ss << " (стоимость: " << result.distances[target] << ")";
  return ss.str();
}

int Graph::getDistance(int target, const DijkstraResult &result) const {
  if (target < 0 || target >= (int)result.distances.size())
    return INT_MAX;
  return result.distances[target];
}

void Graph::printGraph() const { std::cout << toString(); }

std::string Graph::toString() const {
  std::stringstream ss;
  ss << "\n=== ГРАФ ===\n";
  ss << "Тип: " << (directed ? "Ориентированный" : "Неориентированный") << "\n";
  ss << "Вершин: " << vertices.size() << "\n";
  ss << "Рёбер: " << getEdgeCount() << "\n";

  ss << "\nСписок смежности:\n";
  for (size_t vi = 0; vi < vertices.size(); ++vi) {
    const Vertex &v = vertices[vi];
    if (v.id >= (int)adjList.size())
      continue;

    ss << v.id << " (" << v.name << "): ";
    if (adjList[v.id].empty()) {
      ss << "(нет рёбер)";
    } else {
      for (size_t i = 0; i < adjList[v.id].size(); ++i) {
        const Edge &e = adjList[v.id][i];
        if (i > 0)
          ss << ", ";
        ss << "-> " << e.to << " (" << e.weight << ")";
      }
    }
    ss << "\n";
  }
  return ss.str();
}

int Graph::getVertexCount() const { return (int)vertices.size(); }

int Graph::getEdgeCount() const {
  int count = 0;
  for (size_t i = 0; i < adjList.size(); ++i) {
    count += (int)adjList[i].size();
  }
  if (!directed)
    count /= 2;
  return count;
}

std::vector<Vertex> Graph::getVertices() const { return vertices; }

const std::vector<std::vector<Edge>> &Graph::getAdjList() const {
  return adjList;
}
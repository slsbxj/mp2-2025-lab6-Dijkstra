#include "graph.h"
#include "gtest.h"

TEST(GraphTest, AddVertexAndEdge) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addEdge(0, 1, 5);

  EXPECT_EQ(g.getVertexCount(), 2);
  EXPECT_EQ(g.getEdgeCount(), 1);
}

TEST(GraphTest, NegativeWeightThrows) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");

  EXPECT_THROW(g.addEdge(0, 1, -5), std::invalid_argument);
}

TEST(GraphTest, DijkstraWorks) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addVertex(2, "C");
  g.addEdge(0, 1, 4);
  g.addEdge(0, 2, 2);
  g.addEdge(1, 2, 1);

  DijkstraResult result = g.dijkstra(0);

  EXPECT_EQ(g.getDistance(0, result), 0);
  EXPECT_EQ(g.getDistance(1, result), 3);
  EXPECT_EQ(g.getDistance(2, result), 2);
}

TEST(GraphTest, PathRetrieval) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addVertex(2, "C");
  g.addEdge(0, 1, 4);
  g.addEdge(0, 2, 2);
  g.addEdge(1, 2, 1);

  DijkstraResult result = g.dijkstra(0);
  std::vector<int> path = g.getPath(0, 1, result);

  ASSERT_EQ(path.size(), 3u);
  EXPECT_EQ(path[0], 0);
  EXPECT_EQ(path[1], 2);
  EXPECT_EQ(path[2], 1);
}

TEST(GraphTest, PathToItself) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addEdge(0, 1, 5);

  DijkstraResult result = g.dijkstra(0);
  std::vector<int> path = g.getPath(0, 0, result);

  ASSERT_EQ(path.size(), 1u);
  EXPECT_EQ(path[0], 0);
}

TEST(GraphTest, GraphIsConnected) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addVertex(2, "C");
  g.addEdge(0, 1, 1);
  g.addEdge(1, 2, 1);

  EXPECT_TRUE(g.isReadyForDijkstra());
}

TEST(GraphTest, GraphNotConnected) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addVertex(2, "C");
  g.addEdge(0, 1, 1);

  EXPECT_FALSE(g.isReadyForDijkstra());
  EXPECT_EQ(g.getErrorMessage(), "Граф не является связным");
}

TEST(GraphTest, GenerateRandomGraph) {
  Graph g;
  g.generateRandom(5, 7, 1, 10);

  EXPECT_EQ(g.getVertexCount(), 5);
  EXPECT_GE(g.getEdgeCount(), 1);
}

TEST(GraphTest, RemoveEdge) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addEdge(0, 1, 5);
  EXPECT_EQ(g.getEdgeCount(), 1);

  g.removeEdge(0, 1);
  EXPECT_EQ(g.getEdgeCount(), 0);
}

TEST(GraphTest, RemoveVertex) {
  Graph g;
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addEdge(0, 1, 5);
  EXPECT_EQ(g.getVertexCount(), 2);

  g.removeVertex(0);
  EXPECT_EQ(g.getVertexCount(), 1);
}

TEST(GraphTest, DirectedGraph) {
  Graph g(true);
  g.addVertex(0, "A");
  g.addVertex(1, "B");
  g.addEdge(0, 1, 5);

  EXPECT_TRUE(g.isDirectedGraph());
  EXPECT_EQ(g.getEdgeCount(), 1);

  const std::vector<std::vector<Edge>> &adjList = g.getAdjList();
  bool hasReverse = false;
  for (size_t i = 0; i < adjList[1].size(); ++i) {
    if (adjList[1][i].to == 0)
      hasReverse = true;
  }
  EXPECT_FALSE(hasReverse);
}

TEST(GraphTest, LargeGraphDijkstra) {
  Graph g;
  for (int i = 0; i < 10; ++i) {
    g.addVertex(i, "V" + std::to_string(i));
  }
  for (int i = 0; i < 9; ++i) {
    g.addEdge(i, i + 1, 1);
  }

  DijkstraResult result = g.dijkstra(0);
  EXPECT_EQ(g.getDistance(9, result), 9);
}
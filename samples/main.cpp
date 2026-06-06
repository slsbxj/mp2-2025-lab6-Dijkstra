#include "graph.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

void printMenu() {
  cout << "\n========================================\n";
  cout << "    АЛГОРИТМ ДЕЙКСТРЫ С d-КУЧЕЙ\n";
  cout << "========================================\n";
  cout << "1. Ручной ввод графа\n";
  cout << "2. Автоматическая генерация графа\n";
  cout << "3. Вывести граф\n";
  cout << "4. Запустить алгоритм Дейкстры\n";
  cout << "5. Найти кратчайший путь до вершины\n";
  cout << "6. Вывести все расстояния\n";
  cout << "7. Настройка степени d-кучи\n";
  cout << "8. Проверить граф на готовность к Дейкстре\n";
  cout << "0. Выход\n";
  cout << "========================================\n";
  cout << "Выберите действие: ";
}

void manualInput(Graph &graph) {
  cout << "\n--- РУЧНОЙ ВВОД ГРАФА ---\n";

  int vertexCount;
  cout << "Количество вершин: ";
  cin >> vertexCount;

  cin.ignore();
  for (int i = 0; i < vertexCount; ++i) {
    string name;
    cout << "Имя вершины " << i << " (Enter для " << i << "): ";
    getline(cin, name);
    if (name.empty())
      name = to_string(i);
    graph.addVertex(i, name);
  }

  int edgeCount;
  cout << "Количество рёбер: ";
  cin >> edgeCount;

  for (int i = 0; i < edgeCount; ++i) {
    int from, to, weight;
    cout << "Ребро " << i + 1 << " (from to weight): ";
    cin >> from >> to >> weight;
    try {
      graph.addEdge(from, to, weight);
    } catch (const exception &e) {
      cout << "Ошибка: " << e.what() << endl;
      i--;
    }
  }

  cout << "Граф успешно создан!\n";
}

void generateRandom(Graph &graph) {
  cout << "\n--- АВТОМАТИЧЕСКАЯ ГЕНЕРАЦИЯ ---\n";

  int vertexCount, edgeCount, minWeight, maxWeight;
  cout << "Количество вершин: ";
  cin >> vertexCount;
  cout << "Количество рёбер: ";
  cin >> edgeCount;
  cout << "Минимальный вес: ";
  cin >> minWeight;
  cout << "Максимальный вес: ";
  cin >> maxWeight;

  graph.generateRandom(vertexCount, edgeCount, minWeight, maxWeight);
  cout << "Граф успешно сгенерирован!\n";
}

int main() {
  setlocale(LC_ALL, "Russian");

  Graph graph(false);
  int dDegree = 2;
  DijkstraResult lastResult;
  bool hasResult = false;
  int lastStartVertex = -1;

  int choice;
  do {
    printMenu();

    if (!(cin >> choice)) {
      cout << "Ошибка: введите число!\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }

    switch (choice) {
    case 1:
      manualInput(graph);
      hasResult = false;
      lastStartVertex = -1;
      break;
    case 2:
      generateRandom(graph);
      hasResult = false;
      lastStartVertex = -1;
      break;
    case 3:
      graph.printGraph();
      break;
    case 4: {
      cout << "\n--- АЛГОРИТМ ДЕЙКСТРЫ ---\n";

      if (!graph.isReadyForDijkstra()) {
        cout << "Граф не готов для алгоритма Дейкстры!\n";
        cout << "Причина: " << graph.getErrorMessage() << endl;
        break;
      }

      int startVertex;
      cout << "Введите начальную вершину: ";

      if (!(cin >> startVertex)) {
        cout << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }

      cout << "Запуск алгоритма Дейкстры с d-кучей степени " << dDegree
           << "...\n";
      lastResult = graph.dijkstra(startVertex, dDegree);
      hasResult = true;
      lastStartVertex = startVertex;

      cout << "\nРезультаты работы алгоритма:\n";
      cout << "Вершина | Расстояние | Путь от " << startVertex << "\n";
      cout << "----------------------------------------\n";

      vector<Vertex> vertices = graph.getVertices();
      for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex &v = vertices[i];
        int dist = graph.getDistance(v.id, lastResult);
        string path = graph.getPathString(startVertex, v.id, lastResult);
        cout << "   " << v.id << "    |     ";
        if (dist == INT_MAX) {
          cout << "нет пути";
        } else {
          cout << dist;
        }
        cout << "     | " << path << "\n";
      }
      break;
    }
    case 5: {
      if (!hasResult) {
        cout << "Сначала выполните алгоритм Дейкстры (пункт 4)!\n";
        break;
      }
      int targetVertex;
      cout << "Введите целевую вершину: ";

      if (!(cin >> targetVertex)) {
        cout << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }

      string path =
          graph.getPathString(lastStartVertex, targetVertex, lastResult);
      cout << "Кратчайший путь: " << path << endl;
      break;
    }
    case 6: {
      if (!hasResult) {
        cout << "Сначала выполните алгоритм Дейкстры (пункт 4)!\n";
        break;
      }

      cout << "\n--- ВСЕ РАССТОЯНИЯ ---\n";
      vector<Vertex> vertices = graph.getVertices();
      for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex &v = vertices[i];
        int dist = graph.getDistance(v.id, lastResult);
        cout << "До вершины " << v.id << " (" << v.name << "): ";
        if (dist == INT_MAX) {
          cout << "нет пути";
        } else {
          cout << dist;
        }
        cout << endl;
      }
      break;
    }
    case 7:
      cout << "Введите степень d-кучи (>=2): ";
      if (!(cin >> dDegree)) {
        cout << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }
      if (dDegree < 2)
        dDegree = 2;
      cout << "Степень d-кучи установлена: " << dDegree << endl;
      break;
    case 8:
      cout << "\n--- ПРОВЕРКА ГРАФА ---\n";
      if (graph.isReadyForDijkstra()) {
        cout << "Граф готов для алгоритма Дейкстры!\n";
        cout << "✓ Граф связный\n";
        cout << "✓ Нет отрицательных весов\n";
      } else {
        cout << "Граф НЕ готов для алгоритма Дейкстры!\n";
        cout << "Причина: " << graph.getErrorMessage() << endl;
      }
      break;
    case 0:
      cout << "До свидания!\n";
      break;
    default:
      cout << "Неверный выбор. Попробуйте снова.\n";
    }
  } while (choice != 0);

  return 0;
}
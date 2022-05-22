#include "Dijkstra.h"
#include <iostream>
int minDistance(int numVertices, int dist[], bool sptSet[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < numVertices; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

// Функция, которая реализует алгоритм кратчайшего пути из одного источника Дейкстры
// для графа, представленного с использованием представления матрицы смежности
int* dijkstra(int **graph, int numVertices, int src, int* parent)
{
	int* dist = new int[numVertices];

	bool* sptSet = new bool[numVertices]; // sptSet[i] будет истинным, если вершина i входит в кратчайший
		// дерево путей или кратчайшее расстояние от src до i завершено

	// Родительский массив для хранения дерева кратчайших путей

	// Инициализировать все расстояния как INFINITE и stpSet[] как false
	for (int i = 0; i < numVertices; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	// Расстояние исходной вершины от самой себя всегда равно 0
	dist[src] = 0;
	// Находим кратчайший путь для всех вершин
	for (int count = 0; count < numVertices - 1; count++) {
		// Выбираем вершину минимального расстояния из набора вершин, не
		// еще не обработано. u всегда равно src в первой итерации.
		int u = minDistance(numVertices, dist, sptSet);
		// Помечаем выбранную вершину как обработанную
		sptSet[u] = true;
		// Обновить значение расстояния соседних вершин выбранной вершины.
		for (int v = 0; v < numVertices; v++) {
			// Обновляем dist[v] только если нет в sptSet, есть ребро из
			// от u до v, а общий вес пути от src к v через u равен
			// меньше текущего значения dist[v]
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v]) {
				parent[v] = u;
				dist[v] = dist[u] + graph[u][v];
			}
		}
	}

	return dist;
}
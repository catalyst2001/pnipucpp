#pragma once
#include <limits.h>
int minDistance(int numVertices, int dist[], bool sptSet[]);
int* dijkstra(int **graph, int numVertices, int src, int* parent);
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

// �������, ������� ��������� �������� ����������� ���� �� ������ ��������� ��������
// ��� �����, ��������������� � �������������� ������������� ������� ���������
int* dijkstra(int **graph, int numVertices, int src, int* parent)
{
	int* dist = new int[numVertices];

	bool* sptSet = new bool[numVertices]; // sptSet[i] ����� ��������, ���� ������� i ������ � ����������
		// ������ ����� ��� ���������� ���������� �� src �� i ���������

	// ������������ ������ ��� �������� ������ ���������� �����

	// ���������������� ��� ���������� ��� INFINITE � stpSet[] ��� false
	for (int i = 0; i < numVertices; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	// ���������� �������� ������� �� ����� ���� ������ ����� 0
	dist[src] = 0;
	// ������� ���������� ���� ��� ���� ������
	for (int count = 0; count < numVertices - 1; count++) {
		// �������� ������� ������������ ���������� �� ������ ������, ��
		// ��� �� ����������. u ������ ����� src � ������ ��������.
		int u = minDistance(numVertices, dist, sptSet);
		// �������� ��������� ������� ��� ������������
		sptSet[u] = true;
		// �������� �������� ���������� �������� ������ ��������� �������.
		for (int v = 0; v < numVertices; v++) {
			// ��������� dist[v] ������ ���� ��� � sptSet, ���� ����� ��
			// �� u �� v, � ����� ��� ���� �� src � v ����� u �����
			// ������ �������� �������� dist[v]
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v]) {
				parent[v] = u;
				dist[v] = dist[u] + graph[u][v];
			}
		}
	}

	return dist;
}
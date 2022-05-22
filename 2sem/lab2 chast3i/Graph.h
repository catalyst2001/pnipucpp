#pragma once
#include <iostream>
#include <vector>
using namespace std;

class GraphNode {
public:
	int i, x, y;
	int highlight = 0;
	GraphNode() {}
	GraphNode(int i, int x, int y) : i(i), x(x), y(y) {}
};

class GraphEdge {
public:
	GraphNode* first;
	GraphNode* second;
	int highlight = 0;
	int weight = 0;
	GraphEdge() {}
	GraphEdge(int w, GraphNode* n1, GraphNode* n2) : weight(w), first(n1), second(n2) {}
};

class Graph {
private:
	int numVertices;
	vector<GraphNode*> nodes;
	vector<GraphEdge*> edges;

public:
	Graph(int numVertices);

	GraphNode* addNode(int i, int x, int y);
	GraphEdge* addEdge(GraphNode* n1, GraphNode* n2, int w);
	vector<GraphNode*> getNodes();
	vector<GraphEdge*> getEdges();
	int** matrix;
	~Graph();

	int size();
};
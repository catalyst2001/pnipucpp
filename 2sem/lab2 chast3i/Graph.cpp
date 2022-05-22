#include "Graph.h"

Graph::Graph(int numVertices) {
	this->numVertices = numVertices;
	matrix = new int*[numVertices];
	for (int i = 0; i < numVertices; i++) {
		matrix[i] = new int[numVertices];
		for (int j = 0; j < numVertices; j++)
			matrix[i][j] = 0;
	}
}

GraphNode* Graph::addNode(int i, int x, int y) {
	GraphNode* g = new GraphNode(i, x, y);
	nodes.push_back(g);
	return g;
}

GraphEdge* Graph::addEdge(GraphNode* n1, GraphNode* n2, int w) {
	matrix[(*n1).i-1][(*n2).i-1] = w;
	matrix[(*n2).i-1][(*n1).i-1] = w;
	GraphEdge* edge = new GraphEdge(w, n1, n2);
	edges.push_back(edge);
	return edge;
}

Graph::~Graph() {
	for (int i = 0; i < numVertices; i++)
		delete[] matrix[i];
	delete[] matrix;
}

vector<GraphNode*> Graph::getNodes() {
	return nodes;
}

vector<GraphEdge*> Graph::getEdges() {
	return edges;
}

int Graph::size() {
	return numVertices;
}
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GraphVisualizer.h"
#include "Graph.h"
#include "Dijkstra.h"

#define START_NODE 6

Graph myGraph = Graph(7);

int main()
{
	SFMLGraphVisualize graphviz;

	GraphNode *node1 = myGraph.addNode(1, 400, 50);
	GraphNode *node2 = myGraph.addNode(2, 455, 520);
	GraphNode *node3 = myGraph.addNode(3, 600, 150);
	GraphNode *node4 = myGraph.addNode(4, 310, 420);
	GraphNode *node5 = myGraph.addNode(5, 800, 400);
	GraphNode *node6 = myGraph.addNode(6, 170, 275);
	GraphNode *node7 = myGraph.addNode(7, 100, 70);
	myGraph.addEdge(node1, node2, 42);
	myGraph.addEdge(node1, node7, 12);
	myGraph.addEdge(node1, node3, 19);
	myGraph.addEdge(node2, node4, 23);
	myGraph.addEdge(node2, node5, 19);
	myGraph.addEdge(node3, node6, 30);
	myGraph.addEdge(node3, node5, 34);
	myGraph.addEdge(node4, node6, 9);
	myGraph.addEdge(node5, node7, 47);
	myGraph.addEdge(node6, node7, 26);

	//myGraph.toScreen();
	//graphviz.highlightPath(path);


	int* parent = new int[myGraph.size()];
	parent[START_NODE - 1] = -1;

	int* paths = dijkstra(myGraph.matrix, myGraph.size(), START_NODE - 1, parent);

	graphviz.visualize(&myGraph, START_NODE, paths, parent);
	
	return 0;
}

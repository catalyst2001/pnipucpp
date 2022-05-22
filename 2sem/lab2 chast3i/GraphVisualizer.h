#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>
#include <sstream>
#include <vector>
#include "Graph.h"

int node_hovered(GraphNode* p_node, sf::Vector2i& pt);
void calcPath(int parent[], int j, std::vector<int>& out);

class SFMLGraphVisualize
{
private:
	sf::RenderWindow window;
	sf::Font font;
public:
	SFMLGraphVisualize();
	void drawNode(GraphNode* node, sf::Color nodeColor);
	void drawEdge(GraphEdge* edge, sf::Color color, double weight);
	void visualize(Graph* g, int start_node, int* paths, int* parent);
};
#include "GraphVisualizer.h"


#define NODERADIUS 30
using namespace std;

sf::Color colors[] = {
	sf::Color(0, 0, 0), //inactive
	sf::Color(255, 0, 0) //active
};
vector<int> shortestPath;

SFMLGraphVisualize::SFMLGraphVisualize()
{
	this->font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
}

void SFMLGraphVisualize::visualize(Graph* g, int start_node, int* paths, int* parent)
{
	window.create(sf::VideoMode(850, 580), "Nechaev Igor - Dijkstra graph algorithm");

	vector<GraphNode*> nodes = g->getNodes();
	vector<GraphEdge*> edges = g->getEdges();

	while (window.isOpen())
	{
		sf::Event event;

		static sf::Vector2i cursorPos;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonReleased)
			{
				cursorPos = sf::Mouse::getPosition(window);
				for (unsigned int i = 0; i < nodes.size(); i++)
				{
					nodes[i]->highlight = 0;
					int hover = node_hovered(nodes[i], cursorPos);
					if (hover == 1) {
						nodes[i]->highlight = 1;
						shortestPath = {start_node-1};
						calcPath(parent, i, shortestPath);

						std::cout << "Shortest path from " << start_node << " to " << i+1 << ": ";
						for (int k = 0; k < shortestPath.size(); k++){
							for (unsigned int j = 0; j < edges.size(); j++){
							edges[j]->highlight = 0;
								if (std::find(shortestPath.begin(), shortestPath.end(), edges[j]->first->i-1) != shortestPath.end()
									&& std::find(shortestPath.begin(), shortestPath.end(), edges[j]->second->i-1) != shortestPath.end())
								{
									edges[j]->highlight = 1;
								}
								else {
									edges[j]->highlight = 0;
								}
							}
							std::cout << " " << shortestPath[k] + 1;
						}
						std::cout << std::endl;
					}
				}
			}
		}

		window.clear(sf::Color::White);


		for (unsigned int j = 0; j < edges.size(); j++)
		{
			drawEdge(edges[j], colors[edges[j]->highlight], edges[j]->weight);
		}

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			GraphNode* node = nodes[i];
			drawNode(node, colors[node->highlight]);
		}
		window.display();
	}
}

void SFMLGraphVisualize::drawNode(GraphNode* node, sf::Color nodeColor)
{
	sf::CircleShape Circle(NODERADIUS);
	Circle.setPosition(node->x - NODERADIUS, node->y - NODERADIUS);
	Circle.setFillColor(sf::Color::White);
	Circle.setOutlineColor(nodeColor);
	Circle.setOutlineThickness(5);

	window.draw(Circle);

	sf::Text Label(std::to_string(node->i), font, 32);
	Label.setPosition(node->x - NODERADIUS / 2 + 5,
						node->y - NODERADIUS / 2 - 5);
	Label.setFillColor(sf::Color::Blue);

	window.draw(Label);
}

void SFMLGraphVisualize::drawEdge(GraphEdge* edge, sf::Color color, double weight)
{
	sf::Vector2f p(edge->first->x, edge->first->y);
	sf::Vector2f q(edge->second->x, edge->second->y);

	double angle = atan2((double)p.y - q.y, (double)p.x - q.x);

	sf::Vertex line[2] =
	{
			sf::Vertex(sf::Vector2f(p.x, p.y), color),
			sf::Vertex(sf::Vector2f(q.x, q.y), color)
	};

	window.draw(line, 2, sf::Lines);

	std::stringstream weightstring;
	weightstring << weight;
	sf::Text Labelweight(weightstring.str(), font, 32);
	int size = sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2));
	Labelweight.setPosition(p.x - (size / 2) * cos(angle) + 10 * sin(angle),
		p.y - (size / 2) * sin(angle) + 10 * cos(angle));
	Labelweight.setFillColor(sf::Color::Blue);
	window.draw(Labelweight);

}


int node_hovered(GraphNode* p_node, sf::Vector2i& pt)
{
	return ((p_node->x - pt.x) * (p_node->x - pt.x) + (p_node->y - pt.y) * (p_node->y - pt.y)) <= NODERADIUS * NODERADIUS;
}

void calcPath(int* parent, int j, std::vector<int>& out)
{
	if (parent[j] == -1)
		return;
	calcPath(parent, parent[j], out);
	out.push_back(j);
}

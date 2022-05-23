#include <Windows.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <limits.h>
#include <math.h>

//#define DEBUG_BOUNDS

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define MAX_GRAPH_NODES 10 //максимальное количество связей от одной ноды до других

// 
// класс описывающий ссылку на следующую ноду и длину пути до нее
// 
class graph_node;
class node_link {
	friend class graph_node;
public:
	node_link() {}
	node_link(graph_node *p_graph_from_link, graph_node *p_graph_node, int path_len) : p_from(p_graph_from_link), p_node(p_graph_node), path_length(path_len) {}
	~node_link() {}

	void set_node(graph_node *p_grphnode) { p_node = p_grphnode; }
	graph_node *get_node() { return p_node; }
	void set_path_length(int plength) { path_length = plength; }
	int get_path_length() { return path_length; }

	graph_node *p_from; //кто ссылается
	graph_node *p_node; //на кого ссылаемся
	int path_length; //длина пути до следующей ноды
};

template<typename T>
class matrix
{
	int n_cols;
	T *p_matrix_data;
public:
	void alloc_matrix(int n) {
		n_cols = n;
		if (p_matrix_data)
			free(p_matrix_data);

		p_matrix_data = (T *)malloc(n * n * sizeof(T));
		assert(p_matrix_data);
		memset(p_matrix_data, 0, n_cols * n_cols * sizeof(T));
	}

	void free_matrix() {
		if (p_matrix_data)
			free(p_matrix_data);
	}
	T &element(int r, int c) { return p_matrix_data[r * n_cols + c]; }
};

class node_graph_builder;

// 
// 
// 
class graph_node
{
	friend class node_graph_builder;
	int node_index;
	int num_of_links;
	node_link graph_node_links[MAX_GRAPH_NODES];
	char node_data[8];
public:
	POINT pt;
	graph_node() : num_of_links(0) {}
	~graph_node() {}

	int get_links_count() { return num_of_links; }
	int get_node_index() { return node_index; }

	// 
	// соединить ноду с указанием длины пути
	// указанная нода автоматически укажет на вас, по этому не требуется делать это повторно
	// 
	void link_node(graph_node *p_node, int path_length) {
		if (!find_link(p_node)) {
			assert(num_of_links != MAX_GRAPH_NODES);
			graph_node_links[num_of_links] = node_link(this, p_node, path_length);
			num_of_links++;
			return;
		}
		printf(__FUNCSIG__ " Node link already exists!\n");
	}

	// 
	// отсоединить ноду по адресу
	// 
	bool unlink_node(graph_node *p_node) {
		for (size_t i = 0; i < num_of_links; i++) {
			if (graph_node_links[i].get_node() == p_node) {

				/* shift links to left in links array */
				for (size_t j = i; j < num_of_links - 1; j++)
					graph_node_links[j] = graph_node_links[j + 1];

				num_of_links--;
				return true;
			}
		}
		return false;
	}

	// 
	// поиск ноды графа по адресу
	// 
	node_link *find_link(graph_node *p_node) {
		for (size_t i = 0; i < num_of_links; i++)
			if (graph_node_links[i].get_node() == p_node)
				return &graph_node_links[i];

		return NULL;
	}
};

// 
// основной класс графов
// 
class node_graph_builder
{
	matrix<int> graph_matrix;
	std::vector<graph_node *> m_alloc_nodes;
public:
	node_graph_builder() {};
	node_graph_builder(int n_nodes) { graph_matrix.alloc_matrix(n_nodes); };
	~node_graph_builder() {};

	size_t get_nodes_count() { return m_alloc_nodes.size(); }
	graph_node *get_node(const int i) { return m_alloc_nodes[i]; }

	matrix<int> &get_adjacency_matrix() { return graph_matrix; }

	graph_node *new_node(int x, int y) {
		graph_node *p_node = (graph_node *)calloc(1, sizeof(graph_node));
		p_node->pt.x = x;
		p_node->pt.y = y;
		m_alloc_nodes.push_back(p_node);
		p_node->node_index = m_alloc_nodes.size() - 1; //TODO: node index
		return p_node;
	}

	bool remove_node(graph_node *p_node) {
		for (size_t i = 0; i < m_alloc_nodes.size(); i++) {
			if (m_alloc_nodes[i] == p_node) {
				m_alloc_nodes.erase(m_alloc_nodes.begin() + i);
				return true;
			}
		}
		return false;
	}

	void link_nodes(graph_node *p_nodeA, graph_node *p_nodeB, int path_length)
	{
		p_nodeA->link_node(p_nodeB, path_length);
		p_nodeB->link_node(p_nodeA, path_length);
		graph_matrix.element(p_nodeA->node_index, p_nodeB->node_index) = path_length;
		graph_matrix.element(p_nodeB->node_index, p_nodeA->node_index) = path_length;
	}

	void scale_graph(float scale_factor) {
		graph_node *p_node;
		for (size_t i = 0; i < m_alloc_nodes.size(); i++) {
			p_node = m_alloc_nodes[i];
			p_node->pt.x *= scale_factor;
			p_node->pt.y *= scale_factor;
		}
	}

	void offset_graph(POINT offset) {
		graph_node *p_node;
		for (size_t i = 0; i < m_alloc_nodes.size(); i++) {
			p_node = m_alloc_nodes[i];
			p_node->pt.x += offset.x;
			p_node->pt.y += offset.y;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	int min_dist(int dist[], int num_of_nodes, bool sptSet[])
	{
		int min = INT_MAX, min_index;
		for (int v = 0; v < num_of_nodes; v++) {
			if (!sptSet[v] && dist[v] <= min) {
				min = dist[v];
				min_index = v;
			}
		}
		return min_index;
	}

	void printPath(int *parent, int j)
	{
		// Base Case : If j is source
		if (parent[j] == -1)
			return;

		printPath(parent, parent[j]);
		printf("%d ", j);
	}

	// A utility function to print the constructed distance
	void printSolution(int *dist, int n, int *parent, int src)
	{
		printf("Vertex\t Distance\tPath");
		for (int i = 1; i < n; i++) {
			printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
			printPath(parent, i);
		}
	}

	//TODO: begin node index 2
	void dijkstra(int src) {
		int number_of_nodes = m_alloc_nodes.size();
		assert(src >= 0 && src < number_of_nodes);

		int *dist = (int *)calloc(number_of_nodes, sizeof(int));
		//assert(dist);
		bool *sptSet = (bool *)calloc(number_of_nodes, sizeof(bool));
		//assert(sptSet);
		int *parent = (int *)calloc(number_of_nodes, sizeof(int));
		//assert(parent);
		for (int i = 0; i < number_of_nodes; i++)
			dist[i] = INT_MAX;

		dist[src] = 0;
		parent[src] = -1;

		//TODO: nodes must be linked!!!
		for (int count = 0; count < number_of_nodes - 1; count++) {
			int u = min_dist(dist, number_of_nodes, sptSet);
			sptSet[u] = true;
			for (int v = 0; v < number_of_nodes; v++) {
				if (!sptSet[v] && graph_matrix.element(u, v) && dist[u] + graph_matrix.element(u, v) < dist[v]) {
					parent[v] = u;
					dist[v] = dist[u] + graph_matrix.element(u, v);
				}
			}
		}
		printSolution(dist, number_of_nodes, parent, src);

		free(dist);
		free(sptSet);
		free(parent);
	}
};

node_graph_builder grph_build(6);

//
// построить граф
// 
void build_graph(node_graph_builder &gb)
{
	graph_node *p_graph_node1 = gb.new_node(45, 134);
	graph_node *p_graph_node2 = gb.new_node(345, 300);
	graph_node *p_graph_node3 = gb.new_node(724, 120);
	graph_node *p_graph_node4 = gb.new_node(465, 45);
	graph_node *p_graph_node5 = gb.new_node(812, 315);
	graph_node *p_graph_node6 = gb.new_node(690, 465);
	gb.link_nodes(p_graph_node1, p_graph_node2, 5); //1-2 len: 5
	gb.link_nodes(p_graph_node1, p_graph_node4, 16); //1-5 len: 16
	gb.link_nodes(p_graph_node2, p_graph_node3, 10); //2-3 len: 10
	gb.link_nodes(p_graph_node3, p_graph_node4, 14); //3-4 len: 14
	gb.link_nodes(p_graph_node3, p_graph_node5, 20); //3-5 len: 20
	gb.link_nodes(p_graph_node5, p_graph_node6, 9); //5-6 len: 9
	gb.link_nodes(p_graph_node6, p_graph_node2, 7); //6-2 len 7
}

void draw_graph_node(HDC hdc, POINT &pt, COLORREF color, INT radius, INT node_index, HBRUSH background)
{
	COLORREF textcolor = GetTextColor(hdc);
	SetTextColor(hdc, color);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, background);

	RECT rect;
	rect.left = pt.x - radius;
	rect.top = pt.y - radius;
	rect.right = pt.x + radius;
	rect.bottom = pt.y + radius;
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

	char buffer[64];
	sprintf_s(buffer, sizeof(buffer), "%d", node_index + 1);
	TextOutA(hdc, pt.x, pt.y, buffer, strlen(buffer));
	SetTextColor(hdc, textcolor);
	SelectObject(hdc, oldbrush);
}

void draw_line(HDC hdc, POINT &from, POINT &to, HBRUSH color)
{
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, color);
	MoveToEx(hdc, from.x, from.y, NULL);
	LineTo(hdc, to.x, to.y);
	SelectObject(hdc, oldbrush);
}

void compute_line_text_pos(HDC hdc, POINT *p_dst, POINT &line_from, POINT &line_to)
{
	RECT rect;
	rect.left = min(line_from.x, line_to.x);
	rect.top = min(line_from.y, line_to.y);
	rect.right = max(line_from.x, line_to.x);
	rect.bottom = max(line_from.y, line_to.y);
#ifdef DEBUG_BOUNDS
	HBRUSH brush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, brush);
	SetPixel(hdc, p_dst->x, p_dst->y, RGB(255, 0, 0));
#endif
	LONG half_x = abs(rect.right - rect.left) >> 1;
	LONG half_y = abs(rect.bottom - rect.top) >> 1;
	p_dst->x = rect.right - half_x;
	p_dst->y = rect.bottom - half_y;
}

void draw_link(HDC hdc, POINT &from, POINT &to, INT bias, HBRUSH color)
{
	POINT txtpos;
	draw_line(hdc, from, to, color);
	compute_line_text_pos(hdc, &txtpos, from, to);

	char buffer[64];
	sprintf_s(buffer, sizeof(buffer), "%d", bias);
	TextOutA(hdc, txtpos.x, txtpos.y, buffer, strlen(buffer));
}

// 
// нарисовать граф
// 
void draw_graph(HDC hdc, node_graph_builder &graph_builder)
{
	//отрисовать пути
	graph_node *p_from, *p_to;
	HBRUSH line_brush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	matrix<int> &adj_matrix = graph_builder.get_adjacency_matrix();
	for (size_t i = 0; i < graph_builder.get_nodes_count(); i++) {
		for (size_t j = 0; j < graph_builder.get_nodes_count(); j++) {
			int bias = adj_matrix.element(i, j);
			if (!bias || i == j) //TODO: i == j
				continue;

			p_from = graph_builder.get_node(i); //current node
			assert(p_from);
			p_to = graph_builder.get_node(j); //child node
			assert(p_from);
			draw_link(hdc, p_from->pt, p_to->pt, bias, line_brush);
		}
	}

	//отрисовать ноды
	for (size_t i = 0; i < graph_builder.get_nodes_count(); i++) {
		if ((p_from = graph_builder.get_node(i))) {
			draw_graph_node(hdc, p_from->pt, RGB(0, 0, 0), 30, i, line_brush);
		}
	}
}

// 
// процедура обработчик оконных сообщений
// 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	LRESULT res = 0;
	static HDC mem_dc;
	static HBITMAP mem_bitmap;
	//static BOOL mouse_clicked = FALSE;
	//static POINT mouse, first_click;
	switch (message) {

	case WM_CREATE:
		HDC dc;
		dc = GetDC(hWnd);
		mem_dc = CreateCompatibleDC(dc);
		GetClientRect(hWnd, &rect);
		mem_bitmap = CreateCompatibleBitmap(dc, rect.right, rect.bottom);
		SelectObject(mem_dc, mem_bitmap);
		SelectObject(mem_dc, GetStockObject(ANSI_VAR_FONT));
		SetBkMode(mem_dc, TRANSPARENT);
		ReleaseDC(hWnd, dc);
		break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		FillRect(mem_dc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		draw_graph(mem_dc, grph_build);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, mem_dc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	}

	//case WM_LBUTTONDOWN:
	//	mouse.x = first_click.x = LOWORD(lParam);
	//	mouse.y = first_click.y = HIWORD(lParam);
	//	printf("first_click %d %d\n", first_click.x, first_click.y);
	//	mouse_clicked = TRUE;
	//	break;

	//case WM_LBUTTONUP:
	//	mouse_clicked = FALSE;
	//	break;

	//case WM_MOUSEMOVE: {
	//	if (mouse_clicked) {
	//		mouse.x = LOWORD(lParam);
	//		mouse.y = HIWORD(lParam);
	//		grph_build.offset_graph(mouse);
	//		InvalidateRect(hWnd, NULL, FALSE);
	//	}
	//	break;
	//}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		res = DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return res;
}

int main()
{
	WNDCLASSEXA wc;
	memset(&wc, NULL, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandleA(0);
	wc.lpszClassName = "GraphView";
	wc.lpfnWndProc = WndProc;
	if (!RegisterClassExA(&wc))
		return 1;

	//graph options
	build_graph(grph_build);
	grph_build.dijkstra(2);
	grph_build.scale_graph(0.8f);
	grph_build.offset_graph({ 50, 100 });

	INT x = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (WINDOW_WIDTH >> 1);
	INT y = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (WINDOW_HEIGHT >> 1);
	HWND hWnd = CreateWindowExA(0, wc.lpszClassName, "Graph Viewer", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, (HMENU)NULL, NULL, NULL);
	if (!hWnd)
		return 2;

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	while (GetMessageA(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	DestroyWindow(hWnd);
	UnregisterClassA(wc.lpszClassName, wc.hInstance);
	return 0;
}
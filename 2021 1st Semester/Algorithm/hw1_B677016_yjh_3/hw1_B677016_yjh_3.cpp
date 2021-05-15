//Referenced the source code adj_list.cpp in the data structure textbook sources
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
typedef struct GraphNode
{
	int vertex;
	int weight;
	struct GraphNode* link;
} GraphNode;

typedef struct GraphType {
	int n;	// ������ ����
	GraphNode* adj_list[MAX_VERTICES];
} GraphType;

// �׷��� �ʱ�ȭ 
void init(GraphType* g)
{
	int v;
	g->n = 0;
	for (v = 0; v < MAX_VERTICES; v++)
		g->adj_list[v] = NULL;
}

// ���� ���� ����
void insert_vertex(GraphType* g, int v)
{
	if (((g->n) + 1) > MAX_VERTICES) {
		fprintf(stderr, "�׷���: ������ ���� �ʰ�");
		return;
	}
	g->n++;
}

// ���� ���� ����, v�� u�� ���� ����Ʈ�� w��ŭ �����Ѵ�.
void insert_edge(GraphType* g, int u, int v, int w)
{
	GraphNode* node;
	if (u >= g->n || v >= g->n) {
		fprintf(stderr, "�׷���: ���� ��ȣ ����");
		return;
	}
	node = (GraphNode*)malloc(sizeof(GraphNode));
	node->vertex = v;
	node->weight = w;
	node->link = g->adj_list[u];
	g->adj_list[u] = node;
}

void print_graph_degree(GraphType* g)
{
	int in_degree[6] = { 0 };
	int out_degree[6] = { 0 };
	int max_in_degree= 0;
	int max_out_degree = 0;
	int min_in_degree = 0;
	int min_out_degree = 0;

	for (int i = 0; i < g->n; i++) {
		GraphNode* p = g->adj_list[i];
		while (p != NULL) {
			out_degree[i]++;
			in_degree[p->vertex]++;
			p = p->link;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		printf("���� [%d]�� �������� : %d\n���� [%d]�� �������� : %d\n", i, in_degree[i], i, out_degree[i]);
		if (in_degree[i] > in_degree[max_in_degree])
		{
			max_in_degree = i;
		}
		if (in_degree[i] < in_degree[min_in_degree])
		{
			min_in_degree = i;
		}
		if (out_degree[i] > out_degree[max_out_degree])
		{
			max_out_degree = i;
		}
		if (out_degree[i] < out_degree[min_out_degree])
		{
			min_out_degree = i;
		}
	}
	printf("���������� ���� ���� ���� : %d\n���� : %d\n",min_in_degree ,in_degree[min_in_degree]);
	printf("���������� ���� ū ���� : %d\n���� : %d\n", max_in_degree, in_degree[max_in_degree]);
	printf("���������� ���� ���� ���� : %d\n���� : %d\n", min_out_degree, out_degree[min_out_degree]);
	printf("���������� ���� ū ���� : %d\n���� : %d\n", max_out_degree, out_degree[max_out_degree]);
}

int main()
{
	GraphType* g;
	g = (GraphType*)malloc(sizeof(GraphType));
	init(g);
	for (int i = 0; i < 6; i++)
		insert_vertex(g, i);

	insert_edge(g, 0, 1, 50);
	insert_edge(g, 0, 2, 45);
	insert_edge(g, 0, 3, 10);
	insert_edge(g, 1, 2, 10);
	insert_edge(g, 1, 3, 15);
	insert_edge(g, 2, 4, 30);
	insert_edge(g, 3, 0, 20);
	insert_edge(g, 3, 4, 15);
	insert_edge(g, 4, 1, 20);
	insert_edge(g, 4, 2, 35);
	insert_edge(g, 5, 4, 3);

	print_graph_degree(g);

	free(g);
	return 0;
}
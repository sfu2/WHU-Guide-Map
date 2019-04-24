#ifndef 武大地图_H_INCLUDED
#define 武大地图_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#define INF 32767
#define MAX 50

using namespace std;

//邻接表的构造 
typedef struct route {
	int tag;
	char infor[30];
	int weight;
	route *next;
}Route;

typedef struct node {
	char name[20];
	int tag;
	char infor[100];
	Route *h;
}Pnode;

typedef struct {
	Pnode *phead[50];
	int node;
}Map;
//邻接矩阵的构造

typedef struct Mat {
	int edges[50][50];
	int node;
	int route;
} Graph;
//根据邻接矩阵的 

bool FileToNode(Pnode **phead, int m);//将景点信息（名字，编号，介绍，路径名称），读入图中

void PrintNode(Map *G, int m);//输入访问的节点编号，输出节点信息

void InitialMap(Map *& G);
void CreatMap(Map *& G);
void CreateNextNode(Route *& h, vector<int> buffer);
void MatixToTable(Map * G, int A[MAX][MAX], int node);
//建立邻接矩阵表示的图 

int Dijkstra(Graph *G, vector<int> input, vector<int> &output, int apath[]);
void FindPath(Map* G, vector<int> input, vector<int> &output);
//最短路径算法，apath数组储存最短路径编号，v,m代表起止点，函数返回最短路径长度 


#endif // 武大地图_H_INCLUDED
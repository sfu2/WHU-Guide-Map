#ifndef ANT_MAP_H_INCLUDED
#define ANT_MAP_H_INCLUDED

/*
搜索策略
1）价值较高的点优先度极高
2）低价值点有概率进入；
3）循环搜索多次，求期望函数，取其最大值
4）多度点可以多次进入
5）当邻接的路无法进入时停止搜索
*/
#include"GUI_MAP.h"
#include"iostream"
#include "math.h"
#include "time.h"
#define N_IT_COUNT 2000
//搜索200次
#define SPOT_NUM 24
using namespace std;

typedef struct knot {
	int tag_num;
	int spotvalue;
	//int studyvalue;
}Spot_knot;

typedef struct point {
	Spot_knot phead[SPOT_NUM];
}Point;

void find_Path(Map* map, int StartSpotNum, vector<int> &output);
/*
int weight_study[SPOT_NUM] = {
+5,0,0,+5,+10,
0,+20,+10,+20,+10,
+20,0,+10,+20,0,
0,0,+5,+5,+20,
+20,+10,+10,+10
};
*/
double rand_double(double dbLow, double dbUpper);
class Find
{
public:
	Find(void);
	~Find(void);
public:
	vector<int> m_nPath;//记录走过的路径
	int m_dbPathLength;//记录走过的路径长度
	int m_nCurSpotNum;//现在景点的编号
	int m_nMovedSpotCount;//已经去过的景点数目
	Map *WHUmap;//武大地图
	int Start;//出发点
	Point WHU_point;
	double Path_Weight;
	double worth;
public:
	int degree_Num(Pnode *p);//计算结点的度
	void Init(Map *map, int start);//初始化
	int ChooseNextSpot();//选择下一个景点
	bool Move();//在景点间移动
	void Search(Map *map, int start);//搜索路径
	void CalPathLength();//计算走过的路径长度
	void WeightPath();
	double Distance(int n, int m);
};

class CTsp
{
public:
	CTsp(void);
	~CTsp(void);
public:
	Find m_cAry[N_IT_COUNT]; //搜索数组
	Find m_cBest; //该不参与搜索，只是用来保存最优结果
	Map *map;
	int Start;
public:
	//初始化数据
	void InitData(Map *antmap, int start);
	//开始搜索
	void Search();
};

#endif
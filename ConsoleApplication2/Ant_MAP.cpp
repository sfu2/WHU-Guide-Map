#include "Ant_MAP.h"
int weight_spot[SPOT_NUM] = {
+40,0,0,+40,0,
0,+100,0,+10,+50,
+50,+50,+50,0,5,
0,0,+20,+20,0,
0,+50,0,0
};
Find::Find(void)
{
}
Find::~Find(void)
{
}
CTsp::CTsp(void)
{
}
CTsp::~CTsp(void)
{
}

//返回浮点数四舍五入取整后的浮点数
double rand_double(double dbLow, double dbUpper)
{
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp * (dbUpper - dbLow);
}

//返回结点的度
int Find::degree_Num(Pnode *p) {
	int num = 0;
	Route* road = NULL;
	road = p->h;
	for (; road->next != NULL; num++) {
		road = road->next;
	}
	return num+1;
}

//初始化
void Find::Init(Map *map, int start) {
	WHUmap = map;
	Start = start;
	m_nPath.clear();
	m_dbPathLength = 0.0;
	m_nCurSpotNum = Start;
	m_nMovedSpotCount = 0;
	Path_Weight = 0.0;
	worth = 0.0;
		for (int num = 1; num <= SPOT_NUM; num++) {

			int tempi = degree_Num(WHUmap->phead[num]);
			if (5 > tempi > 2)
				WHU_point.phead[num - 1].tag_num = tempi - 2;//大于等于2的度只能走n-1次
			else
				if (tempi > 4)
					WHU_point.phead[num - 1].tag_num = tempi - 3;
				else
		            WHU_point.phead[num-1].tag_num = 1;

		WHU_point.phead[num-1].spotvalue = weight_spot[num-1];
		//WHU_point.phead[num].studyvalue = weight_study[num];
		}
		WHU_point.phead[Start - 1].tag_num--;
}

int Find::ChooseNextSpot()
{
	int nSelectedSpot = -1; //返回结果，先暂时把其设置为-1
	//==============================================================================
	//计算当前景区和没去过的景区之间的信息素总和

	double dbTotal = 0.0;

	typedef struct stru {
		int tag;
		double probility;
	};
	stru c;
	vector<stru> prob;
	prob.clear();

	Route *p = WHUmap->phead[m_nCurSpotNum]->h;

	int i;
	for (; p != NULL; p = p->next) {
		i = p->tag;
		if (WHU_point.phead[i-1].tag_num)
		{
			c.probility = 0;
			c.tag = i;
			c.probility = (WHU_point.phead[i-1].spotvalue + 10)*10/ Distance(m_nCurSpotNum,i );//该函数可更改
			prob.push_back(c);
			dbTotal += c.probility;
		}//只有邻接并且访问次数没用完的结点可以访问
		else {
	;
}
	}
	
	//进行轮盘选择
	double dbTemp = 0.0;
	dbTemp = rand_double( 0, dbTotal);//取一个随机数
	if (dbTotal > 0.0)//总信息素大于0
	{
		
		for (int i = 0; i < prob.size(); i++)
		{
			dbTemp = dbTemp - prob[i].probility;
			if (dbTemp < 0.0)
			{
				nSelectedSpot = prob[i].tag;
				break;
			}
		}
	}
	//如果出现nSelectedSpot==-1,表明无路可走，此时停下来
	return nSelectedSpot;
}

bool Find::Move()
{
	int nSpotNo = ChooseNextSpot(); //选择下一个景点
	if (nSpotNo == -1) return false;
		m_nPath.push_back(nSpotNo); //保存走的路径
	WHU_point.phead[nSpotNo-1].spotvalue = 0;
	WHU_point.phead[nSpotNo-1].tag_num--;//把这个景点的可访问次数减一
	m_nCurSpotNum = nSpotNo; //改变当前所在景点为选择的景点
	m_nMovedSpotCount++; //已经去过的景点数量加1
	return true;
}

void Find::Search(Map *map, int start)
{
	Init(map,start);

	m_nPath.push_back(Start);
	while (1) {
		if (!Move())
			break;
	}
	CalPathLength();
	WeightPath();
	worth = Path_Weight*10 / m_dbPathLength;
}

void Find::CalPathLength()
{
	m_dbPathLength = 0.0; //先把路径长度置0
	int m = 0;
	int n = 0;
	for (int i = 1; i < m_nPath.size(); i++)
	{
		m = m_nPath[i];
		n = m_nPath[i - 1];
		m_dbPathLength = m_dbPathLength + Distance(m, n);
	}
}

void Find::WeightPath()
{
	Path_Weight = 0.0;
	int m = 0;
	for (int i = 0; i < m_nPath.size(); i++)
	{
		m = m_nPath[i];
		Path_Weight += WHU_point.phead[m].spotvalue;
	}
}

//返回两点间的距离
double Find::Distance(int n, int m)
{
	if (!(WHUmap&&WHUmap->phead[n] && WHUmap->phead[n]->h))
	{
		return -1.0;
	}

	Route *p = WHUmap->phead[n]->h;

	do
	{
		if (p->tag == m )
		{
			return (double)p->weight;
		}
		p = p->next;
	} while (p != NULL);
	return 0;
}

void CTsp::InitData(Map* antmap,int start)
{
	//先把最优的路径期望设置为0
	m_cBest.worth = 0.0;
	map = antmap;
	Start = start;
}

void CTsp::Search()
{
	//在迭代次数内进行循环
		//每只蚂蚁搜索一遍
	for (int j = 0; j < N_IT_COUNT; j++)
	{
		m_cAry[j].Search(map,Start);
	}
	//保存最佳结果
	for (int j = 0; j < N_IT_COUNT; j++)
	{
		if (m_cAry[j].m_nMovedSpotCount >= 12)
		{
			if (m_cAry[j].worth > m_cBest.worth)
			{
				m_cBest = m_cAry[j];
			}
		}
	}
}

void find_Path(Map* map, int StartSpotNum, vector<int> &output)
{
	//用当前时间点初始化随机种子，防止每次运行的结果都相同
	time_t tm;
	time(&tm);
	unsigned int nSeed = (unsigned int)tm;
	srand(nSeed);
	//开始搜索
	CTsp tsp;
	tsp.InitData(map, StartSpotNum); //初始化
	tsp.Search(); //开始搜索
	//输出结果
	for (int i = 0; i < tsp.m_cBest.m_nPath.size(); i++)
		output.push_back(tsp.m_cBest.m_nPath[i]);
}
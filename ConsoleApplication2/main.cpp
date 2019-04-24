#include"GUI_MAP.h"
#include"Ant_MAP.h"

using namespace std;

void mainloop() {
	Map *graph;
	vector<Site> site;
	vector<int> input, output;

	InitialMap(graph);
	CreatMap(graph);
	initSite(site);
	initComponent();
	setcaption("武汉大学校园导航");

    for( ; is_run(); delay_fps(60)){
		getSite(input, site);
		showSite(site, graph);
		showSearchPanel();
		//showAllPath();

		/* YOU MAY CHANGE CODE BELOW  */
		// getSite(vector<int> &input, vector<Site> site, int site_cnt)
		// 该函数用于读取用户按下Ctrl后对节点的点击；
		// 节点的编号存于input，site参数不要改动，site_cnt为需要读取节点的数量
		// connectPath(vector<int> output, vector<Site> site)
		// output中为需要被高亮显示的路径（节点必须有路径相连），site参数不要改动
		
		if (!input.empty()) {
			output.clear();
			if (input.size() == 2)// 最短路径
				FindPath(graph, input, output);
			else // 推荐路径
				find_Path(graph, input[0], output);
			connectPath(output, graph);
		}
		/* YOU MAY CHANGE CODE ABOVE  */
    }
	delComponent();
}

int main() {
	setinitmode(0);
	initgraph(MAP_LENGTH, MAP_WIDTH);   // 图形初始化，窗口尺寸
	setrendermode(RENDER_MANUAL);		// 绘图更新模式设置为手动，避免闪烁
    mainloop();
	return 0;
}

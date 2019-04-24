#include"GUI_MAP.h"

static bool ctrl_hit = 0, shift_hit = 0;
static PIMAGE mask, focus_mask, bkground, text_bkground;

using namespace std;

// delete gui component
void delComponent() {
	delimage(mask);
	delimage(focus_mask);
	delimage(bkground);
	delimage(text_bkground);
}

// show all available paths
void showAllPath() {
	FILE *fp;
	int scanNum_1, scanNum_2;
	int line_x1, line_y1, line_x2, line_y2;
	char skipline[SKIPLINE_SIZE];

	fopen_s(&fp, "DependentFiles\\path.txt", "rt");
	while (!feof(fp)) {
		if (fscanf_s(fp, "%d %d", &scanNum_1, &scanNum_2)) {
			fscanf_s(fp, "%d %d %d %d",
				&line_x1, &line_y1, &line_x2, &line_y2);
			setcolor(GREEN);
			setlinestyle(PS_DASH, NULL, 3);
			do {
				line(line_x1, line_y1, line_x2, line_y2);
				line_x1 = line_x2;
				line_y1 = line_y2;
				fscanf_s(fp, "%d", &line_x2);
				if (line_x2 == -1)
					break;
				fscanf_s(fp, "%d", &line_y2);
			} while (true);
		} else
			fgets(skipline, SKIPLINE_SIZE, fp);
	}
	fclose(fp);
}

// get site_num when ctrl or shift is hit
void getSite(vector<int> &input, vector<Site> site) {
	int site_num, site_cnt = 0;
	mouse_msg m_msg = { 0 };
	input.clear();
	flushmouse();
	ctrl_hit = shift_hit = 0;
	if (KEY_DOWN(VK_CONTROL)) {
		site_cnt = 2;
		ctrl_hit = 1;
	} else if (KEY_DOWN(VK_SHIFT)) {
		site_cnt = 1;
		shift_hit = 1;
	} else
		return;
	putimage(0, 0, bkground);
	ege::outtextxy(0, 20, "ctrl hit!\n");
	showSite(site, NULL);
	do {
		m_msg = getmouse();
		if (KEY_DOWN(VK_CONTROL) || KEY_DOWN(VK_SHIFT)) {
			if (m_msg.is_up()) {
				ege::outtextxy(0, 40, "mouse hit!\n");
				if (site_num = isSite(site, m_msg.x, m_msg.y)) {
					input.push_back(site_num);
					site_cnt--;
				}
			}
		} else {
				ctrl_hit = shift_hit = 0;
				input.clear();
				break;
		}
	} while (site_cnt);
	if (!site_cnt) {
		char str[30];
		sprintf(str, "Get: %d", input[0]);
		outtextxy(0, 60, str);
		getch();
	}
}

// connect path given by FindPath() or fing_path()
void connectPath(vector<int> output, Map * G) {
    FILE *fp;
	Route *route;
	vector<int> pathline, tmp;
	string path(ctrl_hit ? "最短路径：" : "推荐路线：");
	char skipline[SKIPLINE_SIZE];
	int scanNum_1, scanNum_2, distance = 0;
	int line_x1, line_y1, line_x2, line_y2, site_1, site_2;

	path.append(G->phead[output[0]]->name);
	for (int i = 1; i < (int)output.size();i++) {
		site_1 = output[i-1];
		site_2 = output[i];
		route= getPath(G, site_1, site_2);
		distance += route->weight;
		sprintf(skipline, "--%s-->%s", route->infor, G->phead[site_2]->name);
		path.append(skipline);
		fopen_s(&fp, "DependentFiles\\path.txt", "rt");
		while (!feof(fp)) {
			if (fscanf_s(fp, "%d %d", &scanNum_1, &scanNum_2)
				&& (scanNum_1 == site_1&&scanNum_2 == site_2
				|| scanNum_1 == site_2&&scanNum_2 == site_1)) {
				fscanf_s(fp, "%d %d %d %d",
					&line_x1, &line_y1, &line_x2, &line_y2);
				pathline.push_back(PATH_START);
				tmp.clear();
				do {
					tmp.push_back(line_x1);
					tmp.push_back(line_y1);
					tmp.push_back(line_x2);
					tmp.push_back(line_y2);
					line_x1 = line_x2;
					line_y1 = line_y2;
					fscanf_s(fp, "%d", &line_x2);
					if (line_x2 == -1)
						break;
					fscanf_s(fp, "%d", &line_y2);
				} while (true);
				if (site_1 == scanNum_2) {
					for (int j = 1; j < (int)tmp.size()/2; j += 2) {
						SWAP(tmp[j-1], tmp[tmp.size()-j-1]);
						SWAP(tmp[j], tmp[tmp.size()-j]);
					}
				}
				pathline.insert(pathline.end(), tmp.begin(), tmp.end());
				break; // end if scanNum == site
			} else
				fgets(skipline, SKIPLINE_SIZE, fp);
		}// end while feof fp
		fclose(fp);
	}// end for(int i = 1; i < (int)output.size();i++)
	showTextPanel(G, 0, path.c_str());
	drawLine(pathline, output, G);
	sprintf_s(skipline, "总距离：%dm", distance);
	ege::outtextxy(*(pathline.end()-2) - (site_2 == 19 ? 30 : 0), *(pathline.end()-1), skipline);
	ege::getch();
}

// draw path
void drawLine(vector<int> path, vector<int> output, Map *G){
	int distance, k = 0, i;
	int line_x1, line_y1, line_x2, line_y2;
	setcolor(GREEN);
	setlinestyle(PS_DASH, NULL, 3);
	for (i = 0; i < (int)path.size(); i += 4) {
		if (path[i] == PATH_START) {
			rectprintf(path[i+1], path[i+2]-20, path[i+1], path[i+2], "%s", G->phead[output[k++]]->name);
			i++;
		}
		line_x1 = path[i];
		line_y1 = path[i + 1];
		line_x2 = path[i + 2];
		line_y2 = path[i + 3];
		distance = LENGTH(line_x1, line_y1, line_x2, line_y2)/ DRAW_SPEED;
		if (!distance)
			distance = 1;
		for (int step = 1; step <= distance; delay_fps(60), step++)
			line(line_x1, line_y1, 
				line_x1 + (line_x2 - line_x1)*step/distance,
				line_y1 + (line_y2 - line_y1)*step/distance);
	}
	rectprintf(path[i -2], path[i -1] - 20, path[i - 2], path[i -1], "%s", G->phead[output[k]]->name);
}

void initTransparentMask(PIMAGE &mask, int mask_size){
    setcolor(WHITE);
	setfillcolor(WHITE);
	fillellipse(mask_size, mask_size, mask_size, mask_size);
	mask = newimage();
	getimage(mask, 0, 0, 2* mask_size, 2* mask_size);
	cleardevice();
}

// show text panel and undate graphics
void showTextPanel(Map * graph, int focus_site, const char *textString) {
	LOGFONTA f;
	PIMAGE text;
	int x, y, textWidth = TEXT_WIDTH, textHeight = TEXT_HEIGHT;
	cleardevice();
	// set font
	setbkcolor(WHITE);
	setcolor(BLACK);
	getfont(&f);
	strcpy(f.lfFaceName, "楷体");
	f.lfQuality = PROOF_QUALITY;
	f.lfHeight = 18;
	setfont(&f);
	// print mouse position
	mousepos(&x, &y);
	rectprintf(MAP_LENGTH - 185, 10, MAP_LENGTH - 100, 25, "当前鼠标坐标：(%3d,%3d)", x, y);
	// print upper-left info
	if (textString)
		rectprintf(0, 0, textWidth = (int)(TEXT_WIDTH * 1.5), textHeight = (int)(TEXT_HEIGHT * 1.3),
			"%s", textString);
	else if (focus_site && graph)
		rectprintf(0, 0, TEXT_WIDTH, TEXT_HEIGHT,
			"%s %d\n\n%s", graph->phead[focus_site]->name, 
			graph->phead[focus_site]->tag, graph->phead[focus_site]->infor);
	else
		rectprintf(0, 0, textWidth = (int)(TEXT_WIDTH * 2.2), textHeight = (int)(TEXT_HEIGHT * 1),
			"            武汉大学校园导览系统操作提示\n\n"
			"1.将鼠标移至半透明圆圈处即可显示地点名称、代号、介绍等信息\n"
			"2.按下'S'即可弹出地点搜索框并返回所查询地点坐标\n"
			"3.按住Ctrl的同时点击两地点即可显示两地点间的最短路径及距离\n"
			"4.按住Shift的同时点击一地点即可显示为您推荐的游览路线");
	text = newimage();
	getimage(text, 0, 0, MAP_LENGTH, MAP_WIDTH);
	// put bkground to cover whatever is behind it
	putimage(0, 0, bkground);
	// display upper-left info and mouse position
	putimage_alphablend(NULL, text_bkground, 0, 0, 0xA0, 0, 0, textWidth, textHeight);
	putimage_alphatransparent(NULL, text, 0, 0, WHITE, 0xFF);
	delimage(text);
}

// show site with mask
// NOTE: graphics updates included
void showSite(vector<Site> &site, Map* graph){
	int focus_site = isSite(site);
	bool isFocus = focus_site >= 1;
	showTextPanel(graph, focus_site);
	if (isFocus && !(ctrl_hit || shift_hit))
		showConnectSite(site, graph, focus_site);
	for (int i = 0; i < (int)site.size(); i++) {
		if (!isFocus)
			site[i].isFocus = 0;
		if (site[i].isFocus)
			putimage_alphatransparent(NULL, focus_mask,
				site[i].x - FOCUSMASK_SIZE, site[i].y - FOCUSMASK_SIZE, BLACK, 0xA0);
		else
			putimage_alphatransparent(NULL, mask,
				site[i].x - MASK_SIZE, site[i].y - MASK_SIZE, BLACK, 0xA0);
	}
}

void initTextBackground(PIMAGE &text_bkground) {
	text_bkground = newimage();
	setbkcolor(WHITE);
	getimage(text_bkground, 0, 0, MAP_LENGTH, MAP_WIDTH);
	cleardevice();
}

// get path between two sites
Route* getPath(Map * G, int site_1, int site_2) {
	Route *p;
	p = G->phead[site_1]->h;
	while (p && p->tag != site_2)
		p = p->next;
	return p;
}

// show sites directly-connected to focused site
void showConnectSite(vector<Site> &site, Map *G, int site_num) {
	FILE *fp;
	char skipline[SKIPLINE_SIZE];
	int scanNum_1, scanNum_2, otherSite;
	int line_x1, line_y1, line_x2, line_y2;

	fopen_s(&fp, "DependentFiles\\path.txt", "rt");
	while (!feof(fp)) {
		if (fscanf_s(fp, "%d %d", &scanNum_1, &scanNum_2)
			&& (scanNum_1 == site_num || scanNum_2==site_num)) {
			site[scanNum_1 - 1].isFocus = site[scanNum_2 - 1].isFocus = 1;
			fscanf_s(fp, "%d %d %d %d",
				&line_x1, &line_y1, &line_x2, &line_y2);
			setcolor(GREEN);
			setlinestyle(PS_DASH, NULL, 3);
			do {
				line(line_x1, line_y1, line_x2, line_y2);
				line_x1 = line_x2;
				line_y1 = line_y2;
				fscanf_s(fp, "%d", &line_x2);
				if (line_x2 == -1)
					break;
				fscanf_s(fp, "%d", &line_y2);
			} while (true);
			sprintf_s(skipline, "距离：%dm", getPath(G, scanNum_1, scanNum_2)->weight);
			otherSite = (scanNum_1 == site_num) ? scanNum_2 : scanNum_1;
			ege::outtextxy(site[otherSite - 1].x - (otherSite == 19 ? 30 : 0),
				site[otherSite - 1].y, skipline);
		} else
			fgets(skipline, SKIPLINE_SIZE, fp);
	} // end while feof fp
	fclose(fp);
}

// return the site mouse (x, y) is at
int isSite(vector<Site> site, int x, int y){
	int focus_site = 0;
	if(x==NOT_GIVEN)
		mousepos(&x, &y);
    for(int i=0; i<(int)site.size(); i++){
        if(abs(site[i].x-x)<15 && abs(site[i].y-y)<15)
            focus_site=i+1;
    }
    return focus_site;
}

// load site.txt into vector site
void initSite(vector<Site> &site){
    FILE *fp;
    Site tmp;
    fopen_s(&fp, "DependentFiles\\site.txt", "rt");
    while(!feof(fp)){
		fscanf_s(fp, "%d %d %s %d", &tmp.x, &tmp.y, tmp.name, 20, &tmp.num);
		tmp.isFocus = 0;
        site.push_back(tmp);
    }
}

// get search panel
void showSearchPanel() {
	char key;
	if (kbhit()) {
		key = getch();
		if (key=='s'||key=='S')
			ShellExecute(NULL, L"open", L"SearchForSite.exe", 
				NULL, L"DependentFiles", SW_SHOWNORMAL);
	}
}

// insert <ch> into <where> of the <str>
void insert(char *str, int where, char ch) {
	for (int i = strlen(str) + 1; i != where; i--)
		str[i] = str[i - 1];
	str[where] = ch;
}

// initiate gui component
void initComponent() {
	initBackground(bkground);
	initTransparentMask(mask, MASK_SIZE);
	initTransparentMask(focus_mask, FOCUSMASK_SIZE);
	initTextBackground(text_bkground);
}

void initBackground(PIMAGE &img){
    char filePath[120];
    _getcwd(filePath, 120);
    for(int i=0; i<(int)strlen(filePath);i++){
        if(filePath[i]=='\\')
            insert(filePath, ++i, '\\');
    }
    strcat_s(filePath, 120, "\\\\DependentFiles\\\\map.png");
    img=newimage();
    getimage(img, filePath, 0, 0);
    resizeImg(img, MAP_LENGTH, MAP_WIDTH);
}

/*  Author: stophin@BaiduTieba
 *  Site:   http://tieba.baidu.com/p/5561082957
 */
void resizeImg(PIMAGE &img, int pwidth, int pheight) {
    int width,height;
    //Resize image to pwidth*pheight
    PIMAGE backproc, save=gettarget();
    //Get image size
    settarget(img);
    width=getwidth();
    height=getheight();
    settarget(save);

    backproc=newimage(pwidth, pheight);
    //Resize
    putimage(backproc, 0, 0, pwidth, pheight, img, 0, 0, width, height);
    getimage(img, backproc, 0, 0, pwidth, pheight);
    delimage(backproc);
}

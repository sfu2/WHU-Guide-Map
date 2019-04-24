#ifndef GUI_MAP_H_INCLUDED
#define GUI_MAP_H_INCLUDED

#include<graphics.h>
#include<vector>
#include<direct.h>
#include"Œ‰¥ÛµÿÕº.h"

#define SKIPLINE_SIZE 100
#define NOT_GIVEN -1
#define PATH_START -1
#define DRAW_SPEED 4

#define MAP_WIDTH 600
#define MAP_RATIO 1.7
#define MAP_LENGTH (int)(MAP_WIDTH*MAP_RATIO)

#define TEXT_WIDTH	200
#define TEXT_HEIGHT 116

#define MASK_SIZE 15
#define FOCUSMASK_SIZE 20

#define SWAP(x, y) do{ int z = x; x = y ; y = z;} while(false)
#define LENGTH(x1,y1, x2, y2) (int)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)))
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

typedef struct{
    int x, y, num;
    char name[20];
	bool isFocus;
}Site;

extern PIMAGE mask, focus_mask, bkground, text_bkground;

// Deletion for image component
void delComponent();
// Initiation for image component
void initComponent();
void initTextBackground(PIMAGE &text_bkground);
void initBackground(PIMAGE &img);
void initTransparentMask(PIMAGE &mask, int mask_size);
// Called by initBackground()
void resizeImg(PIMAGE &img, int pwidth, int pheight);
void insert(char *str, int where, char ch);
// get site from file
void initSite(vector<Site> &site);
// is mouse on site
int isSite(vector<Site> site, int x = NOT_GIVEN, int y = NOT_GIVEN);
// display site info
void showSite(vector<Site> &site, Map *graph);
Route* getPath(Map * G, int site_1, int site_2);
// NOTE: did most of graphic updates!
void showTextPanel(Map * graph, int focus_site, const char *textString = NULL);
void showConnectSite(vector<Site> &site, Map *G, int site_num);
// get site by ctrl and click(s)
void getSite(vector<int> &input, vector<Site> site);
// display path
void showAllPath();
void connectPath(vector<int> output, Map * G);
void showSearchPanel();
void drawLine(vector<int> path, vector<int> output, Map *G);

#endif // GUI_MAP_H_INCLUDED

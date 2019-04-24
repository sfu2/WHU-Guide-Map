#include<stdio.h>
#include<stdlib.h>
#include<vector>
#pragma warning(disable:4996)

using namespace std;

typedef struct {
	int x, y, num;
	char name[20];
}Site;

typedef struct {
	int tag;
	int match;
}Search;

void initSite(vector<Site> &site);
bool searchForSite(vector<Site> site, char *input, int *output, int site_cnt);

int main() {
	int *output;
	char input[30];
	vector<Site> site;
	initSite(site);
	output = (int*)malloc(sizeof(int)*site.size());
	printf("请输入你想搜索的地点名称：");
	scanf("%s%*c", input);
	if (searchForSite(site, input, output, site.size())) {
		printf("编号  地点     横坐标 纵坐标\n"
			"---------------------------------\n");
		for (int i = 0; output[i] != 0; i++)
			printf("%2d   %-10s %3d   %3d\n", output[i], site[output[i] - 1].name, site[output[i] - 1].x, site[output[i] - 1].y);
	} else
		printf("No match\n");
	getchar();
	return 0;
}

bool searchForSite(vector<Site> site, char *input, int *output, int site_cnt) {
	int i;
	Search *match;
	match = (Search*)malloc(sizeof(Search)*(site_cnt + 1));
	for (i = 1; i <= site_cnt; i++) {
		match[i].tag = i;
		match[i].match = 0;
		for (int j = 0, k = 0; input[j] != '\0'&&site[i-1].name[k] != '\0'; k+=2) {
			if (site[i - 1].name[k] == '/')
				k++;
			if (input[j] == site[i-1].name[k]&& input[j+1] == site[i - 1].name[k+1]) {
				j+=2;
				match[i].match++;
			}
		}
	}
	for (i = 1; i <= site_cnt; i++) {
		for (int j = site_cnt; j-i+1>0; j--) {
			if (match[j].match>match[i].match) {
				match[0] = match[j];
				match[j] = match[i];
				match[i] = match[0];
			}
		}
	}
	for (i = 0; i < site_cnt; i++) {
		if (match[i + 1].match) {
			output[i] = match[i + 1].tag;
		} else {
			output[i] = 0;
			break;
		}
	}
	return i;
}

void initSite(vector<Site> &site) {
	FILE *fp;
	Site tmp;
	fp = fopen("site.txt", "rt");
	while (!feof(fp)) {
		fscanf(fp, "%d %d %s %d", &tmp.x, &tmp.y, tmp.name, &tmp.num);
		site.push_back(tmp);
	}
}

#include "武大地图.h"
#pragma warning(disable:4996)

 //将文件内容写入图 
bool FileToNode(Pnode **phead, int m ){
	Route *q;
	int k=0;
	int cout;
	char tag[10];
	char name[20];
	char infor[200];
	FILE *fp=fopen("DependentFiles\\Mapinform.txt","r");
	if(fp==NULL){
		return false;
	}
	for(int i=1; i<=m; i++){
		k=0;
		fgets(name, INF, fp);
		while(name[k]!='#'){
			phead[i]->name[k]=name[k];
			k++;
		}
		phead[i]->name[k]='\0';
		fgets(tag, INF, fp);
		k=cout=0;
		while(tag[k]!='#'){
			phead[i]->tag=10*cout+int(tag[k]-'0');
			cout=phead[i]->tag;
			k++;
		}
		fgets(infor, INF, fp);
		k=0;
		while(infor[k]!='#'){
			phead[i]->infor[k]=infor[k];
			k++;
		}
		phead[i]->infor[k]='\0';
	}


	for(int i=1; i<=m; i++){
		q=phead[i]->h;
		fgets(infor, INF, fp);
		k=0;//记录缓存行数组的指针位置
		int h = 0;
		char path[30];
		while(q!=NULL){
			cout = 0;
			while(infor[k]!='#'){
				cout=10 * cout + int(infor[k] - '0');
				k++;
			}
			k++;
			while (q != NULL) {
				if (q->tag == cout) {
					while (infor[k] != ' '&&infor[k]!='*') {
						path[h] = infor[k];
						h++; k++;
					}
					for (int i = 0; i < h; i++)
						q->infor[i] = path[i];
					break;
				}
				q = q->next;
			}
			q->infor[h] = '\0';
			h = 0;
			q = phead[i]->h;
			if (infor[k] == '*')
				break;
			k++;
		}	
	}
	fclose(fp);
	return true;
}

//指定节点输出内容 
void PrintNode(Map *G, int m){
	printf("景点名称：%s\n",G->phead[m]->name);
	printf("景点编号：%d\n",G->phead[m]->tag);
	printf("景点介绍：%s\n",G->phead[m]->infor);
	printf("邻近的景点有：\n");
	for(Route *p=G->phead[m]->h; p!=NULL; p=p->next)
		printf("%s ",G->phead[p->tag]->name);
} 


int Dispath (Graph *G, int dist[], int path[], int s[], int v, int m, int apath[]){
 	int k,d;
 	if(s[m]==1&&m!=v){
 		d=0;apath[d]=m;
 		k=path[m];
 		if(k==-1){
 			printf("查找路径失败\n");
 			return -1;
		 }
		 while(k!=v){
		 	apath[++d]=k;
		 	k=path[k];
		 }
		 d++;
		 apath[d]=v;//添加起点
		 return (dist[m]);	
	 }
	 if(m==v){
	 	apath[0]=m;
	 	return 0;
	 } 
	 else if(s[m]!=1){
	 	printf("查找路径失败\n");
 		return -1;
	 }

}
//最短路径算法（v为起点，原点,k为终点） 
int Dijkstra(Graph *G, vector<int> input, vector<int> &output, int apath[]){
	int dist[MAX], path[MAX];
 	int s[MAX];//s[i]代表顶点在S或者U中
	int MINdis,i,j,u;
	int v, m, result;
	v=input[0]-1;
	m=input[1]-1;
	for(i=0; i<G->node; i++){
	 	dist[i]= G->edges[v][i];
	 	s[i]=0;
	 	if(G->edges[v][i]<INF)
	 		path[i]=v;
	 	else
	 		path[i]=-1;
	 } 
	 s[v] =1;path[v]=0;
	 for(i=0; i<G->node-1; i++){
	 	MINdis=INF;
	 	for(j=0; j<G->node; j++)
	 		if(s[j]==0&&dist[j]<MINdis){
	 			u=j;
	 			MINdis=dist[j];
			 }
			 s[u]=1;
			 for(j=0; j<G->node; j++)
			 	if(G->edges[u][j]<INF&&dist[u]+G->edges[u][j]<dist[j]){
			 		dist[j]=dist[u]+G->edges[u][j];
			 		path[j]=u;
				 }
		
	 }
	 result=Dispath(G, dist, path, s, v, m, apath);
	 int k;
	 for(k=0; apath[k]!=v;k++)
	 	output.insert(output.begin(), apath[k]+1);
	 output.insert(output.begin(), v+1);
	 return result;
 }
 //输出最短路径(apath[]存储路径信息apath[0]储存终点；函数返回-1代表查找路径失败，无路径,否则返回最短路径长度） 

 Graph * CreatMatrix(int A[MAX][MAX], int node){
 	Graph *Gx=(Graph*)malloc(sizeof(Graph));
	 for(int i=0; i<node; i++)
	 	for(int j=0; j<node; j++){
	 	Gx->edges[i][j]=A[i][j];	
	}
	 Gx->node=node;
	 return (Gx);
 }

void MatixToTable(Map *G, int A[MAX][MAX], int node) {
	 Route* p;
	 int i, j;
	 for (i = 0; i < node; i++){
		 p = G->phead[i + 1]->h;
		 while (p != NULL) {
			 A[i][p->tag - 1] = p->weight;
			 p = p->next;
		 }
		 A[i][i] = 0;
		 for (j = 0; j < node; j++) {
			 if (A[i][j] == -1)
				 A[i][j] = INF;
		 }

	 }
 }

void FindPath(Map* G,vector<int> input, vector<int> &output){
 	int A[MAX][MAX];
	int i, j, node = G->node;
	for( i=0; i<MAX; i++)
		for ( j = 0; j < MAX; j++)
			A[i][j] = -1;
	int apath[MAX] ;
	MatixToTable(G, A, node);
	Graph *Gx=CreatMatrix(A,node);
	Dijkstra(Gx, input, output,apath);
 }

void getNeighbor(int site_num, vector<int> &output) {
	FILE *fp;
	char skipline[100];
	double distance = 0;
	int scanNum_1, scanNum_2;
	int line_x1, line_y1, line_x2, line_y2;

	output.clear();
	fopen_s(&fp, "DependentFiles\\path.txt", "rt");
	while (!feof(fp)) {
		if (fscanf_s(fp, "%d %d", &scanNum_1, &scanNum_2)
			&& (scanNum_1 == site_num || scanNum_2 == site_num) ){
			fscanf_s(fp, "%d %d %d %d",
				&line_x1, &line_y1, &line_x2, &line_y2);
			distance = 0;
			do {
				distance += sqrt((double)((line_x1 - line_x2)*(line_x1 - line_x2)
					+ (line_y1 - line_y2)*(line_y1 - line_y2)));
					line_x1 = line_x2;
					line_y1 = line_y2;
					fscanf_s(fp, "%d", &line_x2);
					if (line_x2 == -1)
						break;
				fscanf_s(fp, "%d", &line_y2);
			} while (true);
			output.push_back(scanNum_1 == site_num ? scanNum_2 : scanNum_1);
			output.push_back(distance);
		}
		else
			fgets(skipline, 100, fp);
	}
	fclose(fp);
}

void InitialMap(Map *& G) {
	int i, j;
	Route *p;
	Pnode* q;
	G = (Map*)malloc(sizeof(Map));
	for (i = 1; i <= MAX; i++) {
		q = (Pnode*)malloc(sizeof(Pnode));
		G->phead[i] = q;
		G->phead[i]->h = NULL;
	}
}

void CreatMap( Map *& G) {
	int f = 1;
	vector<int>buffer;
	while (1) {
		getNeighbor(f,buffer);
		//buffer里存有邻接节点及其距离

		if (buffer.size() == 0)
			break;
		G->phead[f]->tag = f;
		Route* p;
		for (int i = 0; i < (int)buffer.size(); i = i + 2) {
			p = (Route*)malloc(sizeof(Route));
			p->next = G->phead[f]->h;
			G->phead[f]->h = p;
			p->tag = buffer[i];
			p->weight = buffer[i + 1];
		};//将邻接节点存入邻接表
		f++;
	}
	
	if (!FileToNode(G->phead, --f))
		exit(-1);
	G->node = f;
}

void CreateNextNode(Route *&h, vector<int>buffer) {
	for (int i = 0; i < (int)buffer.size(); i = i + 2) {
		h->tag = buffer[i];
		h->weight = buffer[i + 1];
		h = h->next;
	}
}



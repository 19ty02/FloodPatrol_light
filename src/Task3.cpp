#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;
const double inf = 1e9;
const int maxn = 20, maxs = 1 << maxn;
const int N=54;
const int Govpos=50;
const double eps=1e-5;
const double velocity=35.0;
const double T=2.0, t=1.0;

struct node{
	int id;
	double val;
	bool operator < (const node &b) const{
		return val>b.val;
	}
};

int checktime(int id){
	return id>=36?T:t;
}

vector<int> bet[N][N]; // from i to j 
double mind[N][N];

void floyd(vector<vector<bool> > G, vector<vector<double> > L){
	int n=G.size()-1;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			if(i==j)mind[i][j]=0;
			else if(G[i][j])mind[i][j]=L[i][j];
			else mind[i][j]=inf;
		}
	}

	for(int k=1;k<=n;k++){
		for(int i=1;i<=n;i++){
			for(int j=1;j<=n;j++){
				if(mind[i][k]+mind[k][j]<mind[i][j]-eps){
					mind[i][j]=mind[i][k]+mind[k][j];
					bet[i][j].clear();
					for(auto x:bet[i][k])bet[i][j].push_back(x);
					bet[i][j].push_back(k);
					for(auto x:bet[k][j])bet[i][j].push_back(x);
				}
			}
		}
	}
	
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			if(!bet[i][j].empty()){
				assert(G[i][bet[i][j][0]]&&"Error: Path between to nodes do not continues");
				for(int k=1;k<bet[i][j].size();k++){
					assert(G[bet[i][j][k-1]][bet[i][j][k]]&&"Error: Path between to nodes do not continues");
				}
				assert(G[bet[i][j][bet[i][j].size()-1]][j]&&"Error: Path between to nodes do not continues");
			}
		}
	}
}

double dis[N];
double tim[N],htim[N];
int pre[N];

// start point has the id 0
double dijkstra(int s, vector<vector<bool> > G, vector<vector<double> > L){
    int n = G.size();
    for(int i=0;i<n;i++){
    	dis[i]=inf;
	}
    priority_queue<node> q;
    dis[s]=0; pre[s]=-1;
    q.push((node){s,dis[s]});
    
    while (!q.empty())
    {
        int u = q.top().id;
        q.pop();
        for (int v = 0; v < n; v++)
        {
            if (G[u][v] && dis[u] + L[u][v] < dis[v])
            {
                dis[v]=dis[u]+L[u][v];
                pre[v]=u;
                q.push((node){v,dis[v]});
            }
        }
    }
	
	int ps=0; double mx=0;
	for(int i=1;i<n;i++){
		tim[i]=2*dis[i]/velocity+checktime(i);
		htim[i]=dis[i]/velocity+checktime(i);
		if(tim[i]>mx){
			ps=i;
			mx=tim[i];
		}
	}
	
	int cnt=0;
	for(int i=1;i<n;i++){
		if(dis[i]/velocity<1.6){
			cnt++;
		}
	}
    return mx;
}


vector<vector<int> > readDivide(){
	int m;
	scanf("%d",&m);
	vector<vector<int> > res(m);
	for(int i=0;i<m;i++){
		while(1){
			int x;scanf("%d",&x);
			if(x==-1)break;
			res[i].push_back(x);	
		}
	}
	return res;
}

pair<vector<vector<bool> >, vector<vector<double> > > readGraph(){
	int n;
	scanf("%d",&n);
	vector<vector<bool> > G(n);
	vector<vector<double> > L(n);
	for(int i=0;i<n;i++){
		G[i].resize(n);
		L[i].resize(n);
	}
	
	while(1){
		int u,v; double l;
		scanf("%d%d%lf",&u,&v,&l);
		if(u==-1)break;
		G[u][v]=true;
		L[u][v]=l;
	}
	return make_pair(G,L);
}

double printbet(int u,int v,bool prt){
	for(auto x:bet[u][v]){
		if(prt)printf(",%d",x);
	}
	if(prt)printf(",%d",v);
	return mind[u][v]/velocity;
}
void printPath(vector<int> pth,double lim){
	double tottime=0;
	
	printf("巡视地点: ");
	for(int i=0;i<pth.size();i++){
		int x=pth[i];
		if(i>0)printf(",");
		printf("%-2d",x);
		tottime+=checktime(x);
	}
	for(int i=pth.size();i<3;i++)printf("   ");
	
	printf(" | ");
	
	tottime+=printbet(Govpos,pth[0],false);
	for(int i=1;i<pth.size();i++){
		tottime+=printbet(pth[i-1],pth[i],false);
	}
	tottime+=printbet(pth[pth.size()-1],Govpos,false);	
	
	printf(" 耗时： %.2lfh | ",tottime);
	assert(tottime<lim+eps&&"Error: The path is out of time limitaion");
	
	printf("巡视路线: ");
	printf("%d",Govpos);
	tottime+=printbet(Govpos,pth[0],true);
	for(int i=1;i<pth.size();i++){
		tottime+=printbet(pth[i-1],pth[i],true);
	}
	tottime+=printbet(pth[pth.size()-1],Govpos,true);	
	
	printf("\n");
}

bool vis[N];
void findSolution(double lim,int n, vector<vector<bool> > G, vector<vector<double> > L){
	int cnt=0;
	vis[50]=true;
	while(1){
		int u=0,mx=0;
		for(int i=1;i<n;i++){
			if(!vis[i]&&tim[i]>mx){
				mx=tim[i];
				u=i;
			}
		}
		if(u==0)break;
		
		vector<int> pth;
		
		vis[u]=true; pth.push_back(u);
		double now=dis[u]/velocity+checktime(u);
		
		int vi=0,vj=0;
		mx=0;
		for(int i=1;i<n;i++){
			for(int j=1;j<n;j++){
				if(i!=j&&!vis[i]&&!vis[j]&&now+checktime(i)+checktime(j)+(mind[u][i]+mind[i][j]+dis[j])/velocity<lim+eps){
					if(htim[i]+htim[j]>mx){
						mx=htim[i]+htim[j];
						vi=i;vj=j;
					}
				}
			}
		}
		if(vi!=0){
			vis[vi]=vis[vj]=true;
			pth.push_back(vi);
			pth.push_back(vj);
			now+=(mind[u][vi]+mind[vi][vj])/velocity+checktime(vi)+checktime(vj);
			u=vj;
		}
		
		while(1){
			int v=0,mx=0;
			for(int i=1;i<n;i++){
				if(!vis[i]&&now+checktime(i)+(mind[u][i]+dis[i])/velocity<lim+eps){
					if(tim[i]>mx){
						mx=tim[i];
						v=i;
					}
				}
			}
			if(v==0)break;
			vis[v]=true; pth.push_back(v);
			now+=mind[u][v]/velocity+checktime(v);
			u=v;
		}
		
		cnt++;
		printPath(pth,lim);
	}
	printf("总路径条数: %d\n",cnt);
	for(int i=1;i<n;i++){
		assert(vis[i]&&"Have node hasn't been visited");
	}
}

int main(){
    freopen("../data/data3.txt","r",stdin);
    freopen("solution3.txt","w",stdout);
    auto pr=readGraph();
    auto G=pr.first;
	auto L=pr.second;
    
    floyd(G, L); 
    
    double mx=dijkstra(Govpos, G, L);
    printf("完成巡视的最短时间: %.2lfh 方案如下：\n",mx); 
    findSolution(mx, G.size(), G, L);
}

#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;
const double inf = 1e9;
const int maxn = 20, maxs = 1 << maxn;
const int N=55;
const double eps=1e-5;
const double velocity=35.0;
const double Govpos=50;
const double T=2.0, t=1.0;

struct node{
	int id;
	double val;
	bool operator < (const node &b) const{
		return val>b.val;
	}
};

int checktime(int id){
	if(id==Govpos)return 0;
	return id>=36?T:t;
}

vector<int> bet[N][N]; // from i to j 
double dis[N][N];

void getdis(vector<vector<bool> > G, vector<vector<double> > L){
	int n=G.size()-1;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			if(i==j)dis[i][j]==0;
			else if(G[i][j]) dis[i][j]=L[i][j];
			else dis[i][j]=inf;
		}
	}

	for(int k=1;k<=n;k++){
		for(int i=1;i<=n;i++){
			for(int j=1;j<=n;j++){
				if(dis[i][k]+dis[k][j]<dis[i][j]-eps){
					dis[i][j]=dis[i][k]+dis[k][j];
					bet[i][j].clear();
					for(auto x:bet[i][k])bet[i][j].push_back(x);
					bet[i][j].push_back(k);
					for(auto x:bet[k][j])bet[i][j].push_back(x);
				}
			}
		}
	}
}

// start point has the id 0
// The L is not the length of edge, but the mindis
// output the true id in path
vector<int> solveHamilton(vector<vector<bool> > G, vector<vector<double> > L, vector<int> f2t)
{
    int n = f2t.size();
    vector<vector<double> > f(n);

    int m = 1 << n;
    for (int i = 0; i < n; i++)
    {
        f[i].resize(m);
        for (int s = 0; s < m; s++)
        {
            f[i][s] = inf;
        }
    }

    priority_queue<node> q;
    vector<int> pre(n * m);
    vector<int> vis(n * m);
    f[0][1] = 0;
    pre[0 * m + 1] = -1;

    q.push((node){0 * m + 1, 0});

    while (!q.empty())
    {
        int fr = q.top().id;
        q.pop();
        if(vis[fr])continue;
        vis[fr]=true;

        int u = fr / m, s = fr % m;
        for (int v = 0; v < n; v++)
        {
            int t = s | (1 << v);
            if (f[u][s] + L[u][v]< f[v][t])
            {
                f[v][t] = f[u][s] + L[u][v];
                pre[v * m + t] = u*m + s;
                q.push((node){v * m + t, f[v][t]});
            }
        }
    }
	
    vector<int> ans;
    int cur = 0 * m + (m - 1);
    while (cur != -1)
    {
		int u=cur/m, v=pre[cur]/m;
        ans.push_back(f2t[u]);
        if(pre[cur]!=-1){
        	for(auto x:bet[f2t[u]][f2t[v]]){
        		ans.push_back(x);
			}
		}
        cur = pre[cur];
    }
    return ans;
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
		G[u][v]=G[v][u]=true;
		L[u][v]=L[v][u]=l;
	}
	return make_pair(G,L);
}

void solve(vector<int> nList, vector<vector<bool> > Gtot, vector<vector<double> > Ltot){
	int n=nList.size();
	vector<bool> inList(N);
	for(int i=0;i<n;i++){
		inList[nList[i]]=true;
	}
	
	vector<vector<bool> > G(n);
	vector<vector<double> > L(n);
	for(int i=0;i<n;i++){
		G[i].resize(n);
		L[i].resize(n);
	}
	
	for(int u=0;u<n;u++){
		for(int v=0;v<n;v++){
			G[u][v]=Gtot[nList[u]][nList[v]];
			L[u][v]=dis[nList[u]][nList[v]];
		}
	}
	
	auto ans=solveHamilton(G, L, nList);
	
	double di=0,ti=0;
	vector<bool> vis(N);
	
	for(int i=0;i<nList.size();i++){
		ti+=checktime(nList[i]);
	}
	for(int i=1;i<ans.size();i++){
		assert(Gtot[ans[i]][ans[i-1]]); 
		ti+=Ltot[ans[i]][ans[i-1]]/velocity;
		di+=Ltot[ans[i]][ans[i-1]];
	}
	printf("最短距离: %.2lfkm\n",di);
	printf("路径是:\n");
	for(int i=0;i<ans.size();i++){
		printf("%d",ans[i]);
		if(i<ans.size()-1)printf(",");
	}
	printf("\n");
}

int main(){
    freopen("../data/data1.txt","r",stdin);
    freopen("solution1.txt","w",stdout);
    auto div=readDivide();
    auto pr=readGraph();
    auto G=pr.first;
	auto L=pr.second;
    
    getdis(G,L);
    
    for(int i=0;i<div.size();i++){
    	printf("分组 (%d) :\n",i+1);
    	solve(div[i],G,L);
    	printf("\n");
	}
}

#include <bits/stdc++.h>
using namespace std;
long long DFS(const vector<vector<int>>& adj,vector<pair<int,long long>>& edges,long long pushes,int u,int t,const vector<int>& level,vector<int>& cur){
    if(u==t){
        return pushes;
    }
    for(int &i=cur[u];i<adj[u].size();i++){
        int v=edges[adj[u][i]].first;
        long long w=edges[adj[u][i]].second;
        if(level[u]!=level[v]-1 || w<=0){
            continue;
        }
        long long temp_pushes=min(pushes,w);
        long long bottleneck=DFS(adj,edges,temp_pushes,v,t,level,cur);
		edges[adj[u][i]].second-=bottleneck;
        if((adj[u][i] & 1)==0){ //偶数边为原边，奇数边为后添加的反向边
            edges[adj[u][i]+1].second+=bottleneck;
        }else{
            edges[adj[u][i]-1].second+=bottleneck;
        }
        if(bottleneck>0){
            return bottleneck;
        }
    }
    return 0;
}
long long dinitz(const vector<vector<int>>& adj,vector<pair<int,long long>>& edges,int s,int t){
    int n=adj.size()-1;
    long long flow=0;
    bool hasroute=true;
    while(hasroute){
        hasroute=false;
        vector<int> level(n+1,-1); 
        queue<int> q;
        q.push(s);
        level[s]=0;
        //BFS构建层次图
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int i=0;i<adj[u].size();i++){
                int v=edges[adj[u][i]].first;
                long long w=edges[adj[u][i]].second;
                if(level[v]==-1 && w>0){
                    level[v]=level[u]+1;
                    if(v==t){
                        hasroute=true;
                    }
                    q.push(v);
                }
            }
        }
        if(!hasroute){
            return flow;
        }
        vector<int> cur(n+1,0);
        while(true){
            long long pushed = DFS(adj, edges, LLONG_MAX, s, t, level, cur);
            if (pushed == 0) break;
            flow += pushed;
        }
    }
    return flow;
}
int main(){
    int n,m;
    cin>>n>>m;
    long long sum=0;
	vector<vector<int>> adj(m*n+2); //0为s，mn+1为t
	vector<pair<int,long long>> edges;
	int edgecnt=0;
	int nodecnt=m*n+2;
	// 单选文的快乐
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			//第i行第j列同学，即a[i][j],编号为(i-1)*m+j
			long long h;
			cin>>h;
			sum+=h; 
			edges.push_back(make_pair((i-1)*m+j,h));
			adj[0].push_back(edgecnt++);

			edges.push_back(make_pair(0,0));
			adj[(i-1)*m+j].push_back(edgecnt++);
		}
	}
	//单选理的快乐
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			long long h;
			cin>>h;
			sum+=h; 
			edges.push_back(make_pair(m*n+1,h));
			adj[(i-1)*m+j].push_back(edgecnt++);

			edges.push_back(make_pair((i-1)*m+j,0));
			adj[m*n+1].push_back(edgecnt++);
		}
	}
	//与i+1行对应同学同选文的额外快乐
	for(int i=1;i<=n-1;i++){
		for(int j=1;j<=m;j++){
			long long h;
			cin>>h;
			sum+=h; 
			vector<int> arr;
			adj.push_back(arr);
			edges.push_back(make_pair(nodecnt,h));
			adj[0].push_back(edgecnt++);
			edges.push_back(make_pair(0,0));
			adj[nodecnt].push_back(edgecnt++);

			edges.push_back(make_pair((i-1)*m+j,1e18));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[(i-1)*m+j].push_back(edgecnt++);

			edges.push_back(make_pair(i*m+j,1e18));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[i*m+j].push_back(edgecnt++);
			nodecnt++;
		}
	}
	//与i+1行对应同学同选理的额外快乐
	for(int i=1;i<=n-1;i++){
		for(int j=1;j<=m;j++){
			long long h;
			cin>>h;
			sum+=h; 
			vector<int> arr;
			adj.push_back(arr);
			edges.push_back(make_pair(m*n+1,h));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[m*n+1].push_back(edgecnt++);

			edges.push_back(make_pair(nodecnt,1e18));
			adj[(i-1)*m+j].push_back(edgecnt++);
			edges.push_back(make_pair((i-1)*m+j,0));
			adj[nodecnt].push_back(edgecnt++);

			edges.push_back(make_pair(nodecnt,1e18));
			adj[i*m+j].push_back(edgecnt++);
			edges.push_back(make_pair(i*m+j,0));
			adj[nodecnt].push_back(edgecnt++);
			nodecnt++;
		}
	}
	//与j+1列对应同学同选文的额外快乐
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m-1;j++){
			long long h;
			cin>>h;
			sum+=h; 
			vector<int> arr;
			adj.push_back(arr);
			edges.push_back(make_pair(nodecnt,h));
			adj[0].push_back(edgecnt++);
			edges.push_back(make_pair(0,0));
			adj[nodecnt].push_back(edgecnt++);

			edges.push_back(make_pair((i-1)*m+j,1e18));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[(i-1)*m+j].push_back(edgecnt++);

			edges.push_back(make_pair((i-1)*m+j+1,1e18));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[(i-1)*m+j+1].push_back(edgecnt++);
			nodecnt++;
		}
	}
	//与j+1列对应同学同选理的额外快乐
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m-1;j++){
			long long h;
			cin>>h;
			sum+=h; 
			vector<int> arr;
			adj.push_back(arr);
			edges.push_back(make_pair(m*n+1,h));
			adj[nodecnt].push_back(edgecnt++);
			edges.push_back(make_pair(nodecnt,0));
			adj[m*n+1].push_back(edgecnt++);

			edges.push_back(make_pair(nodecnt,1e18));
			adj[(i-1)*m+j].push_back(edgecnt++);
			edges.push_back(make_pair((i-1)*m+j,0));
			adj[nodecnt].push_back(edgecnt++);

			edges.push_back(make_pair(nodecnt,1e18));
			adj[(i-1)*m+j+1].push_back(edgecnt++);
			edges.push_back(make_pair((i-1)*m+j+1,0));
			adj[nodecnt].push_back(edgecnt++);
			nodecnt++;
		}
	}
	long long mincut=dinitz(adj,edges,0,m*n+1);
	cout<<sum-mincut<<endl;
}
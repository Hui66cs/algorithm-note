#include <bits/stdc++.h>
using namespace std;
void MinCost_MaxFlow(const vector<vector<int>>& adj,vector<tuple<int,int,long long>>& edges,long long& cost,int& flow,int t){
    int n=adj.size()-1;
    bool hasroute=true;
    while(hasroute){
        hasroute=false;
        queue<int> q;
        q.push(1);
        vector<long long> d(n+1,LLONG_MAX);
        vector<bool> inq(n+1,false);
        vector<int> parent(n+1,-1);
        vector<int> parent_edge(n+1,-1);
        bool hasneg=false;
        d[1]=0;
        inq[1]=true;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            inq[u]=false;
            for(int i=0;i<adj[u].size();i++){
                int v=get<0>(edges[adj[u][i]]);
                int capacity=get<1>(edges[adj[u][i]]);
                long long cost=get<2>(edges[adj[u][i]]);
                if(capacity>0 && d[v]>d[u]+cost){
                    d[v]=d[u]+cost;
                    parent[v]=u;
                    parent_edge[v]=adj[u][i];
                    if(!inq[v]){
                        q.push(v);
                        inq[v]=true;
                    }
                    if(v==t){
                        hasroute=true;
                    }
                }
            }
        }
        if(parent[t]==-1){
            break;
        }
        int temp=t;
        while(temp!=1){
            get<1>(edges[parent_edge[temp]])-=1;
            cost+=get<2>(edges[parent_edge[temp]]);
            if((parent_edge[temp] & 1) ==0){
                get<1>(edges[parent_edge[temp]+1])+=1;
            }else{
                get<1>(edges[parent_edge[temp]-1])+=1;
            }
            temp=parent[temp];
        }
        flow+=1;
    }
}
int main(){
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(2*n-1); //边序号。2~n-1号点被拆了，第x号点为x_in, x+n-1号点为x_out
    vector<tuple<int,int,long long>> edges; //v,capacity,cost
    int edgecnt=0;
    for(int i=2;i<=n-1;i++){ //拆点
        edges.push_back(make_tuple(i+n-1,1,0)); //v_in -> v_out
        adj[i].push_back(edgecnt);
        edgecnt++;
        edges.push_back(make_tuple(i,0,0)); //反向边。对于边序号t，t & 1 =0的是正向边，=1的是反向边
        adj[i+n-1].push_back(edgecnt);
        edgecnt++;
    }
    for(int i=0;i<m;i++){
        int u,v;
        long long c;
        cin>>u>>v>>c;
        if(u==1 && v==n){ //这两个点无需拆点,且容量为1
            edges.push_back(make_tuple(n,1,c));
            adj[1].push_back(edgecnt);
            edgecnt++;
            edges.push_back(make_tuple(1,0,-c)); //反向边
            adj[n].push_back(edgecnt);
            edgecnt++;
        }else if(u==1){
            edges.push_back(make_tuple(v,1,c)); 
            adj[1].push_back(edgecnt);
            edgecnt++;
            edges.push_back(make_tuple(1,0,-c)); 
            adj[v].push_back(edgecnt);
            edgecnt++;
        }else if(v==n){
            edges.push_back(make_tuple(n,1,c)); 
            adj[u+n-1].push_back(edgecnt);
            edgecnt++;
            edges.push_back(make_tuple(u+n-1,0,-c)); 
            adj[n].push_back(edgecnt);
            edgecnt++;
        }else{
            edges.push_back(make_tuple(v,1,c)); 
            adj[u+n-1].push_back(edgecnt);
            edgecnt++;
            edges.push_back(make_tuple(u+n-1,0,-c)); 
            adj[v].push_back(edgecnt);
            edgecnt++;
        }
    }
    long long cost=0;
    int flow=0;
    MinCost_MaxFlow(adj,edges,cost,flow,n);
    cout<<flow<<" "<<cost<<endl;
}
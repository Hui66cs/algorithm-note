#include <bits/stdc++.h>
using namespace std;
void MinCost_MaxFlow(const vector<vector<int>>& adj,vector<tuple<int,long long,long long>>& edges,long long& cost,int& flow,int s,int t){
    int n=adj.size()-1;
    bool hasroute=true;
    while(hasroute){
        hasroute=false;
        queue<int> q;
        q.push(s);
        vector<long long> d(n+1,LLONG_MAX/2);
        vector<bool> inq(n+1,false);
        vector<int> parent(n+1,-1);
        vector<int> parent_edge(n+1,-1);
        bool hasneg=false;
        d[s]=0;
        inq[s]=true;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            inq[u]=false;
            for(int i=0;i<adj[u].size();i++){
                int v=get<0>(edges[adj[u][i]]);
                long long capacity=get<1>(edges[adj[u][i]]);
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
        long long bottleneck=LLONG_MAX;
        while(temp!=s){
            bottleneck=min(bottleneck,get<1>(edges[parent_edge[temp]]));
            temp=parent[temp];
        }
        temp=t;
        while(temp!=s){
            get<1>(edges[parent_edge[temp]])-=bottleneck;
            cost+=bottleneck*get<2>(edges[parent_edge[temp]]);
            if((parent_edge[temp] & 1) ==0){
                get<1>(edges[parent_edge[temp]+1])+=bottleneck;
            }else{
                get<1>(edges[parent_edge[temp]-1])+=bottleneck;
            }
            temp=parent[temp];
        }
        flow+=bottleneck;
    }
}
int main(){
    int n,m,s,t;
    cin>>n>>m>>s>>t;
    vector<vector<int>> adj(n+1); 
    vector<tuple<int,long long,long long>> edges; 
    int edgecnt=0;
    for(int i=0;i<m;i++){
        int u,v;
        long long cap,cost;
        cin>>u>>v>>cap>>cost;
        edges.push_back(make_tuple(v,cap,cost));
        adj[u].push_back(edgecnt);
        edgecnt++;
        edges.push_back(make_tuple(u,0,-cost)); //反向边
        adj[v].push_back(edgecnt);
        edgecnt++;
    }
    long long cost=0;
    int flow=0;
    MinCost_MaxFlow(adj,edges,cost,flow,s,t);
    cout<<flow<<" "<<cost<<endl;
}
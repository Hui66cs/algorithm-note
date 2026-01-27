#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void BFS(vector<int>& dist,vector<int>& parent,const vector<vector<int>>& adj,int s){
    dist[s]=0;
    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int u=Q.front();
        Q.pop();
        int n=adj[u].size();
        for(int i=0;i<n;i++){
            int v=adj[u][i];
            if(dist[v]==-1){
                Q.push(v);
                dist[v]=dist[u]+1;
                parent[v]=u;
            }
        }
    }
}

void Issafe(const vector<vector<int>>& adj,vector<bool>& safe,const vector<int>& dist){
    int n=safe.size();
    for(int i=1;i<n;i++){
        if(dist[i]!=-1){
            int m=adj[i].size();
            safe[i]=true;
            for(int j=0;j<m;j++){
                if(dist[adj[i][j]]==-1){
                    safe[i]=false;
                    break;
                }
            }
        }else{
            safe[i]=false;
        }
    }
}

void pro_BFS(vector<int>& dist,vector<int>& parent,const vector<bool>& safe,const vector<vector<int>>& adj,int s){
    if(!safe[s]){
        return;
    }
    dist[s]=0;
    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int u=Q.front();
        Q.pop();
        int n=adj[u].size();
        for(int i=0;i<n;i++){
            int v=adj[u][i];
            if(dist[v]==-1&&safe[v]){
                Q.push(v);
                dist[v]=dist[u]+1;
                parent[v]=u;
            }
        }
    }
}

void Show_Shortest_Path(const vector<int>& parent,int path_node){
    if(parent[path_node]==-1){
        cout<<path_node<<" ";
        return;
    }
    Show_Shortest_Path(parent,parent[path_node]);
    cout<<path_node<<" ";
}

int main(){
    int V;
    int E;
    cin>>V>>E;
    vector<vector<int>> adj(V+1);
    vector<vector<int>> rev_adj(V+1);
    for(int i=0;i<E;i++){
        int u;
        int v;
        cin>>u>>v;
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }
    int source;
    int dest;
    cin>>source>>dest;
    vector<int> dist1(V+1,-1);
    vector<int> parent1(V+1,-1);
    vector<int> dist2(V+1,-1);
    vector<int> parent2(V+1,-1);
    BFS(dist2,parent2,rev_adj,dest);
    vector<bool> safe(V+1);
    Issafe(adj,safe,dist2);
    pro_BFS(dist1,parent1,safe,adj,source);
    cout<<dist1[dest]<<endl;
}

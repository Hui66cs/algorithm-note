#include <iostream>
#include <vector>
using namespace std;

bool TopologicalSort_DFS_Visit(const vector<vector<int>>& adj,vector<int>& d,vector<int>& f,vector<int>& parent,int& time,int u,vector<int>& sort,int& index){
    d[u]=++time;
    for(int v:adj[u]){
        if(d[v]!=-1&&f[v]==-1){
            return false;
        }
        if(d[v]==-1){
            parent[v]=u;
            if(!TopologicalSort_DFS_Visit(adj,d,f,parent,time,v,sort,index)){
                return false;
            }
            
        }
    }
    f[u]=++time;
    sort[index]=u;
    index--;
    return true;
}

bool DFS(const vector<vector<int>>& adj,vector<int>& d,vector<int>& f,vector<int>& parent,vector<int>& sort){
    int time=0;
    int n=d.size();
    int index=n-2;
    for(int i=1;i<n;i++){
        if(d[i]==-1){
            if(!TopologicalSort_DFS_Visit(adj,d,f,parent,time,i,sort,index)){
                return false;
            }
        }
    }
    return true;
}

int main(){
    int V;
    int E;
    cin>>V>>E;
    vector<vector<int>> adj(V+1);
    for(int i=0;i<E;i++){
        int u;
        int v;
        cin>>u>>v;
        adj[u].push_back(v);
    }
    vector<int> d(V+1,-1);
    vector<int> f(V+1,-1);
    vector<int> parent(V+1,-1);
    vector<int> sort(V);
    if(!DFS(adj,d,f,parent,sort)){
        cout<<"Error, not a DAG"<<endl;
    }
}

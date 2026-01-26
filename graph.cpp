#include <iostream>
#include <vector>
using namespace std;
bool Exist_Edge(const vector<bool>& adj,int V, int u,int v){
    if(adj[(u-1)*V+v-1]==true){
        return true;
    }else{
        return false;
    }
}
int main(){
    int V;
    int E;
    cin>>V>>E;
    vector<bool> adj((V)*(V));
    for(int i=0;i<E;i++){
        int u;
        int v;
        cin>>u>>v;
        adj[(u-1)*V+v-1]=1;
        //if it's undirected graph, also need
        //adj[(v-1)*V+u-1]=1;
    }
}

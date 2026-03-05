#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <stack>
using namespace std;

struct Edge{
    int u;
    int v;
    int w;

    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

struct DSU{
    vector<int> parent;
    vector<int> size; 

    DSU(){
        parent.push_back(-1);
        size.push_back(-1);
    }
    
    void make_set(int n){
        for(int i=1;i<n+1;i++){
            parent.push_back(i);
            size.push_back(1);
        }
    }

    int Find(int u){
        if(parent[u]==u){
            return u;
        }else{
            return parent[u]=Find(parent[u]);
        }
    }

    void Union(int a,int b){
        int a_ancestor=Find(a);
        int b_ancestor=Find(b);
        if(a_ancestor==b_ancestor){
            return;
        }
        int size_a=size[a_ancestor];
        int size_b=size[b_ancestor];
        if(size_a<size_b){
            parent[a_ancestor]=b_ancestor;
            size[b_ancestor]+=size[a_ancestor];
        }else{
            parent[b_ancestor]=a_ancestor;
            size[a_ancestor]+=size[b_ancestor];
        }
    }
};

void Kruskal(vector<Edge>& edges,vector<Edge>& tree,int m){
    sort(edges.begin(), edges.end());
    DSU dsu;
    dsu.make_set(m);
    int n=edges.size();
    for(int i=0;i<n;i++){
        int u=edges[i].u;
        int v=edges[i].v;
        if(dsu.Find(u)!=dsu.Find(v)){
            dsu.Union(u,v);
            tree.push_back(edges[i]);
        }
    }
}

int main(){
    int V;
    int E;
    cin>>V>>E;
    vector<Edge> edges(E);
    for(int i=0;i<E;i++){
        int u;
        int v;
        int w;
        cin>>u>>v>>w;
        edges[i].u=u;
        edges[i].v=v;
        edges[i].w=w;
    }
    vector<Edge> tree;  
    Kruskal(edges,tree,V);
    if(tree.size()!=V-1){
        cout<<"orz"<<endl;
        return 0;
    }
    int m=tree.size();
    int length=0;
    for(int i=0;i<m;i++){
        length+=tree[i].w;
    }
    cout<<length<<endl;
}



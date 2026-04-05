#include<bits/stdc++.h>
using namespace std;
struct DSU{
    vector<int> parent;
    vector<int> size; 
    
    DSU(int n) {  //already know how many elements will be in the set
        parent.resize(n+1);
        size.resize(n+1, 1); 
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }
    }

    int Find(int u){  //Path Compression: when searching for the root, we can set all the passing through nodes's parent be the root
        if(parent[u]==u){
            return u;
        }else{
            return parent[u]=Find(parent[u]);
        }
    }

    void Union(int a,int b){  //Union by size: always combine the smaller-size tree to the larger-size tree
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

int main(){
    int n;
    int m;
    int T;
    cin>>n>>m>>T;
    vector<tuple<int,int,int>> edge(m); // u,v,weight
    for(int i=0;i<m;i++){
        int u;
        int v;
        int w;
        cin>>u>>v>>w;
        edge[i]=make_tuple(u,v,w);
    }
    DSU dsu(n);
    for(int i=0;i<m;i++){
        if(get<2>(edge[i])>=T){
            continue;
        }else{
            int u=get<0>(edge[i]);
            int v=get<1>(edge[i]);
            dsu.Union(u,v);
        }
    }
    unordered_set<int> set;
    int count=0;
    for(int i=1;i<=n;i++){
        int parent=dsu.Find(i);
        if(set.count(parent)==0){
            count++;
            set.insert(parent);
        }
    }
    cout<<count<<endl;
}
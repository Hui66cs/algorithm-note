#include <bits/stdc++.h>
using namespace std;
struct DSU{
    vector<int> parent;
    
    DSU(int n) {  //already know how many elements will be in the set
        parent.resize(n+1);
        for (int i = 1; i < n+1; i++) {
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

    void Union(int a,int b){  //将a合并到b的集合中
        int a_ancestor=Find(a);
        parent[a_ancestor]=b;
    }
};

int main(){
    int n;
    int F;
    cin>>n>>F;
    int timestamp=1;
    long long cost=0;
    vector<tuple<double,int,int,int>> info(n+1); //平均权重，权重和，size，被选时间戳
    priority_queue<tuple<double,int,int>> heap; //平均权重，节点编号,被选时间戳
    for(int i=1;i<=n;i++){
        int w;
        cin>>w;
        cost+=w;
        info[i]=make_tuple(w,w,1,timestamp);
        if(i!=F){
            heap.push(make_tuple(w,i,timestamp));
        }
    }
    vector<int> parent(n+1,-1);
    for(int i=1;i<n;i++){
        int u;
        int v;
        cin>>u>>v;
        parent[v]=u;
    }
    DSU dsu(n);
    while(!heap.empty()){
        int u=get<1>(heap.top());
        int time=get<2>(heap.top());
        int real_time=get<3>(info[u]);
        heap.pop();
        if(time!=real_time){
            continue;
        }
        int p=dsu.Find(parent[u]);
        cost+=(long long)get<2>(info[p])*get<1>(info[u]); //增加延迟代价
        get<2>(info[p])+=get<2>(info[u]);  //更新size
        get<1>(info[p])+=get<1>(info[u]); //更新权重和
        get<0>(info[p])=(double)get<1>(info[p])/get<2>(info[p]); //更新平均权重
        dsu.Union(u,p);  //更新祖先
        timestamp++;
        get<3>(info[p])=timestamp; //更新权重和
        if(p!=F){
            heap.push(make_tuple(get<0>(info[p]),p,timestamp));
        }
    }
    cout<<cost<<endl;
}
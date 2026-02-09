#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <stack>
using namespace std;

void Min_Heapify(vector<pair<long long,int>>& heap,int x,int heapsize,vector<int>& location){
    int l=2*x;  //left child
    int r=2*x+1;  //right child
    int smallest;  
    //to find the largest element between it and two childern
    if(l<=heapsize&&heap[x].first>heap[l].first){
        smallest=l;
    }else{
        smallest=x;
    }
    if(r<=heapsize&&heap[smallest].first>heap[r].first){
        smallest=r;
    }
    if(x!=smallest){
        swap(location[heap[x].second],location[heap[smallest].second]);
        swap(heap[x],heap[smallest]);
        Min_Heapify(heap,smallest,heapsize,location);
    }
}

void Build_Min_Heap(vector<pair<long long,int>>& heap,int heapsize,vector<int>& location){
    for(int i=heapsize/2;i>=1;i--){
        Min_Heapify(heap,i,heapsize,location);
    }
}

void Decrease_Key(vector<pair<long long,int>>& heap,int heapsize,vector<int>& location,long long s,int index){
    int location_index=location[index];
    heap[location_index].first=s;
    int p=location_index/2;
    while(true){
        if(p==0||(heap[p].first<=heap[location_index].first)){
            break;
        }else{
            swap(location[heap[p].second],location[heap[location_index].second]);
            swap(heap[p],heap[location_index]);
        }
        location_index=p;
        p=p/2;
    }
}

void Dijkstra(const vector<vector<pair<int,int>>>& adj,vector<long long>& dist,vector<int>& parent,int s){
    dist[s]=0;
    //first is dist of the node, second is the index of this node
    int n=dist.size()-1;
    vector<pair<long long,int>> heap;
    vector<int> location;
    heap.push_back({0,0});  //to occupy the null 
    location.push_back(-1);
    for(int i=1;i<n+1;i++){
        heap.push_back({dist[i],i});
        location.push_back(i);
    }
    Build_Min_Heap(heap,n,location);
    while(n>0){
        int u=heap[1].second;
        swap(location[heap[1].second],location[heap[n].second]);
        swap(heap[1],heap[n]);
        n--;
        Min_Heapify(heap,1,n,location);
        int m=adj[u].size();
        for(int v=0;v<m;v++){
            //in adj[u][v], first is edge node, second is edge weight
            if(location[adj[u][v].first]<=n&&dist[adj[u][v].first]>dist[u]+adj[u][v].second){  
                parent[adj[u][v].first]=u;
                dist[adj[u][v].first]=dist[u]+adj[u][v].second;
                Decrease_Key(heap,n,location,dist[u]+adj[u][v].second,adj[u][v].first);
            }
        }
    }
}

void show_path(vector<int>& path,const vector<int>& parent,int des,int& count){
    if(des==-1){
        return;
    }
    count++;
    show_path(path,parent,parent[des],count);
    path.push_back(des);    
}

int main(){
    int V;
    int E;
    int source;
    int dest;
    cin>>V>>E>>source>>dest;
    vector<vector<pair<int,int>>> adj(V+1);
    for(int i=0;i<E;i++){
        int u;
        int v;
        int w;
        cin>>u>>v>>w;;
        adj[u].push_back({v,w});
    }
    vector<long long> dist(V+1,1e16);
    vector<int> parent(V+1,-1);
    Dijkstra(adj,dist,parent,source);
    if(parent[dest]==-1){
        cout<<-1<<endl;
        return 0;
    }else{
        cout<<dist[dest]<<" ";
        vector<int> path;   
        int count=-1;
        show_path(path,parent,dest,count);
        cout<<count<<endl;
        int n=path.size();
        for(int i=0;i<n-1;i++){
            cout<<path[i]<<" "<<path[i+1]<<endl;
        }
    }
}

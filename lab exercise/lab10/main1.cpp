# include<bits/stdc++.h>
using namespace std;
bool No_circuit(const vector<vector<int>>& adj,const vector<tuple<int,int,double,double>>& edges,vector<int>& d,vector<int>& f,vector<int>& parent,int& time,int u){ 
    d[u]=++time;
    for(int i=0;i<adj[u].size();i++){
        int v=get<1>(edges[adj[u][i]]);
        if(d[v]!=-1&&f[v]==-1){ 
            return false;
        }
        else if(d[v]==-1){
            parent[v]=u;
            if(!No_circuit(adj,edges,d,f,parent,time,v)){
                return false;
            }
        }
    }
    f[u]=++time;
    return true;;
}

bool DFS(const vector<vector<int>>& adj,const vector<tuple<int,int,double,double>>& edges,vector<int>& d,vector<int>& f,vector<int>& parent){
    int time=0;  //timestamp
    int n=d.size();
    for(int i=1;i<n;i++){
        if(d[i]==-1){
            if(!No_circuit(adj,edges,d,f,parent,time,i)){
                return false;
            }
        }
    }
    return true;
}
bool Neg_Detect(const vector<vector<int>>& adj,const vector<tuple<int,int,double,double>>& edges,double ans,int n){
    stack<int> q;
    for(int i=1;i<=n;i++){
        q.push(i);
    }
    vector<double> d(n+1,0);
    vector<bool> inq(n+1,true);
    vector<int> cnt(n+1,0);
    bool hasneg=false;
    while(!q.empty()){
        int u=q.top();
        q.pop();
        inq[u]=false;
        for(int i=0;i<adj[u].size();i++){
            int v=get<1>(edges[adj[u][i]]);
            double w=get<3>(edges[adj[u][i]])*ans-get<2>(edges[adj[u][i]]);
            if(d[v]>d[u]+w){
                d[v]=d[u]+w;
                if(!inq[v]){
                    q.push(v);
                    inq[v]=true;
                }
                cnt[v]++;
                if(cnt[v]>=n){ //有负环。无最短路，会无限循环，强制跳出
                    hasneg=true;
                    break;
                }
            }
        }
        if(hasneg){
            return true;
        }
    }
    return false;
}
int main(){
    int m,n;
    cin>>n>>m;
    vector<vector<int>> adj(n+1);
    vector<tuple<int,int,double,double>> edges(m+1); //a,b,v,p
    vector<int> d(n+1,-1);
    vector<int> f(n+1,-1);
    vector<int> parent(n+1,-1);
    for(int i=0;i<m;i++){
        int a,b,v;
        cin>>a>>b>>v;
        adj[a].push_back(i);
        get<0>(edges[i])=a;
        get<1>(edges[i])=b;
        get<2>(edges[i])=v;
    }
    bool is_DAG=DFS(adj,edges,d,f,parent);
    if(is_DAG){
        cout<<-1<<endl;
        return 0;
    }
    for(int i=0;i<m;i++){   
        int p;
        cin>>p;
        get<3>(edges[i])=p;
    }
    for(int i=1;i<=m;i++){
        int a=get<0>(edges[i]);
        int b=get<1>(edges[i]);
        int v=get<2>(edges[i]);
        int p=get<3>(edges[i]);

    }
    double top=200;
    double down=0;
    while(top-down>1e-3){
        double ans=(top+down)/2;
        bool hasneg=Neg_Detect(adj,edges,ans,n);
        if(hasneg){
            down=ans;
        }else{
            top=ans;
        }
    }
    cout << fixed << setprecision(1) << (top+down)/2 <<endl;
}
# include<bits/stdc++.h>
using namespace std;
void dfs_dis(const vector<vector<int>>& children,vector<long long>& distance,int u,int parent){
    if(parent!=-1){
        distance[u]+=distance[parent];
        
    }
    for(int v: children[u]){
        dfs_dis(children,distance,v,u);
    }
}
void DFS(vector<vector<vector<long long>>>& dp,int u,int k,const vector<vector<int>>& ancestor,const vector<vector<int>>& children,const vector<long long>& distance,const vector<long long>& weight){
    // 处理叶子节点
    if(children[u].empty()){
        for(int j:ancestor[u]){
            dp[u][0][j]=weight[u]*(distance[u]-distance[j]);
            dp[u][1][j]=0;
        }
        return;
    }

    // 处理非叶子节点，递归地先解决好其所有子节点的所有情况
    for(int t:children[u]){
        DFS(dp,t,k,ancestor,children,distance,weight);
    }   

    // 已解决好所有子节点，现在处理当前节点的所有情况
    if(u==0){ //根节点特殊处理
        for(int i=0;i<=k;i++){
            vector<long long> tmp(i+1,1e18);
            tmp[0] = 0;
            for(int v:children[u]){
            vector<long long> newtmp(i+1,1e18);
            for(int used=0;used<=i;used++){
                for(int give=0;give<=i-used;give++){
                    newtmp[give+used]=min(newtmp[give+used],tmp[used]+dp[v][give][u]);
                }
            }
            tmp=move(newtmp);
            }
            dp[0][i][0]=tmp[i]; 
        }   
        return;
    }
    for(int i=0;i<=k;i++){
        if(i==0){
            for(int j:ancestor[u]){
                dp[u][i][j]=weight[u]*(distance[u]-distance[j]);
                for(int v:children[u]){
                    dp[u][i][j]+=dp[v][0][j];
                }
            }
            continue;
        }
        // 选u作为sub center
        // dp[u][i]=孩子们k_t加起来=i-1的所有情况中的最小总代价。用背包解决
        vector<long long> tmp(i+1,1e18);
        tmp[0] = 0; 
        for(int v:children[u]){
            vector<long long> newtmp(i+1,1e18);
            for(int used=0;used<=i-1;used++){
                for(int give=0;give<=i-1-used;give++){
                    newtmp[give+used]=min(newtmp[give+used],tmp[used]+dp[v][give][u]);
                }
            }
            tmp=move(newtmp);
        }
        for(int j:ancestor[u]){
            dp[u][i][j]=min(dp[u][i][j],tmp[i-1]);
        }
        
        // 不选u作为sub center
        // dp[u][i]=孩子们k_t加起来=i的所有情况中的最小总代价。用背包解决
        vector<int> nodes = ancestor[u];
        nodes.push_back(u);
        for(int j:ancestor[u] ){
            vector<long long> tmp(i+1,1e18);
            tmp[0] = 0; 
            for(int v:children[u]){
                vector<long long> newtmp(i+1,1e18);
                for(int used=0;used<=i;used++){
                    for(int give=0;give<=i-used;give++){
                        newtmp[give+used]=min(newtmp[give+used],tmp[used]+dp[v][give][j]);
                    }
                }
                tmp=move(newtmp);
            }
            dp[u][i][j]=min(dp[u][i][j],tmp[i]+weight[u]*(distance[u]-distance[j]));
        }
    }
}
int main(){
    int n,k;
    cin>>n>>k;
    vector<int> parent(n+1,-1);
    vector<vector<int>> children(n+1);
    vector<long long> distance(n+1,0); //每个点离根节点的距离
    vector<long long> weight(n+1); //每个点的货物量
    vector<vector<int>> ancestor(n+1);
    for(int i=1;i<=n;i++){
        int w,p,d;
        cin>>w>>p>>d;
        parent[i]=p;
        children[p].push_back(i);
        distance[i]=d;
        weight[i]=w;
    }
    dfs_dis(children,distance,0,-1);
    for(int i=1;i<=n;i++){
        int temp=parent[i];
        while(temp!=-1){
            ancestor[i].push_back(temp);
            temp=parent[temp];
        }
    }
    vector<vector<vector<long long>>> dp(n+1,vector<vector<long long>>((k+1),vector<long long>(n+1,1e18)));
    DFS(dp,0,k,ancestor,children,distance,weight);
    cout<<dp[0][k][0]<<endl;
}
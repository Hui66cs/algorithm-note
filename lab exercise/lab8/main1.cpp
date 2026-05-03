# include <bits/stdc++.h>
using namespace std;
int Knapsack(const vector<int>& weight,const vector<int>& value,int M,int N){
    //M:背包容量  N：物品个数
    vector<vector<int>> dp(2,vector<int>(M+1,0)); //dp[i][j]：用1~i号物品在不超过j容量的情况下能选出的最大总价值
    for(int i=1;i<=N;i++){
        for(int j=1;j<=M;j++){
            dp[0][j]=dp[1][j];
            dp[1][j]=0;
        }
        for(int j=0;j<=M;j++){
            if(weight[i]>j){
                dp[1][j]=dp[0][j];
            }else{
                dp[1][j]=max(dp[0][j],value[i]+dp[0][j-weight[i]]);
            }
        }
    }
    return dp[1][M];
}
int main(){
    int N,C; 
    cin>>N>>C;
    vector<int> weight(N+1);
    vector<int> value(N+1);
    vector<int> q(N+1);
    for(int i=1;i<=N;i++){
        cin>>weight[i]>>value[i]>>q[i];
    }
    int newN=N;
    for(int i=1;i<=N;i++){
        if(q[i]>1){
            int j=2;
            q[i]--;
            for(;j<=q[i];j<<=1){
                weight.push_back(j*weight[i]);
                value.push_back(j*value[i]);
                newN++;
                q[i]-=j;
            }
            if(q[i]>0){
                weight.push_back(q[i]*weight[i]);
                value.push_back(q[i]*value[i]);
                newN++;
            }
        }
    }
    N=newN;
    cout<<Knapsack(weight,value,C,N)<<endl;
}
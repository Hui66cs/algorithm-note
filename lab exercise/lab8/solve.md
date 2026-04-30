## exercise1 多重背包
这相比普通的0-1背包，可以重复选物品，但每个物品最多选q[i]个
$$
dp[i][j] =
\begin{cases}
dp[i-1][j] & j < w_i \\
\max_{0 \le k \le q[i],\; k w_i \le j} \left( dp[i-1][j - k w_i] + k v_i \right) & j \ge w_i
\end{cases}
$$

朴素实现方法如下，但时间复杂度为$O(Q*C*N)$，Q为q_max,即数量最多物品的数量

```cpp
# include <bits/stdc++.h>
using namespace std;
int Knapsack(const vector<int>& weight,const vector<int>& value,const vector<int>& q,int M,int N){
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
                for(int k=2;k<=q[i];k++){
                    if(k*weight[i]<=j){
                        dp[1][j]=max(dp[1][j],k*value[i]+dp[0][j-k*weight[i]]);
                    }
                }
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
    cout<<Knapsack(weight,value,q,C,N)<<endl;
}
```

**优化**
对于每个数量为q[i]的物品i，我可以将其按二进制拆分。由于二进制的特性，可以组合成0~q[i]的任意整数，这保证了等价性。
如对于13，将其拆分成1+2+4+6（最后这个6是剩余的，即13-1-2-4=6） 视作4个独立的物品，然后再用普通的0-1背包做即可。
这样，时间复杂度就被优化成$O(logQ * C* N)$了

```cpp
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
```

## CF118D Caesar's Legions
$dp[i][j][k][t]$:已放了i个步兵，j个骑兵，其中末尾为连续的k个步兵或连续的t个骑兵
继续放步兵：$dp[i][j][k][0]=dp[i-1][j][k-1][0],2 \le k \le k1$
开始放步兵：$dp[i][j][1][0]=\sum_{1\le y \le k2} dp[i-1][j][0][y]$
继续放骑兵：$dp[i][j][0][t]=dp[i][j-1][0][t-1],2 \le t \le k2$
开始放骑兵：$dp[i][j][0][1]=\sum_{1\le x \le k1} dp[i][j-1][x][0]$
最终的方案数：$\sum dp[n1][n2][x][0]+\sum dp[n1][n2][0][y]$

```cpp
# include <bits/stdc++.h>
using namespace std;
int main(){
    int n1,n2,k1,k2;
    cin>>n1>>n2>>k1>>k2;
    vector<vector<vector<vector<int>>>> dp(n1 + 1,vector<vector<vector<int>>>(n2 + 1,vector<vector<int>>(k1 + 1,vector<int>(k2 + 1, 0))));
    for(int i=1;i<=min(k1,n1);i++){
        dp[i][0][i][0]=1;
    }
    for(int i=1;i<=min(k2,n2);i++){
        dp[0][i][0][i]=1;
    }
    for(int i=1;i<=n1;i++){
        for(int j=1;j<=n2;j++){
            for(int k=1;k<=k1;k++){
                if(k==1){
                    for(int t=1;t<=k2;t++){
                        dp[i][j][k][0]=(dp[i][j][k][0]+dp[i-1][j][0][t])%100000000;
                    }
                }else{
                    dp[i][j][k][0]=dp[i-1][j][k-1][0]%100000000;
                }
            }
            for(int t=1;t<=k2;t++){
                if(t==1){
                    for(int k=1;k<=k1;k++){
                        dp[i][j][0][t]=(dp[i][j][0][t]+dp[i][j-1][k][0])%100000000;
                    }
                }else{
                    dp[i][j][0][t]=dp[i][j-1][0][t-1]%100000000;
                }
            }
        }
    }
    int ans=0;
    for(int i=1;i<=k1;i++){
        ans+=dp[n1][n2][i][0];
        ans%=100000000;
    }
    for(int i=1;i<=k2;i++){
        ans+=dp[n1][n2][0][i];
        ans%=100000000;
    }
    cout<<ans<<endl;
}
```

## exercise2
定义排队方式为从头开始安排学生。将男生定义为1，女生定义为-1.任何连续段的绝对值都要小于等于k。
$dp[i][j][mx][mn]$:已放入i个男生，j个女生。且从开头到当前的最大前缀和为mx，从开头到当前的最小前缀和为nx。
从开头到当前的前缀和为i-j。假设从开头到x的前缀和为s[x],那么对于任意一个连续段[x,y],他的前缀和=s[y]-s[x-1]。为了保证每一段前缀和绝对值$\le k$，只需保证最大前缀和-最小前缀和$\le k$即可（因为一段前缀和小于等于k也是需要最小-最大$\ge -k$,是等价的）。因此维护$mx-nx \le k$合法就行了。
### 状态转移
加入男生：$dp[i][j][max((i-j),mx)][mn]+=dp[i-1][j][mx][mn]$
加入女生：$dp[i][j][mx][min((i-j),mn)]+=dp[i][j-1][mx][mn]$
### 实现
由于前缀和最小可达-100，为了在数组中方便表示，考虑+100进行offset。即数组下标写成dp[i][j][mx+100][mn+100]


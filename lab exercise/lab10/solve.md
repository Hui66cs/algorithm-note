## exercise1
### 题意
在图中找一个有向环，使$\frac{\sum v}{\sum p}$尽可能大
### 解法
0. 将图信息的v和p存好
1. 将原图过一遍dfs判断是否有环，如果无环直接输出-1结束，否则进入下面的二分答案
2. 让初始ans为100.上界top是200，下界down是0
3. 每次将图中每条边(ai,bi)的边权更新为ans*p[i]-v[i]
跑一遍SPFA判负环。若有负环，说明存在环$ans*\sum p-\sum v<0$,即$\frac{\sum v}{\sum p}>ans$，那么ans猜小了，让down=ans，ans=(ans+top)/2。若无负环，让top=ans，ans=(ans+down)/2。当top-down<=1e-3时结束，输出（top+down）/2保留一位小数的结果
**注意这题负环检测的时候可以用栈优化来提高效率**

## exercise2
### 题意
给定一个含有n+1个节点的有向树。节点编号为0~n，其中0为根节点，额外可挑选m个节点作为sub-center。所有货物都要沿有向边传递到0或者sub-center。
货物经过边的cost为w*s，其中w为货物量，s为经过的距离。求一种设sub-center的方案，是总cost尽量小
### 解法（树形DP）
dp[u][k][top]：在u的子树中选k个sub-center，且u上方最近的center或sub-center是top时，u子树的最小总cost(包括自己)
初始化：反向建图，存每个点的children
目标：dp[0][k][0]
用DFS进行遍历，遍历到u时：
- u选为sub-center，则对于其子节点v,$$dp[u][k][top]  
=  \min_{\sum t_j = k-1}  \sum_j dp[v_j][t_j][u]$$其中u的所有子节点的tj之和为k-1
- u不选为sub-center，则对于其子节点v,$$dp[u][k][top]  =  w[u](s[u]-s[top])  +  \min_{\sum t_j = k}  \sum_j dp[v_j][t_j][top]$$其中u的所有子节点的tj之和为k

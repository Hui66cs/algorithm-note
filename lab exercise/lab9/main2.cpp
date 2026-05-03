#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin>>s;
    int m=s.size();
    vector<vector<int>> dp(m,vector<int>(m));  //任何方式下的最短长度
    vector<vector<int>> g(m,vector<int>(m)); //不使用B的最短长度
    for(int i=0;i<m;i++){  
        for(int j=i;j<m;j++){
            dp[i][j]=j-i+1;
            g[i][j]=j-i+1;
        }
    }
    for(int l=1;l<=m;l++){ //l为长度，l=j-i+1
        for(int i=0;i+l-1<m;i++){
            //尝试完全不含B的构造方式
            //尝试分段
            if(l>1){
                g[i][i+l-1]=min(g[i][i+l-1],g[i][i+l-2]+1);
                //前后两半相同
                if(l%2==0){
                    bool g_valid=true;
                    int mid=i+l/2;
                    for(int j=0;j<l/2;j++){
                        if(s[i+j]!=s[mid+j]){
                            g_valid=false;
                        }
                    }
                    if(g_valid){
                        g[i][i+l-1]=min(g[i][i+l-1],g[i][mid-1]+1);
                    }
                }
            }


            //尝试分段
            for(int j=i;j<i+l-1;j++){
                dp[i][i+l-1]=min(dp[i][i+l-1],dp[i][j]+dp[j+1][i+l-1]);
            }
            //尝试由一个循环节构造
            for(int j=1;j<=l/2;j++){ //循环节长度 
                //长度不是倍数，直接跳过
                if(l%j!=0){
                    continue;
                }
                //节数不是2的幂次方，也跳过
                int num=l/j;
                int power=0; //幂次
                int temp=1;
                while(temp<num){
                    temp=temp*2;
                    power+=1;
                }
                if(temp!=num){
                    continue;
                }
                bool valid=true;
                //检验各个循环节的对应位置是否完全一样，否则不合法
                for(int k=1;k<l/j;k++){ //遍历每个循环节
                    for(int t=0;t<j;t++){ //遍历循环节每个位置。
                        if(s[i+t]!=s[i+k*j+t]){
                            valid=false;
                            break;
                        }
                    }
                }
                if(valid){
                    if(i==0){
                        dp[i][i+l-1]=min(dp[i][i+l-1],g[i][i+j-1]+power);
                    }else{
                        dp[i][i+l-1]=min(dp[i][i+l-1],g[i][i+j-1]+power+1);
                    }
                }
            }
        }
    }
    cout<<dp[0][m-1]<<endl;
}
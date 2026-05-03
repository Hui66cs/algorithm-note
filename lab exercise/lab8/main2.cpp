# include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m,k;
    cin>>n>>m>>k;
    vector<vector<vector<vector<int>>>> dp(2,vector<vector<vector<int>>>(m + 1,vector<vector<int>>(201,vector<int>(201, 0))));
    dp[1][0][100][100]=1;
    for(int i=0;i<=n;i++){
        for(int j=0;j<=m;j++){
            for(int h=0;h<=200;h++){
                for(int a=0;a<=200;a++){
                    dp[0][j][h][a]=dp[1][j][h][a];
                    dp[1][j][h][a]=0;
                }
            }
        }
        for(int j=0;j<=m;j++){
            int cursum=i-j;
            if(cursum>=-k && cursum<=k){
                for(int mx=-100;mx<=100;mx++){
                    for(int mn=-100;mn<=100;mn++){
                        if(i<n){  //加男生
                            int nxt = cursum + 1;
                            if(nxt-mn<=k){
                                dp[1][j][max(nxt+100,mx+100)][mn+100]=(dp[1][j][max(nxt+100,mx+100)][mn+100]+dp[0][j][mx+100][mn+100])%12345678;
                            }
                        }
                        if(j<m){  //加女生
                            int nxt = cursum - 1;
                            if(mx-nxt>k){
                                continue;
                            }
                            dp[0][j+1][mx+100][min(mn+100,nxt+100)]=(dp[0][j+1][mx+100][min(mn+100,nxt+100)]+dp[0][j][mx+100][mn+100])%12345678;
                        }
                    }
                }
            }
        }
    }
    int ans=0;
    for(int i=0;i<=200;i++){
        for(int j=0;j<=200;j++){
            ans=(ans+dp[0][m][i][j])%12345678;
        }
    }
    cout<<ans<<endl;
}
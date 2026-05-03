#include <bits/stdc++.h>
using namespace std;
int main(){
    string s1,s2;
    cin>>s1>>s2;
    int m=s1.size();
    int n=s2.size();
    int c1,c2,c3,c4,c5;
    cin>>c1>>c2>>c3>>c4>>c5;
    vector<vector<int>> dp(m+1,vector<int>(3,INT_MAX));
    vector<vector<unsigned char>> dop(m+1,vector<unsigned char>(n+1,'0'));
    //dop中，1为delete，2为replace，3为copy，4为insert，5为swap，6为finish
    int pre=-1;
    for(int j=0;j<=n;j++){
        for(int i=0;i<=m;i++){
            dp[i][0]=dp[i][1];
            dp[i][1]=dp[i][2];
            dp[i][2]=INT_MAX;
        }
        for(int i=0;i<=m;i++){
            if(i==0 && j==0){
                dp[0][0]=0;
            }
            if(dp[i][0]==INT_MAX){
                continue;
            }
            if(i<m && j<n){
                //replace
                if(dp[i][0]+c2<dp[i+1][1]){  //说明这个操作代价更小
                    dp[i+1][1]=dp[i][0]+c2;
                    dop[i+1][j+1]='2';
                }
                //copy
                if(s1[i]==s2[j]){
                    if(dp[i][0]+c3<dp[i+1][1]){
                        dp[i+1][1]=dp[i][0]+c3;
                        dop[i+1][j+1]='3';
                    }
                }
                //swap
                if(i<m-1 && j<n-1 && s1[i]==s2[j+1] && s1[i+1]==s2[j]){
                    if(dp[i][0]+c5<dp[i+2][2]){
                        dp[i+2][2]=dp[i][0]+c5;
                        dop[i+2][j+2]='5';
                    }
                }
                //insert
                if(dp[i][0]+c4<dp[i][1]){ 
                    dp[i][1]=dp[i][0]+c4;
                    dop[i][j+1]='4';
                }
                //delete
                if(dp[i][0]+c1<dp[i+1][0]){ 
                    dp[i+1][0]=dp[i][0]+c1;
                    dop[i+1][j]='1';
                }
            }else if(i<m && j==n){
                //finish
                if(dp[i][0]+(m-i)*c1-1<dp[m][0]){
                    dp[m][0]=dp[i][0]+(m-i)*c1-1;
                    dop[m][n]='6';
                    pre=m-i;
                }
            }else if(j<n){ 
                //insert
                if(dp[i][0]+c4<dp[i][1]){ 
                    dp[i][1]=dp[i][0]+c4;
                    dop[i][j+1]='4';
                }
            }else if(i<m){
                //delete
                if(dp[i][0]+c1<dp[i+1][0]){ 
                    dp[i+1][0]=dp[i][0]+c1;
                    dop[i+1][j]='1';
                }
            }
        }
    }
    cout<<dp[m][0]<<endl;
    vector<string> op;
    int i=m;
    int j=n;
    //second中，1为delete，2为replace，3为copy，4为insert，5为swap，6为finish
    while(i!=0 || j!=0){
        if(dop[i][j]=='1'){
            op.push_back("delete");
            i-=1;
        }else if(dop[i][j]=='2'){
            op.push_back("replace");
            i-=1;
            j-=1;
        }else if(dop[i][j]=='3'){
            op.push_back("copy");
            i-=1;
            j-=1;
        }else if(dop[i][j]=='4'){
            op.push_back("insert");
            j-=1;
        }else if(dop[i][j]=='5'){
            op.push_back("swap");
            i-=2;
            j-=2;
        }else if(dop[i][j]=='6'){
            op.push_back("finish");
            i-=pre;
        }
    }
    for(int i=op.size()-1;i>=0;i--){
        cout<<op[i]<<endl;
    }
    cout<<endl;
}
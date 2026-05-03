## exercise1(区间DP)

### 题意
给定source（长度为m） 和 target（长度为n）字符串和以下各个操作的cost，求由source构建target（从空字符st开始构建）的最小总代价：
1. delete：删除source的第一个字符
2. replace：去掉source的第一个字符，并给st末尾加一个任意字符
3. copy：去掉source的第一个字符，并给st末尾加一个同样的字符
4. insert：在st末尾加一个任意字符
5. swap：去掉source的前两个字符，并把他们交换顺序后加到st末尾
6. finish：当st和target一致且source不为空，将source清空，代价为(source剩余长度*delete的cost)-1

### dp思路
$dp[i][j]:已删去原source的前i个字符，st==target前j个字符所花费的最小总代价$。实际大小数组大小为(m+1)*(n+1),初始化为INF，dp[0][0]=0。
令这5个操作的代价分别为c1、c2、c3、c4、c5
若满足(j<n): 尝试insert：dp[i][j+1]=min(dp[i][j+1],dp[i][j]+c4)
若满足(i<m)：尝试delete: dp[i+1][j]=min(dp[i+1][j],dp[i][j]+c1)
若满足(i<m && j<n)再尝试下面几种操作：
1. 如果这一步采用replace：dp[i+1][j+1]=min(dp[i+1][j+1],dp[i][j]+c2)
2. 如果这一步采用copy,需满足source[i]==target[j]（字符串下标从0开始）：dp[i+1][j+1]=min(dp[i+1][j+1],dp[i][j]+c3)
3. 如果这一步采用swap,需满足(i<m-1 , j<n-1 , source[i]==targer[j+1] , source[i+1]==target[j])：dp[i+2][j+2]=min(dp[i+2][j+2],dp[i][j]+c5)
若(i<m && j==n)：尝试finish，dp[m][n]=dp[i][n]+(m-i)*c1-1
若(i==m && j==n)：直接结束

为了路径回溯，每次更新dp[i][j]的最小值时，用pair顺便记录操作.

朴素实现，内存爆炸
```cpp
#include <bits/stdc++.h>
using namespace std;
int main(){
    string s1,s2;
    cin>>s1>>s2;
    int m=s1.size();
    int n=s2.size();
    int c1,c2,c3,c4,c5;
    cin>>c1>>c2>>c3>>c4>>c5;
    vector<vector<pair<int,int>>> dp(m+1,vector<pair<int,int>>(n+1,make_pair(INT_MAX,0)));
    //second中，1为delete，2为replace，3为copy，4为insert，5为swap，6为finish
    dp[0][0]=make_pair(0,0);
    int pre=-1;
    for(int i=0;i<=m;i++){
        for(int j=0;j<=n;j++){
            if(dp[i][j].first==INT_MAX){ //防止INT_MAX+cost溢出成负数
                continue;
            }
            if(i<m && j<n){
                //replace
                if(dp[i][j].first+c2<dp[i+1][j+1].first){  //说明这个操作代价更小
                    dp[i+1][j+1].first=dp[i][j].first+c2;
                    dp[i+1][j+1].second=2;
                }
                //copy
                if(s1[i]==s2[j]){
                    if(dp[i][j].first+c3<dp[i+1][j+1].first){
                        dp[i+1][j+1].first=dp[i][j].first+c3;
                        dp[i+1][j+1].second=3;
                    }
                }
                //swap
                if(i<m-1 && j<n-1 && s1[i]==s2[j+1] && s1[i+1]==s2[j]){
                    if(dp[i][j].first+c5<dp[i+2][j+2].first){
                        dp[i+2][j+2].first=dp[i][j].first+c5;
                        dp[i+2][j+2].second=5;
                    }
                }
                //insert
                if(dp[i][j].first+c4<dp[i][j+1].first){ 
                    dp[i][j+1].first=dp[i][j].first+c4;
                    dp[i][j+1].second=4;
                }
                //delete
                if(dp[i][j].first+c1<dp[i+1][j].first){ 
                    dp[i+1][j].first=dp[i][j].first+c1;
                    dp[i+1][j].second=1;
                }
            }else if(i<m && j==n){
                //finish
                if(dp[i][n].first+(m-i)*c1-1<dp[m][n].first){
                    dp[m][n].first=dp[i][n].first+(m-i)*c1-1;
                    dp[m][n].second=6;
                    pre=m-i;
                }
            }else if(j<n){ 
                //insert
                if(dp[i][j].first+c4<dp[i][j+1].first){ 
                    dp[i][j+1].first=dp[i][j].first+c4;
                    dp[i][j+1].second=4;
                }
            }else if(i<m){
                //delete
                if(dp[i][j].first+c1<dp[i+1][j].first){ 
                    dp[i+1][j].first=dp[i][j].first+c1;
                    dp[i+1][j].second=1;
                }
            }
        }
    }
    cout<<dp[m][n].first<<endl;
    vector<string> op;
    int i=m;
    int j=n;
    //second中，1为delete，2为replace，3为copy，4为insert，5为swap，6为finish
    while(i!=0 || j!=0){
        if(dp[i][j].second==1){
            op.push_back("delete");
            i-=1;
        }else if(dp[i][j].second==2){
            op.push_back("replace");
            i-=1;
            j-=1;
        }else if(dp[i][j].second==3){
            op.push_back("copy");
            i-=1;
            j-=1;
        }else if(dp[i][j].second==4){
            op.push_back("insert");
            j-=1;
        }else if(dp[i][j].second==5){
            op.push_back("swap");
            i-=2;
            j-=2;
        }else if(dp[i][j].second==6){
            op.push_back("finish");
            i-=pre;
        }
    }
    for(int i=op.size()-1;i>=0;i--){
        cout<<op[i]<<endl;
    }
    cout<<endl;
}
```

这里取巧了，没用Hirschberg算法把空间复杂度降到线性。而是把dp代价从O(nm)变成滚动数组O(m*3)，几乎线性。但为了回溯，操作还是用O(mn)记录，但不用int，改成unsigned char，只用了原本1/4的空间，足够通过这题。
```cpp
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
```

## P4302 字符串折叠 (区间DP)
### 题意
对于字符串的重复部分，可以用用“数字(字符串)”的形式进行折叠。如 3(ab)代表ababab。
给定一个长度为m的字符串s，求其在能利用折叠的条件下所能达到的最短长度。注意括号也算长度

### 解法
dp[i][j]:从s[i]到s[j]这个字符串能化成的最短长度。数组大小为m*m,每个j>=i的格子初始化为j-i+1，最终结果为s[0][m-1].
按区间长度递增进行遍历。
状态转移：
1. s[i][j]不是只由一个循环节构成，则可以把他分成两段：dp[i][j]=min(dp[i][k]+dp[k+1][j])
2. s[i][j]是只由一个循环节构成的，则以s[i]为起点，长度递增地尝试作为循环节。dp[i][j]=min(dp[i][k]+2+digit(重复次数))

```cpp
#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin>>s;
    int m=s.size();
    vector<vector<int>> dp(m,vector<int>(m));
    for(int i=0;i<m;i++){  
        for(int j=i;j<m;j++){
            dp[i][j]=j-i+1;
        }
    }
    for(int l=1;l<=m;l++){ //l为长度，l=j-i+1
        for(int i=0;i+l-1<m;i++){
            //尝试分段
            for(int j=i;j<i+l-1;j++){
                dp[i][i+l-1]=min(dp[i][i+l-1],dp[i][j]+dp[j+1][i+l-1]);
            }
            //尝试由一个循环节构造
            for(int j=1;j<=l/2;j++){ //循环节长度 
                if(l%j!=0){
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
                    int digit=1;
                    int temp=l/j;
                    while(temp/10>0){
                        digit++;
                        temp/=10;
                    }
                    dp[i][i+l-1]=min(dp[i][i+l-1],dp[i][i+j-1]+2+digit);
                }
            }
        }
    }
    cout<<dp[0][m-1]<<endl;
}
```

## exercise2 (区间DP)
### 题意
本质和P4302基本一样。但在自己是单独循环节构成(不分段)的时候，情况比较特别。
首先是他的循环段数必须是2的幂次方。
1. 如果这段字符串是从s[0]开始的，则开头会省略掉B。那么XXXRYR 会解读成XXXXXX'Y'XXXXXXY
2. 如果不是从s[0]开始的，则开头会有一个B
所以在单循环节折叠时，我们要先不管B，做好处理后再判断开头是不是要补B
而仅靠dp[][]无法得到不带B的循环节部分的长度，因此引入g[][]来专门为了获取这个信息。
g[i][j]:不考虑开头B的情况下，s[i~j]的最短折叠长度
状态转移：
1. 这一段末尾不是R，则由前面段的长度+末尾这个字符：g[i][j]=g[i][j-1]+1
2. 末尾是R，则该字符串是由复制前一半字符串得到的，长度为前一般字符串长度+R：g[i][j]=g[i][mid]+1 。由于他会根据长度bottom up得到，不需要更多处理

而对于dp的更新，在有单独需环节构成的情况下需要修改成：
dp[i][j]=min(dp[i][j],g(循环节)+$\log_{2}{循环段数}$+(i==0?:1))
```cpp
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
```
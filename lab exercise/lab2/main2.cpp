#include <bits/stdc++.h>
using namespace std;

void make_preference(const vector<vector<int>>& weight,vector<vector<int>>& preference,char rule,int start){
    int n=weight.size()-1;
    if(rule=='I' && start<=n){  // 左边升序排名，右边查找
        for(int i=1;i<n+1;i++){
            for(int j=n+1;j<2*n+1;j++){
                preference[i][j]=j;
            }
            sort(preference[i].begin()+n+1,preference[i].end(),[& weight,i,n](int a,int b){
                return weight[i][a-n]<weight[i][b-n];
            });
        }
        for(int i=n+1;i<2*n+1;i++){
            for(int j=1;j<n+1;j++){
                preference[i][j]=weight[j][i-n];
            }
        }
    }else if(rule=='I' && start>n){  // 右边升序排名，左边查找
        for(int i=1;i<n+1;i++){
            for(int j=n+1;j<2*n+1;j++){
                preference[i][j]=weight[i][j-n];
            }
        }
        for(int i=n+1;i<2*n+1;i++){
            for(int j=1;j<n+1;j++){
                preference[i][j]=j;
            }
            sort(preference[i].begin()+1,preference[i].end()-n,[& weight,i,n](int a,int b){
                return weight[a][i-n]<weight[b][i-n];
            });
        }
    }else if(rule=='D' && start<=n){  
        //左边降序排名，右边查找
        for(int i=1;i<n+1;i++){
            for(int j=n+1;j<2*n+1;j++){
                preference[i][j]=j;
            }
            sort(preference[i].begin()+n+1,preference[i].end(),[& weight,i,n](int a,int b){
                return weight[i][a-n]>weight[i][b-n];
            });
        }
        for(int i=n+1;i<2*n+1;i++){
            for(int j=1;j<n+1;j++){
                preference[i][j]=weight[j][i-n];
            }
        }
    }else{  //右边降序排名，左边查找
        for(int i=1;i<n+1;i++){
            for(int j=n+1;j<2*n+1;j++){
                preference[i][j]=weight[i][j-n];
            }
        }
        for(int i=n+1;i<2*n+1;i++){
            for(int j=1;j<n+1;j++){
                preference[i][j]=j;
            }
            sort(preference[i].begin()+1,preference[i].end()-n,[& weight,i,n](int a,int b){
                return weight[a][i-n]>weight[b][i-n];
            });
        }
    }
}

void Stable_Match(const vector<vector<int>>& preference,vector<int>& match,char rule,int start){
    // Alice as husband, Bob as wife
    int n=(preference.size()-1)/2;
    bool isI=false;
    if(rule=='I'){
        isI=true;
    }
    queue<int> free;
    vector<int> visit(2*n+1);
    for(int i=1;i<n+1;i++){
        visit[i]=n+1;
    }
    for(int i=n+1;i<2*n+1;i++){
        visit[i]=1;
    }
    if(start<=n){
        for(int i=1;i<n+1;i++){
            free.push(i);
        }
        while(!free.empty()){
            int hus=free.front();
            free.pop();
            if(visit[hus]>2*n){
                continue;
            }
            int wife=preference[hus][visit[hus]];
            visit[hus]++;
            if(match[wife]==0){
                match[wife]=hus;
                match[hus]=wife;
            }else{
                // if Alice choose increasing, so husband's pre is rank increasingly, wife's pre is rank decreasingly, so new one larger will be accepted
                if((isI && preference[wife][match[wife]]<preference[wife][hus]) || (!isI && preference[wife][match[wife]]>preference[wife][hus])){
                    match[match[wife]]=0;
                    free.push(match[wife]);
                    match[wife]=hus;
                    match[hus]=wife;
                }else{
                    free.push(hus);
                }
            }
        }
    }else{
        for(int i=n+1;i<2*n+1;i++){
            free.push(i);
        }
        while(!free.empty()){
            int hus=free.front();
            free.pop();
            if(visit[hus]>n){
                continue;
            }
            int wife=preference[hus][visit[hus]];
            visit[hus]++;
            if(match[wife]==0){
                match[wife]=hus;
                match[hus]=wife;
            }else{
                if((isI && preference[wife][match[wife]]<preference[wife][hus]) || (!isI && preference[wife][match[wife]]>preference[wife][hus])){
                    match[match[wife]]=0;
                    free.push(match[wife]);
                    match[wife]=hus;
                    match[hus]=wife;
                }else{
                    free.push(hus);
                }
            }
        }
    }
}
int main(){
    int t;
    cin>>t;
    while (t>0){
        t--;
        int n;
        cin>>n;
        vector<vector<int>> weight(n+1,vector<int>(n+1));  // 存初始权重信息
        vector<vector<int>> preference(2*n+1,vector<int>(2*n+1));  // 偏好列表
        for(int i=1;i<n+1;i++){
            for(int j=1;j<n+1;j++){
                cin>>weight[i][j];
            }
        }
        cout<<'B'<<endl;
        cout.flush();
        char rule;
        int start;
        cin>>rule>>start;
        vector<int> match(2*n+1,0);
        make_preference(weight,preference,rule,start);
        Stable_Match(preference,match,rule,start);
        while(start!=-1){
            cout<<match[start]<<endl;
            cout.flush();
            cin>>start;
        }
    }
}
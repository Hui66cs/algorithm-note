#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int> t(n);
    vector<int> w(n);
    vector<tuple<int,int,int>> game(n);
    // first是t（ddl），second是w（罚款），third是游戏编号
    for(int i=0;i<n;i++){
        cin>>t[i];
    }
    for(int i=0;i<n;i++){
        cin>>w[i];
    }
    for(int i=0;i<n;i++){
        get<0>(game[i])=t[i];
        get<1>(game[i])=w[i];
        get<2>(game[i])=i+1;
    }
    sort(game.begin(),game.end());
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> accept; //first是罚款，second是game编号
    int cost=0;
    for(int i=0;i<n;i++){
        if(get<0>(game[i])>accept.size()){
            accept.push(pair<int,int>(get<1>(game[i]),get<2>(game[i])));
        }else{
            int min_cost=accept.top().first;
            if(get<1>(game[i])>min_cost){
                accept.pop();
                accept.push(pair<int,int>(get<1>(game[i]),get<2>(game[i])));
            }
        }
    }
    vector<bool> c(n+1,true);
    while(!accept.empty()){
        c[accept.top().second]=false;
        accept.pop();
    }
    for(int i=1;i<n+1;i++){
        if(c[i]){
            cost+=w[i-1];
        }
    }
    cout<<cost<<endl;
}

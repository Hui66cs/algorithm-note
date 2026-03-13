#include<bits/stdc++.h>
using namespace std;
int minimum_lateness(vector<tuple<int,int,int>>& tasks){
    //course[i].first是ddl时刻，course[i].second是任务用时，course[i].third是课程编号
    sort(tasks.begin(),tasks.end());
    int n=tasks.size();
    int t=0;
    int max_delay=0;
    for(int i=0;i<n;i++){
        if(t+get<1>(tasks[i])>get<0>(tasks[i])){
            if (t+get<1>(tasks[i])-get<0>(tasks[i])>max_delay){
                max_delay=t+get<1>(tasks[i])-get<0>(tasks[i]);
            }
        }
        t+=get<1>(tasks[i]);
    }
    return max_delay;
}
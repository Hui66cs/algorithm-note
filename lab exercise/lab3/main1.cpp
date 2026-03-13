#include<bits/stdc++.h>
using namespace std;
int optimal_caching_evict(const vector<string>& Input,int k){
    // k为缓存最大容量
    int n=Input.size();
    int m=0; //m为驱逐次数
    int t=0; //t为缓存内元素个数
    unordered_map<string,queue<int>> location;  //对应字符及其出现过的索引
    priority_queue<pair<int,string>> farthest;  //first为下一次出现的索引，second为对应字符
    unordered_map<string,bool> Is_in_cache;  //是否在缓存里
    unordered_map<string,int> next_index;  //用于处理懒删除
    for(int i=0;i<n;i++){
        location[Input[i]].push(i);
        Is_in_cache[Input[i]]=false;
    }
    for(int i=0;i<n;i++){
        location[Input[i]].pop();
        int next=INT_MAX;
        if(!location[Input[i]].empty()){
            next=location[Input[i]].front();
        }
        next_index[Input[i]]=next;
        if(Is_in_cache[Input[i]]){
            farthest.push(pair<int,string>(next,Input[i]));
            continue;
        }else{
            if(t<k){
                Is_in_cache[Input[i]]=true;
                t++;
            }else{
                while(!Is_in_cache[farthest.top().second] || next_index[farthest.top().second]!=farthest.top().first){
                    farthest.pop();
                }
                string top=farthest.top().second;
                farthest.pop();
                Is_in_cache[top]=false;
                Is_in_cache[Input[i]]=true;
                m++;
            }
        }
        farthest.push(pair<int,string>(next,Input[i]));
    }
    return m;
}

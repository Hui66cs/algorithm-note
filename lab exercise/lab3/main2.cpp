#include<bits/stdc++.h>
using namespace std;
int main(){
    long long N;
    int M;
    cin>>N>>M;
    long long max_suffix_sum=0;
    long long sum=0;
    vector<tuple<long long,long long,long long>> record(M);  //first是这块的local_sum，second是这块的local_max_suffix_sum, third是这小块的重复个数
    for(int i=0;i<M;i++){
        string s;
        long long num;
        cin>>s>>num;
        long long local_max_suffix_sum=0;
        long long local_sum=0;
        for(int k=s.length()-1;k>=0;k--){  // 仅先分析单独一小块
            if(s[k]=='B'){
                local_sum+=1;
            }else{
                local_sum-=1;
            }
            local_max_suffix_sum=max(local_max_suffix_sum,local_sum);
        }
        get<0>(record[i])=local_sum;
        get<1>(record[i])=local_max_suffix_sum;
        get<2>(record[i])=num;
        sum+=local_sum*num;
    }
    if(sum>=2){
        cout<<-1<<endl;
        return 0;
    }
    long long right_block_sum=0;
    for(int i=M-1;i>=0;i--){
        if(get<0>(record[i])>=0){
            max_suffix_sum=max(right_block_sum+(get<2>(record[i])-1)*get<0>(record[i])+get<1>(record[i]),max_suffix_sum);
        }else{
            max_suffix_sum=max(right_block_sum+get<1>(record[i]),max_suffix_sum);
        }
        right_block_sum=right_block_sum+get<2>(record[i])*get<0>(record[i]);
    }
    cout<<max(0LL,max_suffix_sum-1)<<endl;
}
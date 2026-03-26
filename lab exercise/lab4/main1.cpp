#include<bits/stdc++.h>
using namespace std;
int main(){
    int N;
    int M;
    int T;
    cin>>N>>M>>T;
    unordered_map<int,tuple<int,int,int,std::list<int>::iterator>> cache; //key-(value,frequency,latest used timestamp,pointer)
    queue<pair<int,int>> latest_used_time; //key,timestamp
    unordered_map<int, list<int>> freq_list; // freqeuncy-list. In list, the key is displayed in time order
    int timestamp=0;
    int min_freq=0;
    for(int i=0;i<M;i++){
        int op;
        cin>>op;
        while(true){
            if(latest_used_time.empty()){
                break;
            }
            if(cache.count(latest_used_time.front().first)>0 && latest_used_time.front().second!=get<2>(cache[latest_used_time.front().first])){
                latest_used_time.pop();
            }else if(cache.count(latest_used_time.front().first)==0){
                latest_used_time.pop();
            }else if(timestamp-latest_used_time.front().second<=T){
                break;
            }else{
                int rm_key=latest_used_time.front().first;
                freq_list[get<1>(cache[rm_key])].erase(get<3>(cache[rm_key]));
                if(min_freq==get<1>(cache[rm_key]) && freq_list[min_freq].empty()){
                    min_freq++;
                }
                cache.erase(latest_used_time.front().first);
                latest_used_time.pop();
            }
        }
        if(op==1){
            int key;
            cin>>key;
            if(cache.count(key)==0){
                cout<<-1<<endl;
            }else{
                freq_list[get<1>(cache[key])].erase(get<3>(cache[key]));
                if(min_freq==get<1>(cache[key]) && freq_list[min_freq].empty()){
                    min_freq++;
                }
                get<1>(cache[key])++;
                freq_list[get<1>(cache[key])].push_back(key);
                get<2>(cache[key])=timestamp;
                get<3>(cache[key])=--freq_list[get<1>(cache[key])].end();
                latest_used_time.push(pair<int,int>(key,timestamp));
                cout<<get<0>(cache[key])<<endl;
            }
        }else if(op==2){
            int key;
            int value;
            cin>>key>>value;
            if(cache.count(key)==1){
                get<0>(cache[key])=value;
                freq_list[get<1>(cache[key])].erase(get<3>(cache[key]));
                if(min_freq==get<1>(cache[key]) && freq_list[min_freq].empty()){
                    min_freq++;
                }
                get<1>(cache[key])++;
                get<2>(cache[key])=timestamp;
                freq_list[get<1>(cache[key])].push_back(key);
                get<3>(cache[key])=--freq_list[get<1>(cache[key])].end();
            }else if(cache.size()<N){
                freq_list[0].push_back(key);
                cache[key]=make_tuple(value,0,timestamp,--freq_list[0].end());
                min_freq=0;
            }else{
                while(freq_list[min_freq].empty()){
                    min_freq++;
                }
                int rm_key=freq_list[min_freq].front();
                freq_list[get<1>(cache[rm_key])].erase(get<3>(cache[rm_key]));
                cache.erase(rm_key);
                freq_list[0].push_back(key);
                cache[key]=make_tuple(value,0,timestamp,--freq_list[0].end());
                min_freq=0;
            }
            latest_used_time.push(pair<int,int>(key,timestamp));
        }
        timestamp++;
    }
    int count=0;
    while(!latest_used_time.empty()){
        if(cache.count(latest_used_time.front().first)==0 || (cache.count(latest_used_time.front().first)>0 && get<2>(cache[latest_used_time.front().first])!=latest_used_time.front().second)){
            latest_used_time.pop();
            continue;
        }
        cout<<get<0>(cache[latest_used_time.front().first])<<" ";
        count++;
        latest_used_time.pop();
    }
    if(count==0){
        cout<<"empty";
    }
    cout<<endl;
}
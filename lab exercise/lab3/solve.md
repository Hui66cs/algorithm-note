## exercise1
### 解法1：按罚款大小排序，若ddl处未放，则放入ddl地方。若已放，看前面还有没有空位(从后往前)，有则放，没有则加入罚款。(贪心)
**时间复杂度$O(n^2)$**
```cpp
int main(){
    int n;
    cin>>n;
    vector<int> t(n);
    vector<int> w(n);
    vector<tuple<int,int,int>> game(n);
    // first是w（罚款），second是t（ddl），third是游戏编号
    int max_ddl=0;
    for(int i=0;i<n;i++){
        cin>>t[i];
    }
    for(int i=0;i<n;i++){
        cin>>w[i];
    }
    for(int i=0;i<n;i++){
        get<0>(game[i])=w[i];
        get<1>(game[i])=t[i];
        get<2>(game[i])=i+1;
        max_ddl=max(max_ddl,get<1>(game[i]));
    }
    sort(game.begin(),game.end(),greater<tuple<int,int,int>>());
    vector<int> schedule(max_ddl+1,-1); // 0不能安排
    int cost=0;
    for(int i=0;i<n;i++){
        if(schedule[get<1>(game[i])]==-1){
            schedule[get<1>(game[i])]=get<2>(game[i]);
        }else{
            bool lose=true;
            for(int j=get<1>(game[i]);j>=1;j--){
                if(schedule[j]==-1){
                    schedule[j]=get<2>(game[i]);
                    lose=false;
                    break;
                }
            }
            if(lose){
               cost+=get<0>(game[i]);
            }
        }
    }
    cout<<cost<<endl;
}
```

### 解法2：反悔贪心：按ddl升序排序，若当前game的ddl小于已加入的game数，则直接加入该game；否则比较已加入的game中罚款最少的那个与该game的罚款（小顶堆维护）。若该game罚款更多则将两者替换，该game加入（可以减少总体罚款）。
**时间复杂度$O(nlogn)$**
```cpp
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
```

## practise 2 （同P3615）
注意到女生是两种厕所都可用，而男生只有一种厕所能用。由于总人数为2N，要在N分钟内结束，这意味着必须每分钟厕所都要满人才能实现。
则无法做到的判断条件为：男生-女生人数>=2，这说明必有两男单独而有空位
设男生为1，女生为0
可关注整条队伍的后缀和，若某一段后缀和>1，说明这段队伍会有空位，需要调换。算出整条队伍的最大后缀和，为n则说明后面n个人男生密集，需从这个最大后缀和队伍的前一个人（比为女生，如是男生此时后缀和不是最大）换到后面的倒数第二个位置，将这小段和弄成0，因次不满意指数为n-1
$[2^{-12},2^{12}-]$
## practise1

### 题意

给定一个无向有权图，多个节点可以合并成一个省。要求每两个省之间的最短距离>=T。求最多可以划分成多少个省。

### 思路
省 -> 集合
利用并查集，一开始每个点都自己作为一个集合。然后开始遍历每一条边(u,v)，若边权>=T直接continue，否则查询u、v是否在一个集合中，若不在则将两者合并成一个集合；若在一个集合则continue。最后统计集合的个数，即为题目所求。

## practise2

### 模板题(洛谷P4716)

求最小树形图：在有向图中从根节点r出发，构造一个最小生成树，使到达所有点所经过的边权之和最小

### 朱-刘算法

1. 先遍历每一个点，然后找出其边权最小的入边，然后将其所有的入边的边权都减去这个最小边权（此时最小入边的边权为0）。然后将这条最小的入边加入集合E。
2. 第一轮遍历完之后检查图中是否出现有向环。如果有，把每个有向环当作一个super node。然后再进行一次遍历、找最小入边的过程。（迭代比递归好些）
3. 当某轮遍历结束后图中没有有向环，则退出
4. 还原真实最小树形图：对于每个超级节点s，若其入边为(u,s),连接环内的具体边是(a,v),且形成这个有向环时，环内指向这个v的边是(b,v),则将E中的(u,s)和(b,v)删去，加入(a,v)。不断重复此操作直至所有超级节点被还原。
5. 最小总代价即为所有减去的reduced cost之和

### naive实现
1. 用一个edge数组管理所有边
2. 每一轮遍历用一个数组记录每个点的最小边权入边（更新parent数组），然后再遍历一轮检测连通性，把每个点的入边减去最小边权后得到reduced cost。
3. 检查是否存在有向环，若无则跳出循环，结束；若有则将有向环合并成超级节点，即parent数组新增节点，再遍历一次边，将原本指向有向环内点的边改为指向超级节点。
4. 重复2~3的操作，直至没有有向环

膜拜题实现
```cpp
# include<bits/stdc++.h>
using namespace std;
struct Edge{
    int u;
    int v;
    int w;
    Edge() {}
    Edge(int u,int v,int w){
        this->u=u;
        this->v=v;
        this->w=w;
    }
};
int main(){
    int n;
    int m;
    int r;
    cin>>n>>m>>r;
    vector<Edge> edges(m);
    for(int i=0;i<m;i++){
        int u;
        int v;
        int w;
        cin>>u>>v>>w;
        edges[i]=Edge(u,v,w); //指向v的点u，及(u,v)的权重
    }
    int cost=0;
    int cnt=0; // 新点的个数
    while(true){
        vector<int> parent(n+1,-1); //最小树形图中指向i的点
        vector<pair<int,int>> min_in(n+1,make_pair(-1,INT_MAX)); //每个v的最小入边：(u,w)
        for(int i=0;i<m;i++){  //寻找每个v的最小入边
            int u=edges[i].u;
            int v=edges[i].v;
            int w=edges[i].w;
            if(v!=r && u!=v && min_in[v].second>w){
                min_in[v].first=u;
                min_in[v].second=w;
                parent[v]=u;
            }
        }
        bool is_connected=true;
        for(int i=1;i<=n;i++){   
            if(i != r && min_in[i].second == INT_MAX) { //若出现一个非根节点没有入边，则该图不连通（成环的话下一回合缩成超级节点能检测出来）
                cout << -1 << endl; 
                return 0;
            }
            if(i!=r){
                cost+=min_in[i].second; //计算cost
            }
        }
        vector<int> visited(n+1,0);  //用于标记这个点是从哪个点开始溯源的
        vector<int> loop_id(n+1,0);  //用于记录这个点属于第几个环,并提供其他映射
        cnt=0;
        bool has_loop=false;
        for(int i=1;i<=n;i++){  //检查是否存在有向环
            int v=i;
            while(true){
                if(v==r){ //这个点最终能通向根节点，则这个点肯定不形成环，继续检查后面
                    break;
                }else if(loop_id[v]!=0){  //自己能进入或本身在某个有向环中
                    break;
                }else if(visited[v]==i){
                    has_loop=true;
                    int origin=v;
                    cnt++;
                    loop_id[v]=cnt;
                    v=parent[v];
                    while(v!=origin){
                        loop_id[v]=cnt;
                        v=parent[v];
                    }
                    break;
                }
                visited[v]=i;
                v=parent[v];
            }
        }
        if(!has_loop){
            cout<<cost<<endl;
            break;
        }
        for(int i=0;i<m;i++){  //reduced cost
            if(edges[i].v!=r){
                edges[i].w-=min_in[edges[i].v].second;
            }
        }
        //环合并成超级节点:根据loop_id重新分配编号
        for(int i=1;i<=n;i++){
            if(loop_id[i]==0){
                cnt++;
                loop_id[i]=cnt;
            }
        }
        r=loop_id[r];
        for(int i=0;i<m;i++){
            int u1=edges[i].u;
            int v1=edges[i].v;
            int w=edges[i].w;
            int u2=loop_id[u1];
            int v2=loop_id[v1];
            edges[i]=Edge(u2,v2,w);
        }
        n=cnt;
    }
}
```

### 真practise2题意
在上述条件中，自己寻找根，并要求把最小树形图的每个点的实际入边找出。因此这道题的难点在于如何解决不定根、如何将超级节点还原
### 解法
1. 针对不定根：可以设置一个虚拟根，让其到各个点都有边，则最后确定的根就是虚拟根所指向的节点。但虚拟边权的设置需要满足几个条件：
    - 虚拟边权需要足够大：题目只能允许选择一条虚拟边。选了两条虚拟边构建出的DMST是不满足要求的(相当于选了两个根 or 图本身不连通)，需要能用cost>某个阈值判断出是否选择超过一条虚拟边。
    - 要求选择的真实根后，所得到的cost确实是最优解。
    - 同等情况偏好编号小的点做真是根：若几个真是根的总代价相同，优先选择编号小的点作为根。因此连接对应点的虚拟边边权应该按编号升序严格单调递增。
因此希望将cost的比较设定成两部分，类比小数10.01、10.02。整数部分10是大头，决定根本的最小总cost（对应第二小点）。而小数部分0.01、0.02则用于整数部分一样的情况下，尽量选择点序号小的作为根节点。因此想到令虚拟边的边权为某个巨大常数BIG+序号i，而原本真是边的边权都*n。
    针对第2点：假设以A为根的真实DMST总cost为$C_A$,以B为根的真实DMST总cost为$C_B$，且$C_A < C_B$。依题意，我们需要选A作为真实根。如今加上对应虚拟边之后，选A的总代价$C_A'=100*C_A+Big+编号a$,选B的总代价为$C_B'=100*C_B+Big+编号b $。由于只要$C_A<C_B$，即使只小1，由于乘上了n,其所造成的差距肯定大于编号a、b的最大差距n-1,算法肯定会选A。
    针对第3点：$C_A = C_B$时，相差的便只有编号了，算法会选择编号小的，满足题意
    针对第1点：BIG的作用是为了能检测是否选了两条虚拟边。不妨令BIG=n*原图所有边权之和Sum + n，这个Sum就是纯粹的原始边权的和不乘n。这样设计的话，即使原本的DMST把所有边都选了，那么只选了一条虚拟边的DMST的总Cost达到最大=n*Sum+n*Sum+n+n-1=2n*Sum+2n-1，始终小于2*BIG=2n*Sum+2n。因此可以用2*BIG作为阈值检验是否选了两条虚拟边了。
    综上所述，虚拟根指向第i个节点的虚拟边的边权=n*Sum+n+i,原本每条边的边权$w_i=n*w_i$ 

2. 超级节点的还原：同朴素朱刘，但对于每条边，加入两个属性origin_u、origin_v，用来记录一开始这条边的两端。后面在重新分配编号时，只修改u、v。每次选择最小入边后，将边加入到real_parent中，后加入的连接环边会覆盖掉原本环内的对应边。
lazytag，左偏树

# include<bits/stdc++.h>
using namespace std;
struct Edge{
    int u;
    int v;
    long long w;
    int origin_u;
    int origin_v;
    Edge() {}
    Edge(int u,int v,long long w){
        this->u=u;
        this->v=v;
        this->w=w;
    }
};
int main(){
    int n;
    int m;
    cin>>n>>m;
    vector<Edge> edges(m+n);
    long long sum=0;
    for(int i=0;i<m;i++){
        int u;
        int v;
        long long w;
        cin>>u>>v>>w;
        edges[i]=Edge(u,v,n*w); //指向v的点u，及(u,v)的权重
        edges[i].origin_u=u;
        edges[i].origin_v=v;
        sum+=w;
    }
    int real_n=n;
    int r=n; //加入虚拟根
    for(int i=0;i<n;i++){  //加入虚拟边
        edges[m+i]=Edge(r,i,n*sum+n+i);
        edges[m+i].origin_u=r;
        edges[m+i].origin_v=i;
    }
    m+=n;
    long long cost=0;
    int cnt=0; // 新点的个数
    vector<int> real_parent(n+1,-1); //每个点的真实入边
    while(true){
        vector<int> parent(n+1,-1); //最小树形图中指向i的点
        vector<pair<int,long long>> min_in(n+1,make_pair(-1,LONG_LONG_MAX)); //每个v的最小入边：(u,w)
        vector<pair<int,int>> chosen(n+1,make_pair(-1,-1)); 
        for(int i=0;i<m;i++){  //寻找每个v的最小入边
            int u=edges[i].u;
            int v=edges[i].v;
            int origin_u=edges[i].origin_u;
            int origin_v=edges[i].origin_v;
            long long w=edges[i].w;
            if(v!=r && u!=v && min_in[v].second>w){
                min_in[v].first=u;
                min_in[v].second=w;
                parent[v]=u;
                chosen[v]=make_pair(origin_u,origin_v);
            }
        }
        bool is_connected=true;
        for(int i=0;i<=n;i++){   
            if(i != r && min_in[i].second == LONG_LONG_MAX) { //若出现一个非根节点没有入边，则该图不连通（成环的话下一回合缩成超级节点能检测出来）
                cout << "impossible" << endl; 
                return 0;
            }
            if(i!=r){
                cost+=min_in[i].second; //计算cost
                real_parent[chosen[i].second]=chosen[i].first;
            }
        }
        vector<int> visited(n+1,-1);  //用于标记这个点是从哪个点开始溯源的
        vector<int> loop_id(n+1,-1);  //用于记录这个点属于第几个环,并提供其他映射
        cnt=0;
        bool has_loop=false;
        for(int i=0;i<=n;i++){  //检查是否存在有向环
            int v=i;
            while(true){
                if(v==r){ //这个点最终能通向根节点，则这个点肯定不形成环，继续检查后面
                    break;
                }else if(loop_id[v]!=-1){  //自己能进入或本身在某个有向环中
                    break;
                }else if(visited[v]==i){
                    has_loop=true;
                    int origin=v;
                    loop_id[v]=cnt;
                    v=parent[v];
                    while(v!=origin){
                        loop_id[v]=cnt;
                        v=parent[v];
                    }
                    cnt++;
                    break;
                }
                visited[v]=i;
                v=parent[v];
            }
        }
        if(!has_loop){
            break;
        }
        for(int i=0;i<m;i++){  //reduced cost
            if(edges[i].v!=r){
                edges[i].w-=min_in[edges[i].v].second;
            }
        }
        //环合并成超级节点:根据loop_id重新分配编号
        for(int i=0;i<=n;i++){
            if(loop_id[i]==-1){
                loop_id[i]=cnt;
                cnt++;
            }
        }
        r=loop_id[r];
        for(int i=0;i<m;i++){
            int u1=edges[i].u;
            int v1=edges[i].v;
            int w=edges[i].w;
            int u2=loop_id[u1];
            int v2=loop_id[v1];
            edges[i].u=u2;
            edges[i].v=v2;
        }
        n=cnt-1;
    }
    n=real_n;
    if(cost>2*(n*sum+n)){
        cout<<"impossible"<<endl;
        return 0;
    }
    int root;
    for(int i=0;i<n;i++){
        if(real_parent[i]==real_n){
            root=i;
            break;
        }
    }
    cout<<(cost-n*sum-n-root)/n<<" "<<root<<endl;
    for(int i=0;i<n;i++){
        if(i==root){
            cout<<root<<endl;
        }else{
            cout<<i<<" "<<real_parent[i]<<endl;
        }
    }
}
## practise1
### 解法
1. 预处理：将所有点按x排序。然后利用分治思想分成三种情况的最近点对：两点都在左边，两点都在右边，一点在左一点在右。
2. 每次将数组的点二分，即取arr[(i+j)/2]将空间进行分割。直到i==j，则该区域的最近点对距离返回$\infty$
3. 左右区域的最近点对距离可以由递归得到，重点在于O(n)时间combine，即计算跨越两侧的最近点对，方法如下：
4. 令$\delta=min(左侧最近点对距离，右侧最近点对距离)$，跨越两侧的最近点对一定在距离中间平面$\delta$的空间中（两侧）。
注意到对于这个空间的每个点，距其最近的点一定在离它往y轴正负半轴各自延伸$\delta$，往z轴正负半轴也各自延伸$\delta$的空间中，实际上是一个边长为$2\delta$的立方体。为保证同侧两点最短距离$>=\delta$，这个空间最多只能同时存在3*9=27个点(全部位于角上和棱中点处)。这印证了每个点的最近点对搜索只需要O(1)时间。
对于二维最近点对，这里我们直接按y排序找相邻5个点即可，但三维情况下，中间这个三维条带有y、z两个变量无限延伸，排序很难指定规则。
因此我们干脆不排序，而是利用哈希表，将每个点映射到一个点全部坐标全为整数的三维坐标系中。对于原本坐标为(x,y,z)的点，我将其映射到$(\lceil \frac{x}{\delta} \rceil,\lceil \frac{y}{\delta} \rceil,\lceil \frac{z}{\delta} \rceil)$，即以$\delta$为基。由于最近两个点距离>=$\delta$，因此每一点都不会映射到坐标系的同一点。现在对于求距(x,y,z)最近的点，只需要与$(\lceil \frac{x}{\delta} \rceil,\lceil \frac{y}{\delta} \rceil,\lceil \frac{z}{\delta} \rceil)$的x、y、z方向上的相邻点(共6个)对应的原坐标比较即可。这样就实现O(n)时间的combine了

```cpp
# include<bits/stdc++.h>
using namespace std;
struct grid{
    long long x, y, z;
    grid(){}
    grid(long long x,long long y,long long z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
    bool operator==(const grid& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
struct GridHash {
    size_t operator()(const grid& g) const {
        size_t h1 = std::hash<long long>{}(g.x);
        size_t h2 = std::hash<long long>{}(g.y);
        size_t h3 = std::hash<long long>{}(g.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
double distance(const grid& a, const grid& b) {
    double dx = (double)a.x - b.x;
    double dy = (double)a.y - b.y;
    double dz = (double)a.z - b.z; 
    return sqrt(dx * dx + dy * dy + dz * dz);
}
double find(vector<grid>& point,int i,int j){
    if(j==i){
        return 1e12;
    }
    int mid=(i+j)/2;
    long long mid_x=point[mid].x;
    double left=find(point,i,mid);
    double right=find(point,mid+1,j);
    double delta=min(left,right);
    if(delta==0){
        return 0;
    }
    vector<grid> select;
    for(int k=i;k<=j;k++){
        if(point[k].x<=delta+mid_x && point[k].x>=mid_x-delta){
            select.push_back(point[k]);
        }
    }
    double cross=1e12;
    unordered_map<grid,grid,GridHash> coo;
    for(int k=0;k<select.size();k++){
        for(int xp=-2;xp<=2;xp++){
            for(int yp=-2;yp<=2;yp++){
                for(int zp=-2;zp<=2;zp++){
                    long long xn=ceil(2*select[k].x/delta)+xp;
                    long long yn=ceil(2*select[k].y/delta)+yp;
                    long long zn=ceil(2*select[k].z/delta)+zp;
                    if(coo.count(grid(xn,yn,zn))==0){
                        continue;
                    }
                    grid fir_grid=select[k];
                    grid sec_grid=coo[grid(xn,yn,zn)];
                    double dist=distance(fir_grid,sec_grid);
                    if(dist<cross){
                        cross=dist;
                    }
                }
            }
        }
        coo[grid(ceil(2*select[k].x/delta),ceil(2*select[k].y/delta),ceil(2*select[k].z/delta))]=select[k];
    }
    return min(delta,cross);
}
int main(){
    int T;
    cin>>T;
    for(int t=0;t<T;t++){
        int n;
        cin>>n;
        vector<grid> point(n);
        for(int i=0;i<n;i++){
            cin>>point[i].x;
            cin>>point[i].y;
            cin>>point[i].z;
        }
        sort(point.begin(),point.end(),[](const grid& a, const grid& b) {
              return a.x < b.x;
        });
        cout<<fixed<<setprecision(2)<<find(point,0,n-1)<<endl;        
    }
}
```

```cpp
#include <bits/stdc++.h>
using namespace std;

// 关闭同步流
inline void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

struct Point {
    double x, y, z;
};

inline double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

const int HASH_SIZE = 5000011; // 大素数，减少冲突
int head[HASH_SIZE];           // 哈希桶的头指针
int nxt[300005];               // 链表中的下一个节点
int hash_gen[HASH_SIZE];       // 记录该桶是哪一代的（清空哈希表的神技）
int current_gen = 0;           // 当前的代数

inline int get_hash(long long cx, long long cy, long long cz) {
    unsigned long long h = (cx * 73856093ULL) ^ (cy * 19349663ULL) ^ (cz * 83492791ULL);
    return h % HASH_SIZE;
}

inline void insert_hash(int id, long long cx, long long cy, long long cz) {
    int h = get_hash(cx, cy, cz);
    if (hash_gen[h] != current_gen) {
        hash_gen[h] = current_gen;
        head[h] = -1;
    }
    nxt[id] = head[h];
    head[h] = id;
}

double solve(vector<Point>& pts) {
    int n = pts.size();
    if (n < 2) return 0;

    mt19937 rng(1337); 
    shuffle(pts.begin(), pts.end(), rng);

    double d = dist(pts[0], pts[1]);
    if (d == 0) return 0;

    current_gen++; 
    insert_hash(0, floor(pts[0].x / d), floor(pts[0].y / d), floor(pts[0].z / d));
    insert_hash(1, floor(pts[1].x / d), floor(pts[1].y / d), floor(pts[1].z / d));

    for (int i = 2; i < n; i++) {
        long long cx = floor(pts[i].x / d);
        long long cy = floor(pts[i].y / d);
        long long cz = floor(pts[i].z / d);
        
        bool updated = false;

        for (long long dx = -1; dx <= 1; dx++) {
            for (long long dy = -1; dy <= 1; dy++) {
                for (long long dz = -1; dz <= 1; dz++) {
                    int h = get_hash(cx + dx, cy + dy, cz + dz);
                    if (hash_gen[h] == current_gen) { 
                        for (int curr = head[h]; curr != -1; curr = nxt[curr]) {
                            double cur_dist = dist(pts[i], pts[curr]);
                            if (cur_dist < d) {
                                d = cur_dist;
                                updated = true;
                            }
                        }
                    }
                }
            }
        }
        if (updated) {
            if (d == 0) return 0;
            current_gen++; 
            for (int j = 0; j <= i; j++) {
                insert_hash(j, floor(pts[j].x / d), floor(pts[j].y / d), floor(pts[j].z / d));
            }
        } else {
            insert_hash(i, cx, cy, cz);
        }
    }
    return d;
}

int main() {
    fast_io();
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<Point> point(n);
        for (int i = 0; i < n; i++) {
            cin >> point[i].x >> point[i].y >> point[i].z;
        }
        cout << fixed << setprecision(2) << solve(point) << "\n";
    }
    return 0;
}
```

取巧：
```cpp
#include <bits/stdc++.h>
using namespace std;
inline void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}
double find(vector<tuple<long long,long long,long long>>& point,int i,int j){
    if(j==i){
        return 1e18;
    }
    int mid=(i+j)/2;
    long long mid_x=get<0>(point[mid]);
    double left=find(point,i,mid);
    double right=find(point,mid+1,j);
    vector<tuple<long long,long long,long long>> left_temp(mid-i+2);
    vector<tuple<long long,long long,long long>> right_temp(j-mid+1);
    left_temp[mid-i+1]=make_tuple(1e18,1e18,1e18);
    right_temp[j-mid]=make_tuple(1e18,1e18,1e18);
    int a=0;
    int b=0;
    for(int k=0;k<=mid-i;k++){
        left_temp[k]=point[i+k];
    }
    for(int k=0;k<=j-mid-1;k++){
        right_temp[k]=point[mid+1+k];
    }
    for(int k=i;k<=j;k++){
        if(get<1>(left_temp[a])<get<1>(right_temp[b])){
            point[k]=left_temp[a];
            a++;
        }else{
            point[k]=right_temp[b];
            b++;
        }
    }
    double sigma=min(left,right);
    vector<tuple<long long,long long,long long>> select;
    for(int k=i;k<=j;k++){
        if(get<0>(point[k])>=mid_x-sigma && get<0>(point[k])<=mid_x+sigma){
            select.push_back(point[k]);
        }
    }
    double cross=1e18;
    for(int k=0;k<select.size();k++){
        for(int t=1;t<=15 &&k + t < select.size();t++){
            double d1 = (double)get<0>(select[k]) - get<0>(select[k + t]);
            double d2 = (double)get<1>(select[k]) - get<1>(select[k + t]);
            double d3 = (double)get<2>(select[k]) - get<2>(select[k + t]);
            if (abs(d2) >= sigma) break;
            double dist = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
            if (dist < cross) {
                cross = dist;
            }
        }
    }
    return min(cross,sigma);
}

int main() {
    fast_io();
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<tuple<long long,long long,long long>> point(n);
        for (int i = 0; i < n; i++) {
            cin >> get<0>(point[i]) >> get<1>(point[i]) >> get<2>(point[i]);
        }
        sort(point.begin(),point.end());
        cout << fixed << setprecision(2) << find(point,0,n-1) << "\n";
    }
    return 0;
}
```

## practise2
### 题意
1. 有n个节点形成一棵树，每个节点有自己的权重$w_i$。除了根节点可以直接访问，其他节点需要父节点被访问之后才能访问。
2. 时间戳T一开始为1。访问一个节点需要花费1个单位时间，即访问完一个点后时间戳自增1。访问该节点的代价$c_i=T_i \times w_i$
3. 求访问完所有节点的最小总代价$\sum c_i$

### 解法(贪心算法)
1. 每次挑出当前权重最大的点，他肯定在父节点被访问后立即访问，否则无法使总代价最小。因此可以将其合并到父节点（因为两个要紧临着选）
2. 对于每个合成节点而言，他的等价权重为$\frac{\sum wi}{size}$。
proof: 对于两个合成节点node1、node2，假设他们的权重和分别为$W_1、W_2$，节点数量分别为$n_1,n_2$。
    1. 假设目前要在这两个节点中进行选择，时间戳为T，只选node1、只选node2的总代价分别为$C_1,C_2$。
    那么先选node1，再选node2的总代价为$C_1+C_2+n_1 \times W_2$,(即node2所有点要推迟$n_1$时间之后才选)
    2. 同理，先选node2再选node1的总代价为$C_1+C_2+n_2 \times W_1$
    3. 若先选node1总代价更小，则意味着
    $$C_1+C_2+n_1 \times W_2<C_1+C_2+n_2 \times W_1$$
    即 $$n_1 \times W_2<n_2 \times W_1$$
    即 $$\frac{W_1}{n_1}>\frac{W_2}{n_2}$$
3. 若本轮遇到根节点权重最大则跳过，直到只剩缩的只剩一个节点(即根节点)才选。

### 具体实现
1. 假设所有点同时一开始就选了，但由于每次只能选一个，我们后期只需要在初始$cost=\sum wi$ 的前提下加上延迟即可
2. 数据结构:
    - 维护一个tuple<平均权重，节点编号,被选时间戳>的大顶堆（这个时间戳只是用于懒删除，并不是真的计算用的）
    - 维护一个parent[u]:u的父节点
    - 维护一个info[u]:编号为u的节点的<平均权重，权重和，size，被选时间戳>
    - 维护一个dsu,记录parent[u]所属于的节点
3. 初始化:
    - 按图的信息初始化parent
    - dsu每个节点一开始都指向自己
    - 初始化info[u]=<权重，权重，1，1>
    - 每个非根节点一开始按make_tuple<权重，节点编号，1>加入大顶堆
3. 每次弹出堆顶节点u，检测对弹出的时间戳与info中是否对应，进行懒删除。cost+=u.权重和*dsu.find(parent[u]).size (从info中读取)。dsu的u指向dsu.find(parent[u])，这里不要按秩或按大小合并，否则会改变信息。然后dsu.find(parent[u]).size+=u.size,权重和和平均权重、时间戳都更新。
大顶堆中push更新之后的parent[u]，如果parent[u]是根节点则不用入堆。
4. 堆空了则结束。




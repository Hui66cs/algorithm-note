#include <iostream>
#include <vector>

using namespace std;

const long long INF = 1e18;

// 保存原图边的信息
struct OriginalEdge {
    int u, v;
    long long w;
};

// 算法计算用的边
struct Edge {
    int u, v;
    long long w;
    int id; // 记住这属于哪一条原图边
};

int m_total;
vector<int> final_edges; // 用于在递归返回时收集最终确定的所有原图边的 ID

// 递归版 DMST，极其优雅且安全地处理嵌套环边的替换
bool dmst(int n, int root, const vector<Edge>& edges) {
    vector<int> in_edge(n, -1);
    vector<long long> min_w(n, INF);
    vector<int> parent(n, -1);
    
    // 1. 寻找最小入边
    for (const auto& e : edges) {
        if (e.v != root && e.u != e.v && e.w < min_w[e.v]) {
            min_w[e.v] = e.w;
            in_edge[e.v] = e.id;
            parent[e.v] = e.u;
        }
    }

    // 检测联通性（结合你的虚拟根逻辑，实际上不可能不连通）
    for (int i = 0; i < n; i++) {
        if (i != root && in_edge[i] == -1) return false;
    }

    vector<int> loop_id(n, -1);
    vector<int> visited(n, -1);
    int cnt = 0;
    bool has_loop = false;

    vector<vector<int>> cycle_nodes;
    vector<vector<int>> cycle_edges;

    // 2. 找环
    for (int i = 0; i < n; i++) {
        int v = i;
        while (v != root && visited[v] == -1 && loop_id[v] == -1) {
            visited[v] = i;
            v = parent[v];
        }
        if (v != root && visited[v] == i && loop_id[v] == -1) {
            has_loop = true;
            int curr = v;
            vector<int> c_nodes;
            vector<int> c_edges;
            while (true) {
                loop_id[curr] = cnt;
                c_nodes.push_back(curr);
                c_edges.push_back(in_edge[curr]);
                curr = parent[curr];
                if (curr == v) break;
            }
            cycle_nodes.push_back(c_nodes);
            cycle_edges.push_back(c_edges);
            cnt++;
        }
    }

    // 3. 如果没环，直接把这些边加进最终答案，到达递归基
    if (!has_loop) {
        for (int i = 0; i < n; i++) {
            if (i != root) final_edges.push_back(in_edge[i]);
        }
        return true;
    }

    int num_cycles = cnt;
    for (int i = 0; i < n; i++) {
        if (loop_id[i] == -1) {
            loop_id[i] = cnt++;
        }
    }

    // 4. 缩点，更新边权，进入下一层递归
    vector<Edge> next_edges;
    vector<int> edge_target(m_total, -1); // 记录当前这一层，每条原图边究竟指向了哪个节点(包含超级节点)
    
    for (const auto& e : edges) {
        edge_target[e.id] = e.v;
        if (loop_id[e.u] != loop_id[e.v]) {
            Edge ne;
            ne.u = loop_id[e.u];
            ne.v = loop_id[e.v];
            ne.w = e.w - min_w[e.v]; // reduced cost
            ne.id = e.id;
            next_edges.push_back(ne);
        }
    }

    if (!dmst(cnt, loop_id[root], next_edges)) return false;

    // 5. 核心逻辑：从下一层递归回来后，展开环！
    // 找出最终选定边中，是哪一条破坏了环，从而抛弃对应的环内部边
    vector<int> broken_node(num_cycles, -1);
    for (int id : final_edges) {
        int target = edge_target[id];
        if (target != -1) {
            int lid = loop_id[target];
            if (lid >= 0 && lid < num_cycles) {
                broken_node[lid] = target;
            }
        }
    }

    for (int c = 0; c < num_cycles; c++) {
        for (int curr : cycle_nodes[c]) {
            if (curr != broken_node[c]) { // 把没被破坏的环边全数加回最终答案
                final_edges.push_back(in_edge[curr]);
            }
        }
    }

    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    m_total = m + n;
    vector<OriginalEdge> orig_edges(m_total);
    vector<Edge> edges(m_total);
    
    long long sum = 0;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        orig_edges[i] = {u, v, (long long)n * w};
        edges[i] = {u, v, (long long)n * w, i};
        sum += w;
    }
    
    int r = n; // 虚拟根
    for (int i = 0; i < n; i++) {
        long long vw = (long long)n * sum + n + i;
        orig_edges[m + i] = {r, i, vw};
        edges[m + i] = {r, i, vw, m + i};
    }
    
    if (!dmst(n + 1, r, edges)) {
        cout << "impossible\n";
        return 0;
    }
    
    // 我们不再需要去烦心 `cost` 的累加，最终选好的原始边都在 final_edges 里，直接统计即可！
    long long total_cost = 0;
    int virt_id = -1;
    for (int id : final_edges) {
        total_cost += orig_edges[id].w;
        if (id >= m) { // 找出被选中的那条虚拟边
            virt_id = id;
        }
    }
    
    // 你精妙的过滤两棵树的逻辑
    if (total_cost >= 2LL * (n * sum + n)) {
        cout << "impossible\n";
        return 0;
    }
    
    int root = virt_id - m;
    long long true_cost = (total_cost - orig_edges[virt_id].w) / n;
    
    vector<int> real_parent(n, -1);
    for (int id : final_edges) {
        if (id < m) { // 剔除掉虚拟边
            real_parent[orig_edges[id].v] = orig_edges[id].u;
        }
    }
    
    cout << true_cost << " " << root << "\n";
    for (int i = 0; i < n; i++) {
        if (i == root) {
            cout << root << "\n";
        } else {
            cout << i << " " << real_parent[i] << "\n";
        }
    }
    
    return 0;
}
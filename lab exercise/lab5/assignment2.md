# An Efficient $O(m \log n)$ Implementation of Edmonds' Algorithm for DMST

**Author:** 朱恒辉  
**Student ID:** 12412412  

---

## 1. Introduction and High-Level Intuition

Finding a Minimum Directed Spanning Tree (MDST), also known as an optimum branching, is a fundamental problem in graph theory. While the naive Edmonds/Chu-Liu algorithm elegantly solves this problem, its standard implementation requires $O(mn)$ time, which is inefficient for dense graphs. 

The lecture notes present a highly optimized implementation that reduces the time complexity to **$O(m \log n)$**. The core intuition behind this improvement lies in shifting from a "global" perspective to a "local" one. Instead of finding the cheapest incoming edges for all vertices simultaneously and dealing with multiple disjoint cycles, the efficient algorithm **grows a single path backwards**. By maintaining one continuous path and **eagerly contracting any cycle the moment it is formed**, the algorithm avoids complex global cycle management. This streamlined, sequential approach makes it perfectly suited to be accelerated by advanced data structures like Meldable Heaps and Disjoint-Set forests.

---

## 2. Algorithm Description

The algorithm operates in two main phases: the **Contraction Phase** (which builds a nested structure of cycles) and the **Expansion Phase** (which unrolls these cycles to form the final tree). 

To manage the complex nested structures, the algorithm maintains several properties for each vertex $u$ (which can be an original vertex or a newly formed "super-vertex"):
*   `in[u]`: The cheapest selected incoming edge to $u$.
*   `const[u]`: The offset cost applied to edges entering $u$, specifically $-w(in[u])$.
*   `prev[u]`: The preceding vertex in the currently growing path.
*   `parent[u]`: The ID of the super-vertex that $u$ is contracted into.
*   `children[u]`: A list of lower-level vertices contained within the super-vertex $u$.
*   `P[u]`: A min-heap storing all incoming edges to $u$.

*(Note: The `parent` and `children` properties form a nested tree structure that records the hierarchy of contractions, which will be recursively dismantled later.)*

### 2.1 The Contraction Phase
**Idea:** Grow a path backwards from an arbitrary starting node. When the path intersects itself, contract the formed cycle into a super-vertex.

1.  **Initialization:** Iterate through all edges and insert them into the min-heap `P[v]` of their respective destination vertex $v$. Initialize `in`, `prev`, `parent`, and `children` to `null`, and set `const` to $0$ for all vertices.
2.  **Path Growing and Evaluation:** Start at an arbitrary vertex $a$. While its heap `P[a]` is not empty, extract the minimum edge $(u, v)$. Use a Union-Find structure to find the current super-vertex $b$ that contains $u$.
    *   **Case 1 (Self-loop):** If $a == b$, the edge is internal to the current super-vertex. Discard it and extract the next edge.
    *   **Case 2 (Path Extension):** If $a \neq b$ and `in[b] == null` (meaning $b$ has no selected incoming edge yet), the edge extends the path without forming a cycle. We record the edge: `in[a] = (u, v)` and `prev[a] = b`. We then move our focus by setting $a \leftarrow b$ and continue.
    *   **Case 3 (Cycle Formation):** If $a \neq b$ but `in[b] != null`, we have hit a vertex already on our path, forming a cycle. We create a new super-vertex $c$. We trace back the cycle using the `prev` pointers. For each vertex along the cycle, we set `parent` to $c$, add it to `children[c]`, set its weight offset `const = -w(in)`, and **meld** its heap into `P[c]`. We then set $a \leftarrow c$ and resume.
3.  **Termination:** The forward phase concludes when the current vertex's heap `P[a]` becomes empty, indicating the entire reachable graph has been processed.

### 2.2 The Expansion Phase
**Idea:** Use an "unboxing" approach. Starting from the specified root $r$, we work top-down to undo the contractions, breaking cycles at precisely the correct edges to finalize the MDST.

1.  **Initialization:** Create an empty list $R$ to store super-vertices that become roots of disconnected components as we unbox.
2.  **Dismantling the Root:** Call `dismantle(r)`. This function traces from the actual root $r$ up to the top of the contraction tree. It clears the `parent` pointers along this path (breaking the cycles containing $r$). Any *other* children of these dismantled super-vertices are pushed into the list $R$.
3.  **Resolving Remaining Components:** While $R$ is not empty, pop a super-vertex $s$. The recorded edge `in[s] = (u, v)` represents the optimal edge entering this entire component. Since $v$ is the actual recipient of this edge inside the component, we set `in[v] = (u, v)`. However, $v$ might still be a super-vertex containing deeper cycles. Therefore, we call `dismantle(v)` to break the cycle precisely at $v$, which discards the old internal cycle edge entering $v$ and pushes newly exposed sub-components into $R$. Repeat until $R$ is empty.

---

## 3. Underlying Data Structures

To achieve the $O(m \log n)$ time bound, the generic operations described above must be implemented using specific advanced data structures.

### 3.1 Meldable Priority Queues: The Leftist Tree
During cycle contraction, we must combine the sets of incoming edges from multiple vertices into one single set for the new super-vertex. If we used standard Binary Heaps, merging two heaps containing a total of $m$ edges would require $O(m)$ time, which would completely ruin our overall time complexity. 

To solve this, we use a **Meldable Priority Queue**, such as a **Leftist Tree (Leftist Heap)**. 
*   **The Leftist Property:** A Leftist Tree is a binary tree that maintains the standard heap property (the parent is smaller than its children). However, it tracks an additional metric for each node called the *Null Path Length (NPL)*—the shortest distance to an empty node. The tree enforces a strict rule: for any node, the NPL of its left child is always greater than or equal to the NPL of its right child. 
*   **$O(\log m)$ Melding:** This property intentionally unbalances the tree, forcing it to be "heavy" on the left. As a result, the **right spine** (the path traversing only right children) is guaranteed to be exceptionally short: at most $O(\log m)$ nodes. To `meld` two Leftist Trees, we simply recursively merge their right spines. This operation completes strictly in **$O(\log m)$** time. `extract-min` is also $O(\log m)$, as it simply removes the root and melds its left and right subtrees.
*   **Lazy Evaluation:** Leftist Trees perfectly complement the `const[u]` weight offsets. Since all edges entering a contracted super-vertex share the exact same offset, their *relative* order inside the Leftist Tree remains unchanged. We can meld entire trees without manually updating individual edge weights, preserving the $O(\log m)$ efficiency.

### 3.2 Union-Find with Path Compression and Union By Size
When we extract an edge $(u, v)$, we need to quickly find which super-vertex $u$ currently belongs to. 
By using a Disjoint-Set data structure augmented with **Path Compression**, we solve this efficiently. Initially, use an arraylist and set each index(node index) is equal to itself. When we call `find(u)`, the algorithm climbs the tree to find the root super-vertex. Before returning, it updates the parent pointers of $u$ and *all* intermediate nodes visited along the way to point directly to the root. This drastically flattens the tree. Moreover, by using union by size, when combining two trees into one, we always let the smaller one to join the bigger one, which can lower the height of the total tree. Consequently, the amortized time complexity for each `find` operation drops to $O(\alpha(n))$, where $\alpha$ is the inverse Ackermann function—a value so small it is effectively **$O(1)$** in practice.

---

## 4. Time Complexity Analysis

The algorithm's total time complexity is strictly bounded by **$O(m \log n)$**. Here is the precise step-by-step mathematical breakdown (where $V=n, E=m$):

1.  **Initialization:** Creating arrays takes $O(n)$ time. Creating priority queues and inserting all $m$ edges takes $O(m \log m)$ time. Since $m =O(n^2)$, $\log m =O(2 \log n)$, making this $O(m \log n)$. So the total is $O(n+m \log n)=O(m \log n)$
2.  **Extract-min Operations:** An edge is extracted from a heap at most once. If it's a self-loop, it is discarded. If not, it is used. Therefore, there are at most $m$ `extract-min` operations across the entire algorithm. Each takes $O(\log m)$, contributing **$O(m \log m)=O(m \log n)$** in total.
3.  **Melding Heaps:** A contraction reduces the number of independent vertices in the graph by at least one. Thus, there can be at most $n-1$ contractions. Melding heaps during a contraction takes $O(\log m)$. This contributes **$O((n-1) \log m)=O(n \log n)$**.
4.  **Union-Find Queries:** We perform a `find` operation for the endpoints of every extracted edge. At most $O(m)$ queries are made. With path compression, this takes $O(m \cdot \alpha(n))$, which is tightly bounded by **$O(m)$**.
5.  **Expansion Phase:** The `expand` and `dismantle` functions traverse the contraction tree. Since the contraction tree has at most $2n - 1$ nodes, and each node's pointers are modified at most once, this entire phase takes**$O(n)$** time.

**Conclusion:** Summing them up yields $O(m \log n) + O(n \log n) + O(m) + O(n) = \mathbf{O(m \log n)}$.

---

## 5. Space Complexity Analysis

The overall space complexity of the algorithm is linear relative to the size of the graph: **$O(n + m)$**.

1.  **Graph Representation:** Storing the vertices and edges requires $O(n + m)$ space.
2.  **Contraction Forest Arrays:** The algorithm generates "super-vertices". Since $n$ original vertices can be merged a maximum of $n-1$ times, the total number of vertices (original + super) never exceeds $2n - 1$. Therefore, arrays like `in`, `const`, `prev`, `parent`, and `children` require **$O(n)$** space.
3.  **Priority Queues (Heaps):** Every edge from the original graph is inserted into the heaps exactly once and is never duplicated. Using pointer-based implementations for Meldable Heaps, the memory footprint scales strictly with the number of edges, requiring **$O(m)$** space.
4.  **Union-Find Structure:** Requires an array of size $2n$ for parent tracking, taking **$O(n)$** space.

Summing these independent components confirms the strict **$O(n + m)$** space complexity.

---

## 6. Mistakes

The pseudocode use ```in[u] = null``` to jump to extend path. However when u has formed a super vertex b, when in[b] is null and in[u] is not nill, this should also extend the path. So the judging condition should be revised as ```in[b] = null``` to avoid the wrong judging.
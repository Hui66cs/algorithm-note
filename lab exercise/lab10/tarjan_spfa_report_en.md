# SPFA with Tarjan's Subtree Disassembly Trick

## 0. Setting and notation

Let \(G=(V,E)\) be a directed graph with edge cost \(c(u,v)\), where \(|V|=n\) and \(|E|=m\). We want shortest paths to a fixed terminal node \(t\), and we also want to detect whether a negative cycle exists.

The push-based Bellman-Ford-Moore/SPFA implementation keeps a current distance label

\[
M[v] \approx \text{current best known distance from } v \text{ to } t.
\]

Initially, \(M[t]=0\) and \(M[v]=+\infty\) for \(v\ne t\). When the distance of a node \(w\) decreases, SPFA scans all incoming edges \((v,w)\). If

\[
M[v] > c(v,w)+M[w],
\]

then \(M[v]\) can be decreased, and \(v\) becomes active.

To support negative-cycle detection, we also maintain a pointer

\[
first[v] = \text{the first node after } v \text{ on the current path realizing } M[v].
\]

Thus, if \(first[v]=w\), the selected pointer edge is \((v,w)\). These selected edges form a pointer graph \(P\). When \(P\) is acyclic, it is a forest of directed paths toward \(t\). If adding a new pointer edge creates a directed cycle in \(P\), then that cycle must be a negative-cost cycle, because the pointer was created by a strict distance improvement.

Tarjan's trick improves the practical efficiency of SPFA by detecting such cycles immediately and by deleting stale subtrees whose distance labels were derived from an old value.

---

## 1. Pseudocode

The pseudocode below uses incoming adjacency lists `In[w]`, where `In[w]` stores all edges `(v, w)` entering `w`.

For the pointer graph \(P\), besides `first[v]`, we maintain reverse child lists:

```text
children[x] = all nodes y such that first[y] = x
```

This lets us find the whole subtree whose selected paths pass through a node \(v\).

```text
Algorithm Tarjan-SPFA(G, t)

Input:
    Directed graph G = (V, E), edge cost c(u, v), terminal node t

Output:
    Shortest path labels M[v] if no negative cycle is detected;
    otherwise report a negative cycle.

1.  for each node v in V:
2.      M[v]        <- +infinity
3.      first[v]    <- NIL
4.      children[v] <- empty list
5.      dormant[v]  <- false
6.      inQueue[v]  <- false

7.  M[t] <- 0
8.  Q <- empty queue
9.  ENQUEUE(t)

10. while Q is not empty:
11.     w <- DEQUEUE(Q)
12.     inQueue[w] <- false

13.     if dormant[w] = true:
14.         continue

15.     for each edge (v, w) in In[w]:
16.         if M[w] != +infinity and M[v] > c(v, w) + M[w]:

17.             newValue <- c(v, w) + M[w]

18.             result <- DISASSEMBLE-AND-CHECK(v, w)

19.             if result reports a cycle:
20.                 return "negative cycle found", result.cycle

21.             M[v] <- newValue

22.             first[v] <- w
23.             add v to children[w]
24.             dormant[v] <- false

25.             if inQueue[v] = false:
26.                 ENQUEUE(v)
27.                 inQueue[v] <- true

28. return M
```

The key subroutine is the following.

```text
Subroutine DISASSEMBLE-AND-CHECK(v, w)

Purpose:
    We are about to replace first[v] by w.
    Before doing so, test whether adding pointer edge (v, w)
    would create a directed cycle.
    If no cycle is created, mark the old subtree rooted at v as dormant.

1.  S <- all nodes in the subtree rooted at v in the current pointer graph P
        // found by DFS/BFS using children lists

2.  if w is in S:
3.      return the cycle formed by edge (v, w)
        plus the old pointer path from w back to v

4.  for each node x in S:
5.      if first[x] != NIL:
6.          remove x from children[first[x]]
7.      first[x] <- NIL
8.      clear children[x]
9.      dormant[x] <- true

10. return "no cycle"
```

In an actual implementation, the child lists should support \(O(1)\) removal of a node from its parent's child list. This can be done by storing each selected pointer edge as a linked-list item with a handle, or by storing `parent`, `firstChild`, `nextSibling`, and `prevSibling` arrays.

After `DISASSEMBLE-AND-CHECK(v, w)` returns "no cycle", the main algorithm installs the new pointer \(first[v]=w\), wakes up \(v\), and puts \(v\) into the queue.

---

## 2. Core idea and critical procedures

### 2.1 The pointer graph records the current explanation of distances

Whenever \(M[v]\) is improved through edge \((v,w)\), we set

\[
first[v]=w.
\]

This means the current value of \(M[v]\) is explained by taking edge \((v,w)\), then following the current pointer path from \(w\) to \(t\).

If the pointer graph ever contains a directed cycle, then following the inequalities that created those pointers gives a strict improvement around the cycle. Therefore, the total cost of the cycle must be negative. So detecting a cycle in the pointer graph is enough to detect a negative cycle in the original graph.

### 2.2 Naive instant cycle detection is too expensive

Suppose we update \(first[v]\) from its old value to \(w\). A natural way to test whether this creates a cycle is to follow the old pointer path from \(w\) toward \(t\). If this path reaches \(v\), then adding \((v,w)\) creates a cycle.

However, this path can have length \(\Theta(n)\). If we do such a path traversal for many relaxations, the algorithm may gain an additional factor of \(n\), which is too expensive in practice.

### 2.3 Tarjan's trick: check the subtree of \(v\), not the path from \(w\)

Before adding the new pointer edge \((v,w)\), the pointer graph is acyclic. Adding \((v,w)\) creates a cycle exactly when \(w\) is already in the subtree rooted at \(v\). Here the subtree rooted at \(v\) means all nodes whose current pointer path eventually reaches \(v\).

So the algorithm does this:

1. Use reverse child lists to find the subtree \(S\) rooted at \(v\).
2. If \(w\in S\), report a negative cycle immediately.
3. Otherwise, delete all old pointer edges inside \(S\) and mark nodes in \(S\) as dormant.
4. Install the new pointer \(first[v]=w\), wake \(v\), and continue SPFA.

### 2.4 Why marking nodes dormant is safe

Every node \(x\) in the subtree rooted at \(v\) had a current distance value that depended on the old value of \(M[v]\). Once \(M[v]\) decreases, these old explanations become stale. These nodes will eventually need to be updated again if their best paths still go through \(v\).

Instead of allowing all of them to remain active and push obsolete information, Tarjan's trick makes them dormant. A dormant node is not used to push relaxations. It becomes active again only when its own distance label is actually improved later.

This is the important insight: the work spent traversing and disassembling the subtree is not wasted. It also prevents future useless queue processing.

---

## 3. Complexity analysis

### Space complexity

The algorithm stores:

- the distance array \(M[\,]\);
- the pointer array `first[ ]`;
- queue flags `inQueue[ ]`;
- dormant flags `dormant[ ]`;
- the queue;
- child-list information for the pointer graph.

The pointer graph contains at most one outgoing pointer per node, so it has at most \(n\) selected pointer edges. Therefore the child-list structure uses \(O(n)\) extra space.

If the input graph itself is already stored, the additional working space is

\[
O(n).
\]

Including the graph storage, the total space is \(O(n+m)\).

### Time complexity

The worst-case running time remains

\[
O(mn).
\]

Ignoring the time spent disassembling subtrees, the algorithm is still a Bellman-Ford-Moore/SPFA-style algorithm. In the layered interpretation, after \(i\) effective rounds, the algorithm has correctly handled all shortest paths that can be represented by simple paths of at most \(i\) edges. If no negative cycle is found, a simple path has at most \(n-1\) edges, so there are at most \(n\) effective rounds. Each round may scan \(O(m)\) edges, giving \(O(mn)\).

Now consider the extra time spent by Tarjan's disassembly procedure. A node can be marked dormant only after it has previously received a selected pointer. Receiving such a pointer happens together with a distance update. Therefore, the total amount of subtree-disassembly work can be charged to previous distance updates. Since the number of such updates is bounded by the same \(O(mn)\) worst-case analysis, disassembly does not increase the asymptotic worst-case time.

Thus:

\[
\text{Worst-case time} = O(mn), \qquad
\text{extra space} = O(n).
\]

The main benefit is not asymptotic worst-case improvement; it is a large practical speedup.

---

## 4. Why Tarjan's trick can be much faster, even without negative cycles

Tarjan's trick helps even when the graph has no negative cycle because SPFA may still create many temporary distance labels that are later improved. Those temporary labels can cause large amounts of useless queue processing.

### Example 1: a long chain of stale descendants

Consider the following graph:

```text
x_k -> x_{k-1} -> ... -> x_2 -> x_1 -> v -> t
                                   \
                                    old path cost 100
v -> w -> t has total cost 1
```

Suppose SPFA first discovers the path \(v\to t\) of cost \(100\). Then it may propagate this value backward:

\[
M[x_1]=100,\quad M[x_2]=100,\quad \ldots,\quad M[x_k]=100.
\]

So the pointer graph contains a large subtree rooted at \(v\):

```text
x_k -> ... -> x_2 -> x_1 -> v -> t
```

Later, SPFA discovers the better path \(v\to w\to t\) of cost \(1\). Then \(M[v]\) decreases from \(100\) to \(1\).

At this moment, all nodes \(x_1,\ldots,x_k\) have stale labels, because their values were derived from the old value \(M[v]=100\). In ordinary SPFA, many of these nodes may still be in the queue. They may be popped, scan their incoming edges, and push more obsolete updates before being corrected later.

With Tarjan's trick, when \(first[v]\) changes, the algorithm finds the subtree rooted at \(v\), marks \(x_1,\ldots,x_k\) dormant, and removes their old pointers. These nodes will not perform useless relaxations. They will wake up only when the new smaller value actually reaches them.

This graph has no negative cycle; all edge costs in the example can be nonnegative. The speedup comes purely from avoiding stale work.

### Example 2: a bushy subtree with high in-degree nodes

Now suppose \(v\) has a large pointer subtree containing \(K\) nodes, and each of these nodes has many incoming edges. If \(M[v]\) decreases, the labels of all \(K\) descendants become obsolete.

Without Tarjan's trick, SPFA may process many of those \(K\) stale nodes. If each stale node scans many incoming edges, the wasted work can be very large.

With Tarjan's trick, the algorithm pays only \(O(K)\) time to disassemble the subtree and mark these nodes dormant. This can avoid scanning a much larger number of incident edges. Therefore, the practical speedup can be considerable.

### Example 3: avoiding repeated waves of obsolete propagation

In many graphs, distance labels improve in several waves. A node may first receive a mediocre path, then a better path, then an even better path. In ordinary SPFA, each wave may propagate far into the graph before being replaced by the next wave.

Tarjan's trick stops old waves early. Whenever a root node gets a better successor, its old dependent subtree is cut off. This prevents the algorithm from continuing to propagate information that is already known to be outdated.

Therefore, even when no negative cycle exists, Tarjan's trick can make SPFA much faster in practice by:

- detecting negative cycles immediately if they appear in the pointer graph;
- preventing dormant nodes from pushing stale relaxations;
- avoiding repeated scans of edges caused by obsolete distance labels;
- replacing large amounts of future useless work with one subtree traversal.

---

## 5. Conclusion

Tarjan's subtree disassembly trick improves SPFA by maintaining the pointer graph of current shortest-path explanations. When a pointer \(first[v]\) is changed, the algorithm checks the old subtree rooted at \(v\). If the new successor lies inside this subtree, a negative cycle is found immediately. Otherwise, the old subtree is disassembled and marked dormant, because its labels depend on the old value of \(M[v]\).

The algorithm still has worst-case time complexity \(O(mn)\) and only \(O(n)\) extra space, but in practice it can be much faster than ordinary SPFA. Its main advantage is that it avoids processing stale queue entries and obsolete pointer subtrees.

[TOC]

---
# Sorting
## CPP's Sorting is QuickSort
```cpp
#include<algorithm>
sort(arr.begin(),arr.end()); //increasing order
sort(arr.begin(), arr.begin() + 4) //increasing for the first 4 elements
#include <functional>
sort(arr.begin(), arr.end(), greater<int>())  //decreasing order
```
## SelectionSort
**idea**：from head to bottom，exchange the current element with the smallest elements behind it in every iteration，ensuring the first i element is the smallest i elements in the array and in order
```cpp
void SelectionSort(vector<int>& arr,int n){
    for(int i=0;i<n-1;i++){
        int key=i;
        for(int j=i+1;j<n;j++){
            if(arr[j]<arr[key]){
                key=j;
            }
        }
        swap(arr[i],arr[key]);
    }
}
```
==similar to BubbleSort==
**Time Complexity：$\Theta(n^2)$**

## InsertionSort
**idea**：It divides the array into 2 parts:sorted array and pending array. It compares the current element with the sorted elements in each iterations, then insert it into the suitable place(between the larger element and the smaller element) in the sorted array, forming a new sorted array.
```cpp
void InsertionSort(vector<int>& arr,int n){
    for(int i=1;i<n;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=0&&arr[j]>key){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
}
```
**Time Complexity: $\Omega(n),O(n^2)$**

## MergeSort
**idea**：Divide and conquer. Devide the array into 2 parts constantly until it has been to a single element, then gradually conquer two sorted arrays into a larger sorted array.
```cpp
#include <climits>
void Merge(vector<int>& arr,int p,int q,int r){
    left[q-p+1]=INT_MAX;
    right[r-q]=INT_MAX;
    for(int i=0;i<q-p+1;i++){
       left[i]=arr[i+p];
    }
    for(int j=0;j<r-q;j++){
        right[j]=arr[j+q+1];
    }
    left[q-p+1]=INT_MAX; //guard
    right[r-q]=INT_MAX;  //guard
    int i=0;
    int j=0;
    for(int k=p;k<=r;k++)
    if(left[i]<right[j]){
        arr[k]=left[i];
        i++;
    }else{
        arr[k]=right[j];
        j++;
    }
}
void MergeSort(vector<int>& arr,int p,int r){
    if(p>=r) return;
    int q=(p+r)/2;
    MergeSort(arr,p,q);
    MergeSort(arr,q+1,r);
    Merge(arr,p,q,r);
}
```
![](MergeSort.png)
**Time Complexity: $\Theta(nlogn)$**

## HeapSort
**idea**：Build a max-heap at the beginning. Every time exchange the element at the top of the heap with the one at the bottom, and heapsize--, maintaining the last max-heap property.
```cpp
//1-base heapsort
//turn the element and the subtrees under it into a max-heap. Notice that it must satisfy that two subtress has been max-heap already.
void MaxHeapify(vector<int>& arr,int x,int heapsize){
    int l=2*x;  //left child
    int r=2*x+1;  //right child
    int largest;  
    //to find the largest element between it and two childern
    if(l<=heapsize&&arr[x]<arr[l]){
        largest=l;
    }else{
        largest=x;
    }
    if(r<=heapsize&&arr[largest]<arr[r]){
        largest=r;
    }
    if(x!=largest){
        swap(arr[x],arr[largest]);
        MaxHeapify(arr,largest,heapsize);
    }
}

//build  the max-heap
void BuildMaxHeap(vector<int>& arr,int heapsize) {
    for(int i=heapsize/2;i>=1;i--){
        MaxHeapify(arr,i,heapsize);
    }
}
void HeapSort(vector<int>& arr,int heapsize){
    BuildMaxHeap(arr,heapsize);
    while(heapsize>1){
        swap(arr[1],arr[heapsize]);
        heapsize--;
        MaxHeapify(arr,1,heapsize);
    }
}
```
**Time Complexity: $\Theta(nlogn)$**

## QuickSort
**idea**：Divide and conquer. Select the last element in the array as a pivot. In the array, put the elements smaller than the pivot in the left, and put the larger elements in the right, put the pivot between these two arrays. Continually repeat the same operations to the left and right arrays.
```cpp
int Partition(vector<int>& arr,int p,int r){
    int pivot=arr[r];
    int i=p-1;
    for(int j=p;j<r;j++){
        if(arr[j]<pivot){
            i++;
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[i+1],arr[r]);
    return i+1;
}
void QuickSort(vector<int>& arr,int p,int r){
    if(p>=r) return;
    int q=Partition(arr,p,r);
    QuickSort(arr,p,q-1);
    QuickSort(arr,q+1,r);
}
```
**Time Complexity: $\Omega(nlogn),O(n^2)，Average: \Theta(nlogn)$**
![](QuickSort1.png)
![](QuickSort2.png)

### Randomised QuickSort
There might be some extremely unbalanced conditions, causing the time degenerate to $n^2$. We can randomly select the pivot, which is called Randomised QuickSort to avoid the worst case, making the time average time complexity still be $\Theta(nlogn)$, and no one can find the worst case.
**idea**：
```cpp
#include <random>
//random number generator
int randomInt(int a,int b){
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dis(a, b);
    return dis(gen);
}
int Partition(vector<int>& arr,int p,int r){
    int pivot=arr[r];
    int i=p-1;
    for(int j=p;j<r;j++){
        if(arr[j]<pivot){
            i++;
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[i+1],arr[r]);
    return i+1;
}
//randomly choose an index in the array, and exchange the element with the orginal pivot
int Randomised_Partition(vector<int>& arr,int p,int r){
    int i=randomInt(p,r);
    swap(arr[i],arr[r]);
    return Partition(arr,p,r);
}
void QuickSort(vector<int>& arr,int p,int r){
    if(p>=r) return;
    int q=Randomised_Partition(arr,p,r);
    QuickSort(arr,p,q-1);
    QuickSort(arr,q+1,r);
}
```

### 3-way QuickSort
==to avoid the problem of dealing with multiple identical elements inefficiently==
**idea**：divide the aray into 3 areas. (smaller area, equal area, larger area)
```cpp
pair<int, int> Partition(vector<int>& arr,int p,int r){
    int pivot=arr[r];
    int i=p-1; //arr[p] to arr[i]:smaller area
    int j=p;  //arr[i+1]to arr[j-1]:equal area
    //arr[j] to arr[m-1]:pending area，j as the pointer
    int m=r;  //arr[m] to arr[r]:larger area
    while(j<m){
        if(arr[j]<pivot){
            i++;
            swap(arr[i],arr[j]);
            j++;
        }else if(pivot==arr[j]){
            j++;
        }else{
            m--;
            swap(arr[j],arr[m]);
        }
    }
    swap(arr[r],arr[j]);
    return {i, j + 1};
}
void QuickSort(vector<int>& arr,int p,int r){
    if(p>=r) return;
    pair<int, int> bounds = Partition(arr, p, r);
    QuickSort(arr,p,bounds.first);
    QuickSort(arr,bounds.second,r);
}
```

## CountingSort
**idea**: initialise a new array to record the postiton for where the elements in the original array should be

```cpp
void CountingSort(vector<int>& arr){
    int max=-INT_MAX;
    int min=INT_MAX;
    int n=arr.size();
    for(int i=0;i<n;i++){
        if(arr[i]<min){
            min=arr[i];
        }
        if(arr[i]>max){
            max=arr[i];
        }
    }
    //to count the length of the count array
    int count_length=max-min+1; //IMPORTANT
    vector<int> count(count_length);
    for(int i=0;i<n;i++){
        count[arr[i]-min]++;
    }
    for(int i=1;i<count_length;i++){
        count[i]+=count[i-1];
    }
    vector<int> result(n);
    for(int i=n-1;i>=0;i--){
        result[count[arr[i]-min]-1]=arr[i];
        count[arr[i]-min]--;
    }
    for(int i=0;i<n;i++){
        arr[i]=result[i];
    }
}
``` 
==Stable Sort==
**Time Complexity**:$O(n+k),k=max-min+1$

## RadixSort
**idea**: Due to the CountingSort's stable quality, we can choose suitable base to divide the numbers into different digits, for every digits(from LSB to MSB) use CountingSort.
==Notice that it can only deal with non negative integers, so if you want to deal with negative one, you must first shift the all elements to nonegative, and recover it at last==
```cpp
//Base-256 RadixSort, which is used most frequently, since it is 0x..
void RadixSort(vector<int>& arr){
    int n=arr.size();
    int max=-INT_MAX;
    int min=INT_MAX;
    for(int i=0;i<n;i++){
        if(arr[i]<min){
            min=arr[i];
        }
        if(arr[i]>max){
            max=arr[i];
        }
    }
    for(int i=0;i<n;i++){
        arr[i]-=min;  //shift all numbers to positive(we will recover them later)
    }
    int ab_max=max-min;  
    vector<int> digit_rank(256);  //choose base-256
    vector<int> result(n);
    int round=0;
    //Important!!: num>>(8*x) means it use bit operation to shift right 8bit, which is equivalent to /10^8
    //num & 0xFF means it will only preserve the last 8 bit, achieving the goal of CountingSort of 0-255
    while(ab_max>>(round*8)>0){  //if the max number after shifting is still>0, then it needs one more CountingSort turn
        for(int j=0;j<n;j++){
            digit_rank[(arr[j]>>(8*round))&0xFF]++;
        }
        for(int j=1;j<256;j++){
            digit_rank[j]+=digit_rank[j-1];
        }
        for(int j=n-1;j>=0;j--){
            result[digit_rank[(arr[j]>>(8*round))&0xFF]-1]=arr[j];
            digit_rank[(arr[j]>>(8*round))&0xFF]--;
        }
        for(int j=0;j<n;j++){
            arr[j]=result[j];
        }
        for(int j=0;j<256;j++){
            digit_rank[j]=0;
        }
        round++;
    }
    for(int j=0;j<n;j++){
        arr[j]+=min;
    }
}
```
**Time Complexity**:$\Theta(d(n+k))=\Theta((n+k)\log_{k}M)$, M is the largest number in the array.
By base k, the largest number M has $log_{k}M$ digits. Since each digits $\in[0,k]$, so for each digit, the CountingSort takes $\Theta(n+k)$ times

---

# Elementary Data Structure

## Stack
**feature**: last in, first out
```cpp
template <typename T>
class Stack{
public:
    int size;
    T* arr;
    int top;
    Stack(int n){
        size=n;
        arr=new T[size];
        top=0;
    }
    void push(const T& x){
        if(top>=size){
            cout<<"overflow";
        }else{
            arr[top]=x;
            top++;
        }
    }
    void pop(){
        if(top-1<0){
            cout<<"underflow";
        }else{
            top--;
        }
    }
    ~Stack(){
        delete[] arr;
    }
};
```
==or directly use cpp's stack==
```cpp
#include <stack>
stack<T> s; //input the data type T for the stack
push(val);
pop();  //just remove
top();  //return the reference of the top element in the stack
empty();  //bool
size();
```

## Queue
**feature**: first in, first out
```cpp
template <typename T>
class Queue{
public:
    int size;
    T* arr;
    int front;
    int rear;
    int count;  //to distinguish overflow and underflow
    Queue(int n){
        size=n;
        arr=new T[size]; 
        front=0;
        rear=0;
        count=0;
    }
    void enqueue(const T& x){
        if(count==size){
            cout<<"overflow";
        }else{
            arr[rear]=x;
            count++;
            if(rear==size-1){
                rear=0;
            }else{
                rear++;
            }
        }
    }
    void dequeue(){
        if(count==0){
            cout<<"underflow";
        }else{
            count--;
            if(front==size-1){
                front=0;
            }else{
                front++;
            }
        }
    }
    ~Queue(){
        delete[] arr;
    }
};
```
==or directly use cpp's queue==
```cpp
#include <queue>
queue<T> q;  //input the data type T for the queue
push(val);  //enqueue the element to the rear
pop();  //dequeue the element at the front
front();  //return the reference at the front
back();  //return the reference at the rear
empty();  //bool
size();  
```
![](Queue.png)

## Priotity_Queue
**feature**: a max-heap, pup will always take out the greatest element in $O(logn)$
```cpp
#include <queue>
priority_queue<T> max_pq;  //Max-Heap
max_pq.push(x);  //insert an element while maintaining the max-heap property
max_pq.pop();  //delete the greatest element while maintaining the max-heap property
max_pq.top();  //return the reference of the greatest element 

priority_queue<int, vector<int>, greater<int>> min_pq;  //Min-Heap for int
min_pq.psuh(x);
min_pq.pop();
min_pq.top();
// the priority_queue for a struct needs to overload the logic for comparing
Struct student{
    int score;
    string name;
    bool operator<(const& student other) const{
        return score<other.score; //max-heap, if a<b is true, than b will be put ahead of a
        // return score>other.score  min-heap
    }
}
```

## Linked_list
```cpp
#include <list>  //doubly linked list
list<T> list1;
list1.push_back(x);  //insert at the back
list1.push_front(y);  //insert at the front
list1.pop_back();
list1.sort_front();
for(T x:list1){}  
list1.merge();  //merge 2 sorted lists
list1.insert_after(list1.begin()+k,z);  //insert z after the (k+1)th element
auto it=list1.begin();  //Iterator(smart pointer), it++ means it->next. list1.begin() and list1.end() is two side
//*it can get the value or the object
it=list1.erase(it);  //delete the node it, and return the next pointer to it 

#include <forward_list>  //singly linked list, more efficient
forward_list<T> list2;
list2.push_front(x);
list2.pop_front()
```

## Binary Search Tree
**feature**: the left child is smaller that the current node, the right child is larger than the current node
```cpp
struct TreeNode{
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    int val;
    int cnt;
    int size;
    TreeNode(int x){
        parent=nullptr;
        left=nullptr;
        right=nullptr;
        val=x;
        cnt=1;
        size=1;  //the size represent the number of nodes in the subtree whose root is this node
    }
};

TreeNode* root=nullptr;  //define the initial root

void update_size(TreeNode* node){
    if(node!=nullptr){
        node->size=node->cnt;
        if(node->left!=nullptr){
            node->size+=node->left->size;
        }
        if(node->right!=nullptr){
            node->size+=node->right->size;
        }
    }
}

void Insert(TreeNode*& root,TreeNode* p,int x){
    if(root==nullptr){
        root=new TreeNode(x);
        root->parent=p;
        return;
    }
    if(x<root->val){
        Insert(root->left,root,x);
    }else if(x==root->val){
        root->cnt++;
    }else{
        Insert(root->right,root,x);
    }
    update_size(root);
};

TreeNode* Successor(TreeNode* root,int x){
    TreeNode* answer=nullptr;  //record the local optimal solution
    while(root!=nullptr){
        if(root->val>x){  //if current node is greater than x, it will be the best solution up to now
            answer=root;
            root=root->left;  //try to find whether there is a better solution in the left subtree
        }else{
            root=root->right;
        }
    }
    return answer;
}

TreeNode* Predecessor(TreeNode* root,int x){
    TreeNode* answer=nullptr;  //record the local optimal solution
    while(root!=nullptr){
        if(root->val<x){  //if current node is smaller than x, it will be the best solution up to now
            answer=root;
            root=root->right;  //try to find whether there is a better solution in the left subtree
        }else{
            root=root->left;
        }
    }
    return answer;
}

TreeNode* Search(TreeNode* root,int x){
    while(root->val!=x){
        if(root->val>x){
            root=root->left;
        }else{
            root=root->right;
        }
    }
    return root;
}

void Transplant(TreeNode*& root,TreeNode* node1,TreeNode* node2){
    if(node1->parent==nullptr){
        root=node2;
    }else if(node1==node1->parent->left){
        node1->parent->left=node2;
    }else{
        node1->parent->right=node2;
    }
    if(node2!=nullptr){
        node2->parent=node1->parent;
    }
}
 
void Delete(TreeNode*& root,int x){
    TreeNode* node1=Search(root,x);
    if(node1->cnt>1){
        node1->cnt--;
        while(node1!=nullptr){
            update_size(node1);
            node1=node1->parent;
        }
    }else{
        if(node1->left==nullptr){
            TreeNode* node2=node1->right;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node1->parent;
            }
            while(node2!=nullptr){
            update_size(node2);
            node2=node2->parent;
            }
            delete node1;
        }else if(node1->right==nullptr){
            TreeNode* node2=node1->left;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node2->parent;
            }
            while(node2!=nullptr){
            update_size(node2);
            node2=node2->parent;
            }
            delete node1;
        }else{
            TreeNode* node2 = Successor(root,x);
            TreeNode* node3= node2;
            if(node2!=node1->right){
                node3= node2->parent;
                Transplant(root,node2,node2->right);
                node2->right=node1->right;
                node2->right->parent=node2;
            }
            Transplant(root,node1,node2);
            node2->left=node1->left;
            node2->left->parent=node2;
            delete node1;
            
            while(node3!=nullptr){
            update_size(node3);
            node3=node3->parent;
            }
        }
    }
}

int smaller(TreeNode* root,int x){
    if(root==nullptr){
        return 0;
    }
    if(x<root->val){
        return smaller(root->left,x);
    }else if(x==root->val){
        if(root->left!=nullptr){
            return root->left->size;
        }else{
            return 0;
        } 
    }else{
        if(root->left!=nullptr){
            return root->left->size+root->cnt+smaller(root->right,x);
        }else{
            return root->cnt+smaller(root->right,x);
        } 
    }
}

TreeNode* rank_at_x(TreeNode* root,int x){
    int temp=0;
    if(root->left==nullptr){
        temp=0;
    }else{
        temp=root->left->size;
    }
    if(x<=temp){
        return rank_at_x(root->left,x);
    }else if(temp<x&&x<=temp+root->cnt){
        return root;
    }else{
        return rank_at_x(root->right,x-(temp+root->cnt));
    }
}
```
### Tree Walk
```cpp
// left -- root -- right
void Inorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    Inorder(root->left,arr);
    arr.push_back(root->val);
    Inorder(root->right,arr);
}

// root -- left -- right
void Preorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    arr.push_back(root->val);
    Preorder(root->left,arr);
    Preorder(root->right,arr);
}

// left -- right -- root
void Postorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    Postorder(root->left,arr);
    Postorder(root->right,arr);
    arr.push_back(root->val);
}
```
1. Inorder: in sorted order
2. Preorder: copy tree, directory structure
3. Postorder: delete tree, calculate directory storage(Bottom-Up), Tree DP 

## AVL Tree(self-balancing tree)
**feature**: for every node v, $bal(v)<=1$ , $bal(v)=h(T_l)-h(T_r)$
```cpp
int get_height(TreeNode* node){
    if(node==nullptr){
        return 0;
    }else{
        return node->height;
    }
}

int get_size(TreeNode* node){
    if(node==nullptr){
        return 0;
    }else{
        return node->size;;
    }
}

void update(TreeNode* node){  //update the size and height
    if(node==nullptr){
        return;
    }else{
        node->height=max(get_height(node->left),get_height(node->right))+1;
        node->size=node->cnt+get_size(node->left)+get_size(node->right);
    }
}

int get_bal(TreeNode* node){
    if(node==nullptr){
        return 0;
    }
    return get_height(node->left)-get_height(node->right);
}

bool IsNotBalanced(TreeNode* node){
    return abs(get_bal(node)) > 1;
}

void Rotate(TreeNode*& root,TreeNode* node1){
    if(get_bal(node1)==2){
        if(get_bal(node1->left)>=0){  //LL
            TreeNode* node2=node1->left;
            if(node1==root){
                root=node2;
                node2->parent=nullptr;
            }else if(node1==node1->parent->left){
                node2->parent=node1->parent;
                node1->parent->left=node2;
            }else if(node1==node1->parent->right){
                node2->parent=node1->parent;
                node1->parent->right=node2;
            }
            node1->parent=node2;
            node1->left=node2->right;
            if(node2->right!=nullptr){
                node2->right->parent=node1;
            }
            node2->right=node1;
            update(node1);
            update(node2);
            }else{  //LR
            TreeNode* node2=node1->left;
            TreeNode* node3=node2->right;
            if(node1==root){
                root=node3;
                node3->parent=nullptr;
            }else if(node1==node1->parent->left){
                node3->parent=node1->parent;
                node1->parent->left=node3;
            }else if(node1==node1->parent->right){
                node3->parent=node1->parent;
                node1->parent->right=node3;
            }
            node2->right=node3->left;
            if(node3->left!=nullptr){
                node3->left->parent=node2;
            }
            node1->left=node3->right;
            if(node3->right!=nullptr){
                node3->right->parent=node1;
            }
            node2->parent=node3;
            node3->left=node2;
            node1->parent=node3;
            node3->right=node1;
            update(node1);
            update(node2);
            update(node3);
        }
    }else if(get_bal(node1)==-2){
        if(get_bal(node1->right)<=0){  //RR
            TreeNode* node2=node1->right;
            if(node1==root){
                root=node2;
                node2->parent=nullptr;
            }else if(node1==node1->parent->left){
                node2->parent=node1->parent;
                node1->parent->left=node2;
            }else if(node1==node1->parent->right){
                node2->parent=node1->parent;
                node1->parent->right=node2;
            }
            node1->parent=node2;
            node1->right=node2->left;

            if(node2->left!=nullptr){
                node2->left->parent=node1;
            }
            node2->left=node1;
            update(node1);
            update(node2);
        }else{  //RL
            TreeNode* node2=node1->right;
            TreeNode* node3=node2->left;
            if(node1==root){
                root=node3;
                node3->parent=nullptr;
            }else if(node1==node1->parent->left){
                node3->parent=node1->parent;
                node1->parent->left=node3;
            }else if(node1==node1->parent->right){
                node3->parent=node1->parent;
                node1->parent->right=node3;
            }
            node2->left=node3->right;
            if(node3->right!=nullptr){
                node3->right->parent=node2;
            }
            node1->right=node3->left;
            if(node3->left!=nullptr){
                node3->left->parent=node1;
            }
            node2->parent=node3;
            node3->right=node2;
            node1->parent=node3;
            node3->left=node1;
            update(node1);
            update(node2);
            update(node3);
        }
    }
}

void Insert(TreeNode*& node,TreeNode* p,int x){
    if(node==nullptr){
        node=new TreeNode(x);
        node->parent=p;
        return;
    }
    if(x<node->val){
        Insert(node->left,node,x);
    }else if(x==node->val){
        node->cnt++;
    }else{
        Insert(node->right,node,x);
    }
    update(node);
    if(IsNotBalanced(node)){
        Rotate(root,node);
    }
};

void Delete(TreeNode*& root,int x){
    TreeNode* node1=Search(root,x);
    if(node1->cnt>1){
        node1->cnt--;
        while(node1!=nullptr){
            update(node1);
            node1=node1->parent;
        }
    }else{
        if(node1->left==nullptr){
            TreeNode* node2=node1->right;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node1->parent;
            }
            while(node2!=nullptr){
            update(node2);
            TreeNode* temp=node2;
            node2=node2->parent;
            if(IsNotBalanced(temp)){
            Rotate(root,temp);
            }
            }
            delete node1;
        }else if(node1->right==nullptr){
            TreeNode* node2=node1->left;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node2->parent;
            }
            while(node2!=nullptr){
            update(node2);
            TreeNode* temp=node2;
            node2=node2->parent;
            if(IsNotBalanced(temp)){
            Rotate(root,temp);
            }
            }
            delete node1;
        }else{
            TreeNode* node2 = Successor(root,x);
            TreeNode* node3= node2;
            if(node2!=node1->right){
                node3= node2->parent;
                Transplant(root,node2,node2->right);
                node2->right=node1->right;
                node2->right->parent=node2;
            }
            Transplant(root,node1,node2);
            node2->left=node1->left;
            node2->left->parent=node2;
            delete node1;
            
            while(node3!=nullptr){
            update(node3);
            TreeNode* temp=node3;
            node3=node3->parent;
            if(IsNotBalanced(temp)){
            Rotate(root,temp);
            }
            }
        }
    }
}
```

---

# Graph Algorithms
## Expressions of graphs
### for directed graph
1. by adjacent list
**every node has an array, for every edge (u,v), you should add v to the array for u**
**Space Complexity**: $O(V+E)$, suitable for sparse graph($E=O(V)$)
```cpp
int V;
int E;
cin>>V>>E;
vector<vector<int>> adj(V+1);
for(int i=0;i<E;i++){
    int u;
    int v;
    cin>>u>>v;
    adj[u].push_back(v);
    //if it's undirected graph, also need
    ///adj[v].push_back(u)
}
``` 

2. by adjacent matrix
**prepare a matrix which size is $V \times V$, if there is a edge(u,v), the matrix(u,v)=1, else =0**
**Space Complexity**:$O(V^2)$, suitable for dense graph($E=\Theta(V^2)$)

```cpp
bool Exist_Edge(const vector<bool>& adj,int V, int u,int v){
    if(adj[(u-1)*V+v-1]==true){
        return true;
    }else{
        return false;
    }
}
int main(){
    int V;
    int E;
    cin>>V>>E;
    vector<bool> adj((V)*(V));
    for(int i=0;i<E;i++){
        int u;
        int v;
        cin>>u>>v;
        adj[(u-1)*V+v-1]=1;
        //if it's undirected graph, also need
        //adj[(v-1)*V+u-1]=1;
    }
}
```
### for undirected graph

1. adjacent list need to add both side of the edge, so space complexity becomes $O(V+2E)$
2. adjacent matrix becomes symmetric, the space complexity is still $O(V^2)$

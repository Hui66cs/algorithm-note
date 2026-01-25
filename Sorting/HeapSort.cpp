#include <iostream>
#include <vector>
using namespace std;
//1-base堆排序
//将该元素及其下面的部分(子树)转换成最大堆，注意：需要满足子树本身是最大堆的前提条件
void MaxHeapify(vector<int>& arr,int x,int heapsize){
    int l=2*x;
    int r=2*x+1;
    int largest;
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

//建立最大堆
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
int main(){
    int n;
    cin >> n;
    vector<int> arr(n+1);
    for(int i = 1; i < n+1; i++) {
        cin >> arr[i];
    }
    HeapSort(arr,n);
    for(int i = 1; i < n+1; i++) {
        cout << arr[i] << " ";
    }
}
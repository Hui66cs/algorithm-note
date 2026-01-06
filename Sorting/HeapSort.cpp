#include <iostream>
#include <vector>
using namespace std;
static int heapsize;
void MaxHeapify(vector<int>& arr,int x){
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
        MaxHeapify(arr,largest);
    }
}
void BuildMaxHeap(vector<int>& arr) {
    for(int i=heapsize/2;i>=1;i--){
        MaxHeapify(arr,i);
    }
}
void HeapSort(vector<int>& arr){
    BuildMaxHeap(arr);
    while(heapsize>1){
        swap(arr[1],arr[heapsize]);
        heapsize--;
        MaxHeapify(arr,1);
    }
}
int main(){
    int n;
    cin >> n;
    vector<int> arr(n+1);
    for(int i = 1; i < n+1; i++) {
        cin >> arr[i];
    }
    heapsize = n;
    HeapSort(arr);
    for(int i = 1; i < n+1; i++) {
        cout << arr[i] << " ";
    }
}
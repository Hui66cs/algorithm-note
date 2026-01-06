#include <iostream>
#include <vector>
using namespace std;
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
int main(){
    int n;
    cin >> n;
    vector<int> arr(n);
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    QuickSort(arr,0,n-1);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }
}
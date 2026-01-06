#include <iostream>
#include <vector>
using namespace std;
pair<int, int> Partition(vector<int>& arr,int p,int r){
    int pivot=arr[r];
    int i=p-1;
    int j=p;
    int m=r;
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
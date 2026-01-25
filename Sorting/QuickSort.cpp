#include <iostream>
#include <vector>
#include <random>
using namespace std;
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
#include <iostream>
#include <vector>
using namespace std;
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
int main(){
    int n;
    cin>>n;
    vector <int> arr(n);
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    SelectionSort(arr,n);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }
}
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
void Merge(int* arr,int p,int q,int r){
    int left[q-p+2];
    int right[r-q+1];
    for(int i=0;i<q-p+1;i++){
       left[i]=arr[i+p];
    }
    for(int j=0;j<r-q;j++){
        right[j]=arr[j+q+1];
    }
    left[q-p+1]=INT_MAX;
    right[r-q]=INT_MAX;
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
void MergeSort(int* arr,int p,int r){
    if(p>=r) return;
    int q=(p+r)/2;
    MergeSort(arr,p,q);
    MergeSort(arr,q+1,r);
    Merge(arr,p,q,r);
}
int main(){
    int n;
    cin>>n;
    int* arr=new int[n];
    for(int i=0;i<n;i++){
        cin>>arr[i];   
    }
    MergeSort(arr,0,n-1);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }
    delete[] arr;
}
#include <iostream>
using namespace std;
int BS(int* arr,int l,int r,int x){
    int mid=+(r-l)/2;
    if(r<l){
        return -1;
    }
    if(arr[mid]==x){
        return mid;
    }else if(arr[mid]>x){
        return BS(arr,l,mid-1,x);
    }else{
        return BS(arr,mid+1,r,x);
    }
}
int main(){
    int n;
    int x;
    cin>>n;
    cin>>x;
    int* arr=new int[n];
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    cout<<BS(arr,0,n-1,x);
    delete[] arr;
}
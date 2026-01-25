#include <iostream>
#include <vector>
#include <climits>
using namespace std;
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
int main(){
    int n;
    cin>>n;
    vector<int> arr(n);
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    RadixSort(arr);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }   
}
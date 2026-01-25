#include <iostream>
#include <vector>
#include <climits>
using namespace std;
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
    int count_length=max-min+1;
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
int main(){
    int n;
    cin>>n;
    vector<int> arr(n);
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    CountingSort(arr);
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }
}
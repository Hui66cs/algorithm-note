#include<iostream>
#include<vector>
#include<algorithm>
#include <queue>
using namespace std;
struct node{
    int value;
    node* next;
    node* pre;
    node(int n){
        value=n;
        pre=nullptr;
        next=nullptr;
    }
};
int main(){
    int n;
    int m;
    cin>>n;
    cin>>m;
    vector<node*> arr(n+1);
    for(int i=1;i<=n;i++){
        arr[i]=new node(i);
    }
    for(int i=2;i<n;i++){
        arr[i]->next=arr[i+1];
        arr[i]->pre=arr[i-1];
    }
    arr[n]->next=arr[1];
    arr[n]->pre=arr[n-1];
    arr[1]->next=arr[2];
    arr[1]->pre=arr[n];
    int record=1;
    while(n!=0){
        if(n==1){
           cout<<arr[record]->value;
           delete arr[record];
           break;
        }
        node* temp=arr[record];
        for(int i=1;i<m;i++){
            temp=temp->next;
        }
        temp->pre->next=temp->next;
        temp->next->pre=temp->pre;
        cout<<temp->value<<" ";
        record=temp->next->value;
        delete temp;
        n--;
    }
}
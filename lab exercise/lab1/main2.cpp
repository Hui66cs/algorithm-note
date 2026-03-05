#include <iostream>
using namespace std;

int main(){
    long long n;
    long long k;
    cin>>n>>k;
    if(n==1){
        cout<<1<<endl;
        return 0;
    }
    if(k>n/2){
        k=n+1-k;
    }
    cout<<2*k<<endl;
}

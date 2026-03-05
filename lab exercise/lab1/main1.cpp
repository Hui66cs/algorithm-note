#include <iostream>
using namespace std;

int main(){
    int t;
    cin>>t;
    for(int i=0;i<t;i++){
        long long n;
        cin>>n;
        if(n%6!=0){
            cout<<"Alice"<<endl;
        }else{
            cout<<"Bob"<<endl;
        }
    }
}

# include<bits/stdc++.h>
using namespace std;
const double PI = std::acos(-1.0);
struct Complex{
    double real;
    double vir;
    Complex(double real = 0, double vir = 0) {
        this->real = real;
        this->vir = vir;
    }
    Complex operator+(const Complex& other) const {
        return {real + other.real, vir + other.vir};
    }
    Complex operator-(const Complex& other) const {
        return {real - other.real, vir - other.vir};
    }
    Complex operator*(const Complex& other) const {
        return {real * other.real-vir*other.vir, vir * other.real+real * other.vir};
    }
    Complex operator/(int n) const {
        return {real/n, vir/n};
    }
};

void FFT(vector<Complex>& A,const vector<int>& rev,int L,bool idft=false){
    for(int i=0;i<L;i++){
        if(i<rev[i]){
            swap(A[i],A[rev[i]]);
        }
    }
    for(int i=2;i<=L;i<<=1){ //每组长度
        Complex wn;
        if(!idft){
            wn=Complex(cos(2 * PI / i), sin(2 * PI / i));
        }else{
            wn=Complex(cos(2 * PI / i), -sin(2 * PI / i));        
        }
        for(int j=0;j<L;j+=i){ //遍历各组
            Complex w(1,0);
            for(int k=0;k<i/2;k++){ //处理组内
                Complex t1=A[j+k];
                Complex t2=w*A[j+k+i/2];
                A[j+k]=t1+t2;
                A[j+k+i/2]=t1-t2;
                w=w*wn;
            }
        }
    }
    if(idft){
        for(int i=0;i<L;i++){
            A[i]=A[i]/L;
        }
    }
}

int main(){
    string s1;
    string s2;
    cin>>s1>>s2;
    int n=s1.size()-1;
    int m=s2.size()-1;
    int L = 1;
    int bit = 0;
    while (L <= n + m) {
        L <<= 1;
        bit++;
    }
    vector<Complex> A(L); 
    vector<Complex> B(L);
    for(int i=0;i<n+1;i++){
        int x=s1[n-i]-'0';
        A[i]=Complex(x);
    }
    for(int i=0;i<m+1;i++){
        int x=s2[m-i]-'0';
        B[i]=Complex(x);
    }
    vector<int> rev(L);
    rev[0]=0;
    for(int i=1;i<L;i++){
        rev[i]=rev[i>>1]>>1 | ((i&1)<<(bit-1));
    }
    FFT(A,rev,L);
    FFT(B,rev,L);
    vector<Complex> C(L);
    for(int i=0;i<L;i++){
        C[i]=A[i]*B[i];
    }
    FFT(C,rev,L,true);
    int t=L+1;
    vector<int> result(t);
    for(int i=0;i<t;i++){
        if(i<t-1){
            result[i]=(int)(C[i].real+0.5);
        }
        if(result[i]>=10 && i+1<t){
            C[i+1]=C[i+1]+result[i]/10;
            result[i]=result[i]%10;
        }
    }
    int head=0;
    for(int i=t-1;i>=0;i--){
        if(result[i]!=0){
            head=i;
            break;
        }
    }
    for(int i=head;i>=0;i--){
        cout<<result[i];
    }
    cout<<endl;
}
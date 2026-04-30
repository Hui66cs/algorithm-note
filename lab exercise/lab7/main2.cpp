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
int encode(char c){
    switch (c){
        case '?': return 0 ;
        case '0': return 1 ;
        case '1': return 2 ;
        default: return -1;
    }
}
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin>>s;
    int n=s.size()/2;
    vector<int> A(n);
    vector<int> B(n);
    for(int i=0;i<n;i++){
        A[i]=encode(s[i]);        
    }
    for(int i=0;i<n;i++){
        B[i]=encode(s[s.size() - 1 - i]);        
    }
    int L = 1;
    int bit = 0;
    while (L <= n*2 - 2) {
        L <<= 1;
        bit++;
    }
    vector<int> rev(L);
    rev[0]=0;
    for(int i=1;i<L;i++){
        rev[i]=rev[i>>1]>>1 | ((i&1)<<(bit-1));
    }
    vector<Complex> X(L,Complex(0));
    vector<Complex> Y(L,Complex(0));
    vector<Complex> C(L);
    vector<long long> Sum(L);
    //第一次
    for(int i=0;i<n;i++){
        X[i]=Complex(A[i]*A[i]*A[i]);
    }
    for(int i=0;i<n;i++){
        Y[i]=Complex(B[i]);
    }
    FFT(X,rev,L);
    FFT(Y,rev,L);
    for(int i=0;i<L;i++){
        C[i]=X[i]*Y[i];
    }
    FFT(C,rev,L,true);
    for(int i=0;i<L;i++){
        Sum[i]= llround(C[i].real);
    }
    //第二次
    fill(X.begin(), X.end(), Complex(0, 0));
    fill(Y.begin(), Y.end(), Complex(0, 0));
    for(int i=0;i<n;i++){
        X[i]=Complex(A[i]*A[i]);
    }
    for(int i=0;i<n;i++){
        Y[i]=Complex(B[i]*B[i]);
    }
    FFT(X,rev,L);
    FFT(Y,rev,L);
    for(int i=0;i<L;i++){
        C[i]=X[i]*Y[i];
    }
    FFT(C,rev,L,true);
    for(int i=0;i<L;i++){
        Sum[i]=Sum[i]-2LL* llround(C[i].real);
    }
    //第三次
    fill(X.begin(), X.end(), Complex(0, 0));
    fill(Y.begin(), Y.end(), Complex(0, 0));
    for(int i=0;i<n;i++){
        X[i]=Complex(A[i]);
    }
    for(int i=0;i<n;i++){
        Y[i]=Complex(B[i]*B[i]*B[i]);
    }
    FFT(X,rev,L);
    FFT(Y,rev,L);
    for(int i=0;i<L;i++){
        C[i]=X[i]*Y[i];
    }
    FFT(C,rev,L,true);
    for(int i=0;i<L;i++){
        Sum[i]+= llround(C[i].real);
    }
    vector<int> result(n+1,0); 
    for(int t=1;t<=n;t++){  //检查每一个位置
        if (Sum[t-1]==0) {
            result[t]=1;
        }
    }
    long long magic=0;
    for(long long i=1;i<=n;i++){
        magic ^= (result[i]*i*i);
    }
    cout<<magic<<endl;
}
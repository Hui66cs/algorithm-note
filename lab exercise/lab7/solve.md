## 洛谷P4173

### 题意

给定短字符串A和长字符串B，寻找B有多少个子字符串能适配A

### 解法
1. 将a-z编码为1-26，将通配符*编码为0。这样一个字符串就编码成一个数字串了
2. 注意到对于字符a、b，他们能匹配有2种情况
    1. a=b≠0
    2. a=0 or b=0
那么我们可以定义函数 $f(a,b)=ab(a-b)^2$ 来判断两者是否匹配：只有式子=0时才匹配(这里a-b加平方是因为后面字符串的拓展性，有可能会出现正负抵消的情况)
3. 根据上面的观察，可以拓展到字符串中。由于A长度为m，B长度为n (m<n) 。为了判断从B的第t个位置开始、长度为m的子串能否与A匹配，只需要对字串与A这长度m个字符都计算f并求和，只有和为0才匹配。即
$$F(t)=\sum_{i=0}^{m-1} f(A_i,B_{t+i})=\sum_{i=0}^{m-1} A_iB_{t+i}(A_i-B_{t+i})^2$$
4. 将其展开我们可以得到
$$F(t)=\sum_{i=0}^{m-1} A_i^3B_{t+i}-2\sum_{i=0}^{m-1} A_i^2B_{t+i}^2+\sum_{i=0}^{m-1} A_iB_{t+i}^3$$
注意到这几个求和与卷积有点类似，考虑进行变换。
设
$$P_1[i]=A_{m-1-i}^3,\ Q_1[i]=B_i$$
$$P_2[i]=A_{m-1-i}^2,\ Q_2[i]=B_i^2$$
$$P_3[i]=A_{m-1-i},\ Q_3[i]=B_i^3$$

令
$C_1=P_1*Q_1,\ C_2=P_2*Q_2,\ C_3=P_3*Q_3$ ，此处*为卷积符号

则
$$C_1[t+m-1]=\sum_{i=0}^{m-1}A_i^3B_{t+i}$$
$$C_2[t+m-1]=\sum_{i=0}^{m-1}A_i^2B_{t+i}^2$$
$$C_3[t+m-1]=\sum_{i=0}^{m-1}A_iB_{t+i}^3$$

因此
$$F(t)=C_1[t+m-1]-2C_2[t+m-1]+C_3[t+m-1]$$

5. 因此只需要用$O(nlogn)$三次FFT计算出C1、C2、C3，再用$O(n)$算出每一个F(t)即可

```cpp
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
        case '*': return 0 ;
        case 'a': return 1 ;
        case 'b': return 2 ;
        case 'c': return 3 ;
        case 'd': return 4 ;
        case 'e': return 5 ;
        case 'f': return 6 ;
        case 'g': return 7 ;
        case 'h': return 8 ;
        case 'i': return 9 ;
        case 'j': return 10 ;
        case 'k': return 11 ;
        case 'l': return 12 ;
        case 'm': return 13 ;
        case 'n': return 14 ;
        case 'o': return 15 ;
        case 'p': return 16 ;
        case 'q': return 17 ;
        case 'r': return 18 ;
        case 's': return 19 ;
        case 't': return 20 ;
        case 'u': return 21 ;
        case 'v': return 22 ;
        case 'w': return 23 ;
        case 'x': return 24 ;
        case 'y': return 25 ;
        case 'z': return 26 ;
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
    int m;
    int n;
    cin>>m>>n;
    string s1;
    string s2;
    cin>>s1>>s2;
    vector<int> A(m);
    vector<int> B(n);
    for(int i=0;i<m;i++){
        A[i]=encode(s1[i]);        
    }
    for(int i=0;i<n;i++){
        B[i]=encode(s2[i]);        
    }
    int L = 1;
    int bit = 0;
    while (L <= n + m - 2) {
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
    for(int i=0;i<m;i++){
        X[i]=Complex(A[m-1-i]*A[m-1-i]*A[m-1-i]);
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
    for(int i=0;i<m;i++){
        X[i]=Complex(A[m-1-i]*A[m-1-i]);
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
    for(int i=0;i<m;i++){
        X[i]=Complex(A[m-1-i]);
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
    int count=0;
    vector<int> result; 
    for(int t=0;t<=n-m;t++){  //检查每一个位置
        if (Sum[t+m-1]==0) {
            result.push_back(t);
            count++;
        }
    }
    cout<<count<<endl;
    for(int i=0;i<result.size();i++){
        cout<<result[i]+1<<" ";
    }
    cout<<endl;
}
```

## exercise2

### 题意
字符串长度为L，n=L/2向下取整
比较s[0~i]与s[(2n-1-i)~(2n-1)]是否匹配，其中i < n
预处理之后比较s1[i]=s[i]、s2[i]=[L-1-i] (倒序，为了构造卷积)
1. 编码： ？-> 0 , 0->1 , 1->2 
2. $f(a,b)=ab(a-b)^2$
3. 判断长度为t能否匹配，则计算
$$F(t)=\sum_{i=0}^{t-1} f(A_i,B_{t-1-i})=\sum_{i=0}^{t-1} A_iB_{t-1-i}(A_i-B_{t-1-i})^2=$$
4. 展开得到
$$F(t)=\sum_{i=0}^{t-1} A_i^3B_{t-1-i}-2\sum_{i=0}^{t-1} A_i^2B_{t-1-i}^2+\sum_{i=0}^{t-1} A_iB_{t-1-i}^3$$
5. 卷积变换
$$P_1[i]=A_{i}^3,\ Q_1[i]=B_i$$
$$P_2[i]=A_{i}^2,\ Q_2[i]=B_i^2$$
$$P_3[i]=A_{i},\ Q_3[i]=B_i^3$$

令
$C_1=P_1*Q_1,\ C_2=P_2*Q_2,\ C_3=P_3*Q_3$ ，此处*为卷积符号

则
$$C_1[t-1]=\sum_{i=0}^{t-1}P1[i]Q1[t-1-i]=\sum_{i=0}^{t-1}A_i^3B_{t-1-i}$$
$$C_2[t-1]=\sum_{i=0}^{t-1}A_i^2B_{t-1-i}^2$$
$$C_3[t-1]=\sum_{i=0}^{t-1}A_iB_{t-1-i}^3$$

因此
$$F(t)=C_1[t-1]-2C_2[t-1]+C_3[t-1]$$



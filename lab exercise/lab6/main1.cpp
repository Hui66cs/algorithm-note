#include <bits/stdc++.h>
using namespace std;
inline void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}
double find(vector<tuple<long long,long long,long long>>& point,int i,int j){
    if(j==i){
        return 1e18;
    }
    int mid=(i+j)/2;
    long long mid_x=get<0>(point[mid]);
    double left=find(point,i,mid);
    double right=find(point,mid+1,j);
    vector<tuple<long long,long long,long long>> left_temp(mid-i+2);
    vector<tuple<long long,long long,long long>> right_temp(j-mid+1);
    left_temp[mid-i+1]=make_tuple(1e18,1e18,1e18);
    right_temp[j-mid]=make_tuple(1e18,1e18,1e18);
    int a=0;
    int b=0;
    for(int k=0;k<=mid-i;k++){
        left_temp[k]=point[i+k];
    }
    for(int k=0;k<=j-mid-1;k++){
        right_temp[k]=point[mid+1+k];
    }
    for(int k=i;k<=j;k++){
        if(get<1>(left_temp[a])<get<1>(right_temp[b])){
            point[k]=left_temp[a];
            a++;
        }else{
            point[k]=right_temp[b];
            b++;
        }
    }
    double sigma=min(left,right);
    vector<tuple<long long,long long,long long>> select;
    for(int k=i;k<=j;k++){
        if(get<0>(point[k])>=mid_x-sigma && get<0>(point[k])<=mid_x+sigma){
            select.push_back(point[k]);
        }
    }
    double cross=1e18;
    for(int k=0;k<select.size();k++){
        for(int t=1;t<=1 &&k + t < select.size();t++){
            double d1 = (double)get<0>(select[k]) - get<0>(select[k + t]);
            double d2 = (double)get<1>(select[k]) - get<1>(select[k + t]);
            double d3 = (double)get<2>(select[k]) - get<2>(select[k + t]);
            if (abs(d2) >= sigma) break;
            double dist = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
            if (dist < cross) {
                cross = dist;
            }
        }
    }
    return min(cross,sigma);
}

int main() {
    fast_io();
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<tuple<long long,long long,long long>> point(n);
        for (int i = 0; i < n; i++) {
            cin >> get<0>(point[i]) >> get<1>(point[i]) >> get<2>(point[i]);
        }
        sort(point.begin(),point.end());
        cout << fixed << setprecision(2) << find(point,0,n-1) << "\n";
    }
    return 0;
}
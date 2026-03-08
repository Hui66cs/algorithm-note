#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m; //n is student number, m is college number

    cin >> n >> m;

    vector<int> cap(m+1); // capcity of each college 

    for(int i=1;i<m+1;i++){
        cin>>cap[i];
    }

    // student's preference list. j is rank, first is evaluation score, second is college number
    vector<vector<pair<int,int>>> stu_prefer(n+1,vector<pair<int,int>>(m+1)); 

    // college's preference list, j is student number, arr[j] evaluation score
    vector<vector<int>> col_prefer(m+1,vector<int>(n+1)); 


    for(int i=1;i<n+1;i++){
        for(int j=1;j<m+1;j++){
            cin>>stu_prefer[i][j].first;
            stu_prefer[i][j].second=j;
        }
        sort(stu_prefer[i].begin()+1,stu_prefer[i].end(),[](const pair<int,int>& a,const pair<int,int>& b){
            return a.first>b.first;
        });
    }

    for(int i=1;i<m+1;i++){
        for(int j=1;j<n+1;j++){
            cin>>col_prefer[i][j];
        }
    }

    vector<int> stu_choice(n+1,0);  // the matched college id

    vector<int> stu_apply(n+1,1);  // the index of preference rank

    vector<vector<int>> col_choice(m+1);

    vector<int> col_choi_size(m+1,0);

    queue<int> free_stu;

    for(int i=1;i<n+1;i++){
        free_stu.push(i);
    }

    while(!free_stu.empty()){
        int stu_id=free_stu.front();
        free_stu.pop();
        int col_rnk=stu_apply[stu_id];
        stu_apply[stu_id]++;
        if(col_rnk>m){
            continue;
        }
        int col_id=stu_prefer[stu_id][col_rnk].second;
        if(stu_prefer[stu_id][col_rnk].first>0 && col_prefer[col_id][stu_id]>0){
            if(col_choi_size[col_id]<cap[col_id]){
                stu_choice[stu_id]=col_id;
                col_choice[col_id].push_back(stu_id);
                col_choi_size[col_id]++;
            }else{
                int min_stu_id;  // the student id
                int min_index;  // the index in col_choice,col_choice[col_id][min_index]=min_stu_id
                for(int i=0;i<col_choi_size[col_id];i++){
                    if(i==0){
                        min_stu_id=col_choice[col_id][i];
                        min_index=0;
                    }else{
                        if(col_prefer[col_id][col_choice[col_id][i]]<col_prefer[col_id][min_stu_id]){
                            min_stu_id=col_choice[col_id][i];
                            min_index=i;
                        }
                    }
                }
                if(col_prefer[col_id][min_stu_id]<col_prefer[col_id][stu_id]){
                    stu_choice[stu_id]=col_id;
                    stu_choice[min_stu_id]=0;
                    free_stu.push(min_stu_id);
                    col_choice[col_id][min_index]=stu_id; 
                }else{
                    free_stu.push(stu_id);
                }
            }
        }else{
            free_stu.push(stu_id);
        }
    }

    for(int i=1;i<m+1;i++){
        cout<<col_choice[i].size()<<" ";
        sort(col_choice[i].begin(),col_choice[i].end());
        for(int j=0;j<col_choice[i].size();j++){
            cout<<col_choice[i][j]<<" ";
        }
        cout<<endl;
    }
}

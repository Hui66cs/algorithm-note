#include<bits/stdc++.h>
using namespace std;
typedef unsigned int uint;
vector<vector<char>> board(6,vector<char>(6));
uint goal=0b00'01101'00000'10000'11000'11110'11111;
vector<uint> moving={3,7,9,11};
uint encode(const vector<vector<char>>& board){
    uint state=0b0;
    uint blank=0;
    for(int i=1;i<6;i++){
        for(int j=1;j<6;j++){
            if(board[i][j]=='1'){ 
                state|=1<<(5*(i-1)+j-1);
            }else if(board[i][j]=='*'){
                blank=5*(i-1)+j;
            }
        }
    }
    state|=blank<<25;
    return state;
}
bool check_valid(uint a,uint b,bool op){
    //op为true是加法，false是减法
    uint x=(a-1)/5+1;
    uint y=(a-1)%5+1;
    uint mx=0;
    uint my=0;
    if(op){
        if(b==3){
            if(y<=2){
                return false;
            }
            mx=x+1;
            my=y-2;
        }else if(b==7){
            mx=x+1;
            my=y+2;
        }else if(b==9){
            if(y<=1){
                return false;
            }
            mx=x+2;
            my=y-1;
        }else{
            mx=x+2;
            my=y+1;
        }
        if(1<=mx&&mx<=5&&1<=my&&my<=5){
            return true;
        }else{
            return false;
        } 
    }else{
        if(b>a){
            return false;
        }
        if(b==3){
            if(x<=1){
                return false;
            }
            mx=x-1;
            my=y+2;
        }else if(b==7){
            if(x<=1||y<=2){
                return false;
            }
            mx=x-1;
            my=y-2;
        }else if(b==9){
            if(x<=2){
                return false;
            }
            mx=x-2;
            my=y+1;
        }else{
            if(x<=2||y<=1){
                return false;
            }
            mx=x-2;
            my=y-1;
        }
        if(1<=mx&&mx<=5&&1<=my&&my<=5){
            return true;
        }else{
            return false;
        } 
    }
    
}
bool double_bfs(uint start){
    if(start==goal){
        cout<<0<<endl;
        return true;
    }
    unordered_set<uint> discovered1;
    unordered_set<uint> back_bfs;
    queue<pair<uint,int>> q; // 反向bfs状态，已走步数
    queue<pair<uint,int>> p; // 正向bfs状态，已走步数
    q.push(make_pair(goal,0));
    discovered1.insert(goal);
    while(!q.empty()){
        uint cur_state=q.front().first;
        int cur_step=q.front().second;
        discovered1.insert(cur_state);
        q.pop();
        for(int i=0;i<4;i++){
            uint cur_blank=cur_state>>25;
            uint cur_horse=cur_state&((1<<25)-1);
            if(check_valid(cur_blank,moving[i],true)){
                uint nblank=cur_blank+moving[i];
                uint nhorse=0;
                if((cur_horse>>(nblank-1))&1==1){ //换位的是黑马
                    nhorse=(cur_horse|(1<<(cur_blank-1)))&~(1<<(nblank-1));//原本空格的0变成1，新空格置为0
                }else{  //换位的是红马
                    nhorse=cur_horse&~(1<<(nblank-1)); //原本的空格保持0，新空格置为1
                }
                uint nstate=nblank<<25|nhorse;
                if(discovered1.count(nstate)==0){
                    if(nstate==start){
                        cout<<cur_step+1<<endl;
                        return true;
                    }
                    if(cur_step+1==7){
                        back_bfs.insert(nstate);
                    }else{
                        q.push(make_pair(nstate,cur_step+1));
                        discovered1.insert(nstate);
                    }
                }
            }
        }
        for(int i=0;i<4;i++){
            uint cur_blank=cur_state>>25;
            uint cur_horse=cur_state&((1<<25)-1);
            if(check_valid(cur_blank,moving[i],false)){
                uint nblank=cur_blank-moving[i];
                uint nhorse=0;
                if((cur_horse>>(nblank-1))&1==1){ //换位的是黑马
                    nhorse=(cur_horse|(1<<(cur_blank-1)))&~(1<<(nblank-1));//原本空格的0变成1，新空格置为0
                }else{  //换位的是红马
                    nhorse=cur_horse&~(1<<(nblank-1)); //原本的空格保持0，新空格置为1
                }
                uint nstate=nblank<<25|nhorse;
                if(discovered1.count(nstate)==0){
                    if(nstate==start){
                        cout<<cur_step+1<<endl;
                        return true;
                    }
                    if(cur_step+1==7){
                        back_bfs.insert(nstate);
                    }else{
                        q.push(make_pair(nstate,cur_step+1));
                        discovered1.insert(nstate);
                    }
                }
            }
        }
    }

    p.push(make_pair(start,7));
    unordered_set<uint> discovered2;
    discovered2.insert(start);
    while(!p.empty()){
        uint cur_state=p.front().first;
        int cur_step=p.front().second;
        p.pop();
        for(int i=0;i<4;i++){
            uint cur_blank=cur_state>>25;
            uint cur_horse=cur_state&((1<<25)-1);
            if(check_valid(cur_blank,moving[i],true)){
                uint nblank=cur_blank+moving[i];
                uint nhorse=0;
                if((cur_horse>>(nblank-1))&1==1){ //换位的是黑马
                    nhorse=(cur_horse|(1<<(cur_blank-1)))&~(1<<(nblank-1));//原本空格的0变成1，新空格置为0
                }else{  //换位的是红马
                    nhorse=cur_horse&~(1<<(nblank-1)); //原本的空格保持0，新空格置为1
                }
                uint nstate=nblank<<25|nhorse;
                if(discovered2.count(nstate)==0){
                    if(cur_step+1<=15){
                        if(back_bfs.count(nstate)==1){
                            cout<<cur_step+1<<endl;
                            return true;
                        }
                        p.push(make_pair(nstate,cur_step+1));
                        discovered2.insert(nstate);
                    }
                }
            }
        }
        for(int i=0;i<4;i++){
            uint cur_blank=cur_state>>25;
            uint cur_horse=cur_state&((1<<25)-1);
            if(check_valid(cur_blank,moving[i],false)){
                uint nblank=cur_blank-moving[i];
                uint nhorse=0;
                if((cur_horse>>(nblank-1))&1==1){ //换位的是黑马
                    nhorse=(cur_horse|(1<<(cur_blank-1)))&~(1<<(nblank-1));//原本空格的0变成1，新空格置为0
                }else{  //换位的是红马
                    nhorse=cur_horse&~(1<<(nblank-1)); //原本的空格保持0，新空格置为1
                }
                uint nstate=nblank<<25|nhorse;
                if(discovered2.count(nstate)==0){
                    if(cur_step+1<=15){
                        if(back_bfs.count(nstate)==1){
                            cout<<cur_step+1<<endl;
                            return true;
                        }
                        p.push(make_pair(nstate,cur_step+1));
                        discovered2.insert(nstate);
                    }
                }
            }
        }
    }
    return false;
}
int main(){
    int T;
    cin>>T;
    for(int i=0;i<T;i++){
        int x=0;
        int y=0;
        int current_step=0;
        bool find=false;
        for(int j=1;j<6;j++){
            for(int k=1;k<6;k++){
                cin>>board[j][k];
                if(board[j][k]=='*'){
                    x=j;
                    y=k;
                }
            }
        }
        uint start=encode(board);
        if(!double_bfs(start)){
            cout<<-1<<endl;
        }
    }

}
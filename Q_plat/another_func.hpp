//
//  another_func.hpp
//  Q_plat
//
//  Created by Taguwa on 2022/11/14.
//

#ifndef another_func_h
#define another_func_h

#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <queue>
#include <math.h>
#include <random>
#include <regex>


#define EPISODE_MAX 10 // 何回ゴールするか
#define STEP_MAX 500 // ゴールまでの打ち切りステップ数
#define ALPHA 0.3 // 学習率 0~1
#define GAMMA 0.7 // 割引率
#define EPSILON 0.7


using namespace std;

//座標
struct row{
    int x;
    int y;
};

//q_table
struct q{
    double u;
    double d;
    double l;
    double r;
};
//
//表示
//
void print(vector<vector<int>> a){
    for(int i = 0 ; i < a.size() ; i++){
        for(int j = 0 ; j < a.at(0).size() ; j++){
            if(a[i][j] == 9){
                cout << "■ ";
            }else if(a[i][j] == 2){
                cout << "S ";
            }
            else if(a[i][j] == 3){
                cout << "G ";
            }
            else if(a[i][j] == 1){
                cout << "* ";
            }
            else{
                cout << "  ";
            }
        }
        cout << endl;
    }
}
//幅優先

queue<row> que;
int goal = 0;
vector<vector<int>> BFS_nextsteps(vector<vector<int>> a, row* pos){
    if(a[pos->y+1][pos->x] == 3){ que.push({pos->x,pos->y+1}); goal = 1; }
    if(a[pos->y-1][pos->x] == 3){ que.push({pos->x,pos->y-1}); goal = 1; }
    if(a[pos->y][pos->x+1] == 3){ que.push({pos->x+1,pos->y}); goal = 1; }
    if(a[pos->y][pos->x-1] == 3){ que.push({pos->x-1,pos->y}); goal = 1; }
    
    if(a[pos->y+1][pos->x] == 0) que.push({pos->x,pos->y+1});
    if(a[pos->y-1][pos->x] == 0) que.push({pos->x,pos->y-1});
    if(a[pos->y][pos->x+1] == 0) que.push({pos->x+1,pos->y});
    if(a[pos->y][pos->x-1] == 0) que.push({pos->x-1,pos->y});
    
    //queue update
    row front = que.front();
    pos->x = front.x;
    pos->y = front.y;
    que.pop();
    
    //already searched
    a[pos->y][pos->x] = 1;
    
    return a;
}

vector<vector<int>> BFS(vector<vector<int>> a){
    
    row pos = { 1,1 };
    int i = 0;
    while(i <= STEP_MAX){
        a = BFS_nextsteps(a, &pos);
        i++;
        if(goal == 1)break;
    }
    cout << i << "steps" << endl;
    return a;
}

//自動生成
vector<vector<int>> random_question(int size){
    vector<vector<int>> temp(size,vector<int>(size));
    //外枠
    for(int i = 0; i < size ; i++){
        for(int j = 0; j < size; j++){
            if( i == 0 || j == 0 || i == size - 1 || j == size - 1 ) temp[i][j] = 9;
            if( i % 2 == 0 && j % 2 == 0)temp[i][j] = 9;
        }
    }
    
    //内側ランダム生成（棒倒し法）
    random_device rnd;
    mt19937 mt(rnd());
    int direct;
    //1st step
    for(int i = 2; i < 3 ; i++){
        for(int j = 2; j < size-1; j++){
            
            if(i % 2 ==0 && j % 2 == 0){
                direct = mt() % 4;
                if(direct == 0){
                    if(temp[i-1][j] == 9){direct = 1;}
                    else{temp[i-1][j] = 9;}
                }
                if(direct == 1){
                    if(temp[i+1][j] == 9){direct = 2;}
                    else{temp[i+1][j] = 9;}
                }
                if(direct == 2){
                    if(temp[i+1][j] == 9){direct = 3;}
                    else{temp[i][j-1] = 9;}
                }
                if(direct == 3){
                    temp[i][j+1] = 9;
                }
            }
        }
    }
    //2nd step
    for(int i = 4; i < size-1 ; i++){
        for(int j = 2; j < size-1; j++){
            
            if(i % 2 ==0 && j % 2 == 0){
                direct = mt() % 4;
                if(direct == 0){
                    if(temp[i][j-1] == 9){direct = 1;}
                    else{temp[i][j-1] = 9;}
                }
                if(direct == 1){
                    if(temp[i][j+1] == 9){direct = 2;}
                    else{temp[i][j+1] = 9;}
                }
                if(direct == 2){
                    if(temp[i-1][j] == 9){direct = 3;}
                    else{temp[i-1][j] = 9;}
                }
                if(direct == 3){
                    temp[i+1][j] = 9;
                }
            }
        }
    }
    temp[1][1]=2;
    temp[size-2][size-2]=3;
    
    return temp;
}

#endif /* another_func_h */

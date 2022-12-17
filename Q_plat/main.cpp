//only codeing
//
//Q_solution.hpp

#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>

#include <queue>
#include <math.h>
#include <random>
#include <regex>

#include "another_func.hpp"

using std::endl;
using std::ofstream;
using namespace std;

//四方向のQテーブルから、最良一番いいのを出す
row select_best_action(vector<vector<int>> maze, vector<vector<q>> Q_table, row pos){
    
    //
    //いけるところのflagを立てる（0 or 1 でflag = 1に）
    //
    int flag_up = 0;
    int flag_down = 0;
    int flag_left = 0;
    int flag_right = 0;
    
    if(maze[pos.y][pos.x + 1] == 0 || maze[pos.y][pos.x + 1] == 1 || maze[pos.y][pos.x + 1] == 3){
        flag_right = 1;
    }
    if(maze[pos.y + 1][pos.x] == 0 || maze[pos.y + 1][pos.x] == 1 || maze[pos.y][pos.x] == 3){
        flag_down = 1;
    }
    if(maze[pos.y - 1][pos.x] == 0 || maze[pos.y - 1][pos.x] == 1 || maze[pos.y][pos.x + 1] == 3){
        flag_up = 1;
    }
    if(maze[pos.y][pos.x - 1] == 0 || maze[pos.y][pos.x - 1] == 1 || maze[pos.y][pos.x + 1] == 3){;
        flag_left = 1;
    }
    
    //
    //一番いい評価を持つマスをbest_actionとし、qtableを更新
    //
    int max_eva = 0;//最大評価値を保存
    string pos_flag = " ";//行く方向
    /*
    if( !((Q_table[pos.y][pos.x].u == Q_table[pos.y][pos.x].d) &&
          (Q_table[pos.y][pos.x].r == Q_table[pos.y][pos.x].l) &&
          (Q_table[pos.y][pos.x].u == Q_table[pos.y][pos.x].r))    ){
        if(flag_up == 1 && Q_table[pos.y][pos.x].u >= max_eva){max_eva = Q_table[pos.y][pos.x].u; pos_flag = "up";}
        if(flag_left == 1 && Q_table[pos.y][pos.x].l >= max_eva){max_eva = Q_table[pos.y][pos.x].l; pos_flag = "left";}
        if(flag_right == 1 && Q_table[pos.y][pos.x].r >= max_eva){max_eva = Q_table[pos.y][pos.x].r; pos_flag = "right";}
        if(flag_down == 1 && Q_table[pos.y][pos.x].d >= max_eva){max_eva = Q_table[pos.y][pos.x].d; pos_flag = "down";}
    }else{
     */
        // D → R → L → U
        if(flag_up == 1){ pos_flag = "up";}
        if(flag_left == 1 ){ pos_flag = "left";}
        if(flag_right == 1 ){ pos_flag = "right";}
        if(flag_down == 1){ pos_flag = "down";}
    //}
    
    //best actionに代入して、返す
    row best_action;
    if(pos_flag == "up"){ best_action = {pos.x,pos.y-1};}
    else if(pos_flag == "left"){ best_action = {pos.x-1,pos.y};}
    else if(pos_flag == "right"){ best_action = {pos.x+1,pos.y};}
    else if(pos_flag == "down"){ best_action = {pos.x,pos.y+1};}
    
    return best_action;
}

//行動決定関数 ... 一定の確率で、ベストでない動きをする
row select_action(vector<vector<int>> maze, vector<vector<q>> Q_table, row pos){
    
    //ε-greedyをここで使う
    row best = select_best_action(maze,Q_table,pos);
    
    //いけるマスを探すflag
    int flag_up = 0;
    int flag_down = 0;
    int flag_left = 0;
    int flag_right = 0;
    
    //いけるところのflagを立てる（0 or 1 or 3でflag = 1に）
    if(maze[pos.y][pos.x + 1] == 0 || maze[pos.y][pos.x + 1] == 1 || maze[pos.y][pos.x + 1] == 3){
        flag_right = 1;
    }
    if(maze[pos.y + 1][pos.x] == 0 || maze[pos.y + 1][pos.x] == 1 || maze[pos.y][pos.x] == 3){
        flag_down = 1;
    }
    if(maze[pos.y - 1][pos.x] == 0 || maze[pos.y - 1][pos.x] == 1 || maze[pos.y][pos.x + 1] == 3){
        flag_up = 1;
    }
    if(maze[pos.y][pos.x - 1] == 0 || maze[pos.y][pos.x - 1] == 1 || maze[pos.y][pos.x + 1] == 3){;
        flag_left = 1;
    }
    
    //cout << "flag : " <<flag_up<<flag_down<<flag_right<<flag_left<<endl;
    //
    //ε-greedy
    //
    
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    double seed = seed_gen() % 10;
    double epsilon = EPSILON;
    epsilon = 0.7;
    
    //epsilonの確率でbestactionをそのまま使う
    row agent_action = best;
    if(seed <= epsilon * 10){
        agent_action = best;
        cout << "* : normal action : x." <<  agent_action.x << ", y." << agent_action.y << endl;
    }else{
        //int other_action_pattern = flag_up + flag_down + flag_left + flag_right;
        int math_flag = seed_gen() % 4;
        if(math_flag == 0 && flag_right == 1){
            agent_action = {pos.x + 1 , pos.y};
        }else if(math_flag == 1 && flag_left == 1 ){
            agent_action = {pos.x - 1 , pos.y};
        }else if(math_flag == 2 && flag_down == 1){
            agent_action = {pos.x , pos.y + 1};
        }else if(math_flag == 3 && flag_up == 1){
            cout << "this" << endl;
            agent_action = {pos.x , pos.y - 1};
        }else{
            agent_action = best;
        }
        
        cout << "! : irregular action " << math_flag << " : x." <<  agent_action.x << ", y." << agent_action.y << endl;
    }
    
    //
    //Q table更新
    //
    
    //wallに行く報酬
    if(flag_up == 0) Q_table[pos.y][pos.x].u = -100;
    if(flag_down == 0) Q_table[pos.y][pos.x].d = -100;
    if(flag_left == 0) Q_table[pos.y][pos.x].l = -100;
    if(flag_right == 0) Q_table[pos.y][pos.x].r = -100;
  
    //行動に対する報酬
    row goal = {13,13};
    if(!(agent_action.x == goal.x && agent_action.y == goal.y)){
        //ゴール以外の手の報酬
        //上に移動している
        if(agent_action.x == pos.x && agent_action.y < pos.y ){
            Q_table[pos.y][pos.x].u += 10;
            
        }
        //左に移動している
        else if(agent_action.x > pos.x && agent_action.y == pos.y ){
            Q_table[pos.y][pos.x].l += 10;
        }
        //右に移動している
        else if(agent_action.x < pos.x && agent_action.y == pos.y ){
            Q_table[pos.y][pos.x].r += 10;
        }
        //下に移動している
        else if(agent_action.x == pos.x && agent_action.y > pos.y ){
            Q_table[pos.y][pos.x].d += 10;
        }
    }else{
        //ゴールの報酬
        if(agent_action.x == pos.x && agent_action.y < pos.y ) Q_table[pos.y][pos.x].u += 100;
        else if(agent_action.x > pos.x && agent_action.y == pos.y ) Q_table[pos.y][pos.x].l += 100;
        else if(agent_action.x < pos.x && agent_action.y == pos.y ) Q_table[pos.y][pos.x].r += 100;
        else if(agent_action.x == pos.x && agent_action.y > pos.y ) Q_table[pos.y][pos.x].d += 100;
    }
   
    //return
    return agent_action;
}

//epsodeを実行する
void Q_Learning_Platform(int epsode , vector<vector<int>> board , int size){
    //Qtable
    vector<vector<q>> Q_table(size , vector<q>(size , {0,0,0,0}));
    
    //forloop in epsode
    int goal_ = 0;
    row agent = {1,1}; //egent pos
    vector<vector<int>> maze = board;
    int step = 0; //step
    
    for(int i = 0; i < epsode ; i++){
        
        //1epsode ゴールするまで or MAX_STEP
        while(step <= STEP_MAX){
            
            //agentがゴールにいたらepsode終了
            if(board[agent.x][agent.y] == 3){
                cout << "" << endl;
                goal_ = 1;
                break;
            }
            
            //移動＆table更新
            agent = select_action(maze, Q_table, agent);
            step++;
        }
        //epsodeが終わったので盤面とエージェントの情報を初期化
        cout << "goal? :" << goal_ << " | epsode : " << i+1 << endl;
        agent = {1,1};
        maze = board;
        step = 0;
    }
    
    cout << "Finish : Result >> q_table.csv" << endl;
}

//実行用 main()
int main(){
    vector<vector<int>> grid =
//     {{9,9,9,9,9,9,9,9},
//        {9,2,9,0,0,0,0,9},
//        {9,0,9,0,9,9,0,9},
//        {9,0,9,0,0,9,0,9},
//        {9,0,0,0,9,0,0,9},
//        {9,0,9,9,9,9,9,9},
//        {9,0,0,0,0,0,3,9},
        //{9,9,9,9,9,9,9,9}};
                                {{9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
                                 {9,2,9,0,0,0,0,0,0,0,0,0,9,0,9},
                                 {9,0,9,9,9,9,9,9,9,9,9,0,9,0,9},
                                 {9,0,9,0,0,9,0,0,0,0,0,0,0,0,9},
                                 {9,0,9,9,9,9,0,9,9,9,9,9,9,9,9},
                                 {9,0,0,9,0,9,0,9,0,0,0,0,0,9,9},
                                 {9,9,0,9,0,9,0,9,9,9,0,9,0,9,9},
                                 {9,9,0,9,0,9,0,0,0,0,0,9,0,0,9},
                                 {9,9,0,9,0,9,9,9,9,9,9,9,9,0,9},
                                 {9,9,0,9,0,9,0,0,0,0,0,0,0,0,9},
                                 {9,9,0,9,0,0,0,9,9,9,9,9,9,0,9},
                                 {9,9,0,9,9,0,9,9,0,0,0,0,0,0,9},
                                 {9,9,0,9,9,0,9,9,0,9,9,9,9,9,9},
                                 {9,9,0,0,0,0,9,0,0,0,0,0,0,3,9},
                                 {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}};
    
    int maze_size = 13;
    grid = random_question(maze_size);
    print(grid);
    
    ofstream ofs("test.csv");
    for(int i = 0 ; i < grid.size(); i++){
        for(int j = 0 ; j < grid[0].size() ; j++){
            ofs << grid[i][j] << ",";
        }
        ofs << endl;
    }
    Q_Learning_Platform(EPISODE_MAX , grid , maze_size); //epsode , data , size
    
    return 0;
}

//error & to do
//ε-greedyを実装 <- まだ

//only codeing
//
//Q_solution.hpp

#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
#include <stdlib.h>
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
   
    //
    //  まず行動をε-greedyを使って確定
    //  ・best -> 現段階の最適解
    //  ・agent_action -> ε-greedyを用いて決まった行動
    //
    
    row best = select_best_action(maze,Q_table,pos);
    row agent_action = best;
    
    //いけるマスを探すflag
    int flag_up = 0;
    int flag_down = 0;
    int flag_left = 0;
    int flag_right = 0;
    
    //いけるところのflagを立てる（0 or 1 or 3でflag = 1に）
    if(maze[pos.y][pos.x + 1] == 0 || maze[pos.y][pos.x + 1] == 1 || maze[pos.y][pos.x + 1] == 3) flag_right = 1;
    if(maze[pos.y + 1][pos.x] == 0 || maze[pos.y + 1][pos.x] == 1 || maze[pos.y][pos.x] == 3) flag_down = 1;
    if(maze[pos.y - 1][pos.x] == 0 || maze[pos.y - 1][pos.x] == 1 || maze[pos.y][pos.x + 1] == 3) flag_up = 1;
    if(maze[pos.y][pos.x - 1] == 0 || maze[pos.y][pos.x - 1] == 1 || maze[pos.y][pos.x + 1] == 3) flag_left = 1;
    
    //* : normal action or ! : irregular action を選択
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    double seed = seed_gen() % 10;
    double epsilon = EPSILON;
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
    //  Q table更新
    //
    
    //どこに行ってるのか調べる（方向）
    string where_agent_going = "";
    if(agent_action.x == pos.x && agent_action.y < pos.y ){   where_agent_going = "u"; }
    else if(agent_action.x > pos.x && agent_action.y == pos.y ){   where_agent_going = "l";}
    else if(agent_action.x < pos.x && agent_action.y == pos.y ){  where_agent_going = "r";}
    else if(agent_action.x == pos.x && agent_action.y > pos.y ){  where_agent_going = "d";}
    
    //今ステップの報酬とゴールを定義
    double step_value = 0;
    int gol = int(maze[0].size());
    row goal = {gol,gol};
    
    //行動に対する報酬を使って更新
    if(agent_action.x == goal.x && agent_action.y == goal.y){ step_value = 100; } //goal
    else { step_value = 10;} //not goal

    //update table
    if(where_agent_going == "u") Q_table[pos.y][pos.x].u = (ALPHA * step_value) + Q_table[pos.y][pos.x].u;
    if(where_agent_going == "l") Q_table[pos.y][pos.x].l = (ALPHA * step_value) + Q_table[pos.y][pos.x].l;
    if(where_agent_going == "r") Q_table[pos.y][pos.x].r = (ALPHA * step_value) + Q_table[pos.y][pos.x].r;
    if(where_agent_going == "d") Q_table[pos.y][pos.x].d = (ALPHA * step_value) + Q_table[pos.y][pos.x].d;
    
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
            cout << step <<endl;
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
    
    int maze_size = 101;
    //grid = random_question(maze_size);
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

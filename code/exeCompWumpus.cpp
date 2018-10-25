#include "KB.h"
#include "algorithm.h"
#include "execute.h"
#include "wumpus.h"
#include <iostream>
#include <string>
#include "time.h"
using namespace std;

void compWumpus(){
    int boardSize = 6;
    int wumpusNum = 2;
    int pitNum = 4;
    int goldNum = 1;
    WumpusAgent* wa = new WumpusAgent(boardSize, pitNum, wumpusNum, goldNum);
    wa->playWumpus();
}

int main(){
    compWumpus();
}
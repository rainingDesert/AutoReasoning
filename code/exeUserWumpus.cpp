#include "KB.h"
#include "algorithm.h"
#include "wumpus.h"
#include <iostream>
#include <string>
#include "time.h"
using namespace std;

void userWumpus(){
    int boardSize = 6;
    int wumpusNum = 2;
    int pitNum = 4;
    int goldNum = 1;
    WumpusImp* wimp = new WumpusImp(boardSize, pitNum, wumpusNum, goldNum);
    wimp->gamePlay();
}

int main(){
    userWumpus();
}
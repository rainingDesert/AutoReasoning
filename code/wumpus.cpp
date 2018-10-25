#include "wumpus.h"
#include "KB.h"
#include "algorithm.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

///////////////////////////--------------wumpus world---------------//////////////////////////////////////////
//construtor of wumpus world
WumpusWorld::WumpusWorld(int boardSize, int pitNum, int wumpusNum, int goldNum){
    this->boardSize = boardSize;
    this->pitNum = pitNum;
    this->wumpusNum = wumpusNum;
    this->goldNum = goldNum;
    this->pitPos = new int*[pitNum];
    this->wumpusPos = new int*[wumpusNum];
    this->goldPos = new int*[goldNum];
    this->myPos = new int[2];
    this->statusNum = 5;
    this->statusName = new string[5];
    this->statusName[0] = "stech";
    this->statusName[1] = "breeze";
    this->statusName[2] = "glitter";
    this->statusName[3] = "bump";
    this->statusName[4] = "scream";
    this->arrow = true;
    this->createWorld();
}

//create wumpus world
void WumpusWorld::createWorld(){
    //initialize random seed 
    bool** boardAva = new bool*[this->boardSize];
    for(int i = 0; i < this->boardSize; i++){
        boardAva[i] = new bool[this->boardSize];
        for(int j = 0; j < this->boardSize;j++){
            boardAva[i][j] = true;
        }
    }
    srand(time(NULL));
    
    myPos[0] = 0;
    myPos[1] = 0;

    //initialize pit position
    for(int pitId = 0; pitId < this->pitNum; pitId++){
        this->pitPos[pitId] = new int[2];
        int row = rand() % this->boardSize;
        int column = rand() % this->boardSize;
        while(abs(row - myPos[0]) + abs(column - myPos[1]) == 0 || !boardAva[row][column]){
            row = rand() % this->boardSize;
            column = rand() % this->boardSize;
        }
        this->pitPos[pitId][0] = row;
        this->pitPos[pitId][1] = column;
        boardAva[row][column] = false;
    }

    //initialize wumpus position
    for(int wumpusId = 0; wumpusId < this->wumpusNum; wumpusId++){
        this->wumpusPos[wumpusId] = new int[2];
        int row = rand() % this->boardSize;
        int column = rand() % this->boardSize;
        while(abs(row - myPos[0]) + abs(column - myPos[1]) == 0 || !boardAva[row][column]){
            row = rand() % this->boardSize;
            column = rand() % this->boardSize;
        }
        this->wumpusPos[wumpusId][0] = row;
        this->wumpusPos[wumpusId][1] = column;
        boardAva[row][column] = false;
    }

    //gold position
    for(int goldId = 0; goldId < this->goldNum; goldId++){
        this->goldPos[goldId] = new int[2];
        int row = rand() % this->boardSize;
        int column = rand() % this->boardSize;
        while(abs(row - myPos[0]) + abs(column - myPos[1]) == 0|| !boardAva[row][column]){
            row = rand() % this->boardSize;
            column = rand() % this->boardSize;
        }
        this->goldPos[goldId][0] = row;
        this->goldPos[goldId][1] = column;
        boardAva[row][column] = false;
    }
}

//check stench (wumpus nearby)
bool WumpusWorld::checkStench(int* myPos){
    for(int wumpusId = 0; wumpusId < this->wumpusNum; wumpusId ++){
        if(abs(this->wumpusPos[wumpusId][0] - myPos[0]) == 1 && abs(this->wumpusPos[wumpusId][1] - myPos[1]) == 0){
            return true;
        }
        else if(abs(this->wumpusPos[wumpusId][0] - myPos[0]) == 0 && abs(this->wumpusPos[wumpusId][1] - myPos[1]) == 1){
            return true;
        }
    }
    return false;
}

//check breeze (pit nearby)
bool WumpusWorld::checkBreeze(int* myPos){
    for(int pitId = 0; pitId < this->pitNum; pitId++){
        if(abs(this->pitPos[pitId][0] - myPos[0]) == 1 && abs(this->pitPos[pitId][1] - myPos[1]) == 0){
            return true;
        }
        else if(abs(this->pitPos[pitId][0] - myPos[0]) == 0 && abs(this->pitPos[pitId][1] - myPos[1]) == 1){
            return true;
        }
    }
    return false;
}

//check glitter (gold here)
bool WumpusWorld::checkGlitter(int* myPos){
    for(int goldId = 0; goldId < this->goldNum; goldId ++){
        if(this->goldPos[goldId][0] == myPos[0] && this->goldPos[goldId][1] == myPos[1]){
            return true;
        }
    }
    return false;
}

//check bump
bool WumpusWorld::checkBump(int* tryPos){
    if(tryPos[0] >= this->boardSize || tryPos[1] >= this->boardSize || tryPos[0] < 0 || tryPos[1] < 0){
        return true;
    }
    return false;
}

//check scream
bool WumpusWorld::checkScream(int* myPos){
    for(int wumpusId = 0; wumpusId < this->wumpusNum; wumpusId ++){
        if(this->wumpusPos[wumpusId][0] == myPos[0] && this->wumpusPos[wumpusId][1] == myPos[1]){
            return true;
        }
    }
    return false;
}

//get status
bool* WumpusWorld::getStatus(){
    bool* status = new bool[5];
    status[0] = this->checkStench(this->myPos);
    status[1] = this->checkBreeze(this->myPos);
    status[2] = this->checkGlitter(this->myPos);
    status[3] = this->checkBump(this->myPos);
    status[4] = this->checkScream(this->myPos);
    return status;
}

//do the move
void WumpusWorld::doMove(char action){
    if(action == 'u' || action == 'U'){
        this->myPos[0] += 1;
    }
    else if(action == 'd' || action == 'D'){
        this->myPos[0] -= 1;
    }
    else if(action == 'l' || action == 'L'){
        this->myPos[1] -= 1;
    }
    else if(action == 'r' || action == 'R'){
        this->myPos[1] += 1;
    }
}

//do shoot
void WumpusWorld::doShoot(){
    this->arrow = false;
}

//move called by outside
bool* WumpusWorld::move(char action){
    bool* status = NULL;
    int* tempPos = new int[2];
    tempPos[0] = this->myPos[0];
    tempPos[1] = this->myPos[1];
    this->doMove(action);
    status = this->getStatus();
    if(status[3]){
        this->myPos[0] = tempPos[0];
        this->myPos[1] = tempPos[1];
    }
    return status;
}

//get pit position
int** WumpusWorld::getPit(){
    return this->pitPos;
}

//get wumpus position
int** WumpusWorld::getWumpus(){
    return this->wumpusPos;
}
//get gold position
int** WumpusWorld::getGold(){
    return this->goldPos;
}

//get my position
int* WumpusWorld::getMyPos(){
    return this->myPos;
}

//get board size
int WumpusWorld::getBoardSize(){
    return this->boardSize;
}

//get status Num
int WumpusWorld::getStatusNum(){
    return this->statusNum;
}

//get names of status
string* WumpusWorld::getStatusName(){
    return this->statusName;
}

//check pit
bool WumpusWorld::checkPit(int* pos){
    for(int pitId = 0; pitId < this->pitNum; pitId++){
        if(this->pitPos[pitId][0] == pos[0] && this->pitPos[pitId][1] == pos[1]){
            return true;
        }
    }
    return false;
}

//check wumpus
bool WumpusWorld::checkWumpus(int* pos){
    return this->checkScream(pos);
}

//check gold
bool WumpusWorld::checkGold(int* pos){
    return this->checkGlitter(pos);
}

//grad
bool WumpusWorld::grab(){
    if(this->goldNum <= 0){
        return false;
    }
    for(int goldId = 0; goldId < this->goldNum; goldId++){
        if(this->goldPos[goldId][0] == this->myPos[0] && this->goldPos[goldId][1] == this->myPos[1]){
            this->goldPos[goldId][0] = this->goldPos[this->goldNum - 1][0];
            this->goldPos[goldId][1] = this->goldPos[this->goldNum - 1][1];
            this->goldNum--;
            return true;
        }
    }
    return false;
}

//do shoot
bool WumpusWorld::shoot(){
    if(!this->arrow){
        return false;
    }
    if(this->checkWumpus(this->myPos)){
        this->arrow = false;
        for(int wumpusId = 0; wumpusId < this->wumpusNum; wumpusId++){
            if(this->wumpusPos[wumpusId][0] == this->myPos[0] && this->wumpusPos[wumpusId][1] == this->myPos[1]){
                this->wumpusPos[wumpusId][0] = this->wumpusPos[this->wumpusNum - 1][0];
                this->wumpusPos[wumpusId][1] = this->wumpusPos[this->wumpusNum - 1][1];
                break;
            }
        }
        this->wumpusNum--;
    }
    return true;
}

//check whether game over
bool WumpusWorld::checkEnd(){
    if(this->checkPit(this->myPos) || this->checkWumpus(this->myPos) || this->goldNum == 0){
        return true;
    }
    return false;
}

//check whether remain gold
bool WumpusWorld::noGold(){
    return (this->goldNum == 0);
}

//set position
void WumpusWorld::setMove(int* pos){
    this->myPos[0] = pos[0];
    this->myPos[1] = pos[1];
}

///////////////////////////--------------wumpus UI---------------//////////////////////////////////////////
//constructor
WumpusUI::WumpusUI(){}

//final display board
void WumpusUI::finalDisplayBoard(WumpusWorld* ww){
    //display board
    int* myPos = ww->getMyPos();
    int* curPos = new int[2];

    for(int row = ww->getBoardSize() - 1; row >= 0; row--){
        cout << "|";
        for(int column = 0; column < ww->getBoardSize(); column++){
            curPos[0] = row;
            curPos[1] = column;
            if(myPos[0] == row && myPos[1] == column){
                if(ww->checkPit(curPos))
                    cout << setw(3) << "*/P" << setw(3) << "|";
                else if(ww->checkWumpus(curPos))
                    cout << setw(3) << "*/W" << setw(3) << "|";
                else
                    cout << setw(3) << "*/G" << setw(3) << "|";
            }
            else if(ww->checkPit(curPos)){
                cout << setw(3) << "P" << setw(3) << "|";
            }
            else if(ww->checkWumpus(curPos)){
                cout << setw(3) << "W" << setw(3) << "|";
            }
            else if(ww->checkGold(curPos)){
                cout << setw(3) << "G" << setw(3) << "|";
            }
            else{
                cout << setw(3) << " " << setw(3) << "|";
            }
        }
        cout << endl;
    }
}

//display board
void WumpusUI::displayBoard(WumpusWorld* ww){
    //display board
    int* myPos = ww->getMyPos();

    for(int row = ww->getBoardSize() - 1; row >= 0; row--){
        cout << "|";
        for(int column = 0; column < ww->getBoardSize(); column++){
            if(myPos[0] == row && myPos[1] == column){
                cout << setw(3) << "*" << setw(3) << "|";
            }
            else{
                cout << setw(3) << " " << setw(3) << "|";
            }
        }
        cout << endl;
    }
}

//display current statue
void WumpusUI::displayStatus(WumpusWorld* ww, bool* status){
    bool nothing = true;
    string* statusName = ww->getStatusName();

    cout << "You use your talent of hunter, and feel ";
    for(int i = 0; i < ww->getStatusNum(); i++){
        if(status[i]){
            cout << statusName[i] << ", ";
            nothing = false;
        }
    }
    if(nothing){
        cout << "nothing";
    }

    cout << endl;
}

//start the game
void WumpusUI::gameStart(){
    cout << "Welcome! Brave and Talented Hunter" << endl;
    cout << "I will show you the map" << endl;
    cout << "When there is a wumpus nearby, stench you will smell!" << endl;
    cout << "When there is a pit nearby, breeze you will feel!" << endl;
    cout << "When current room have gold, glitter you will see!" << endl;
    cout << "When no room on your move, bump you will act!" << endl;
    cout << "When there is a wumpus here, scream you will hear!" << endl;
}

//get move
char WumpusUI::getMove(){
    char move = '0';
    cout << "Now, talented hunter, please select your move: " << endl;
    cout << "(\"U\" to go up, \"D\" to go down, \"L\" to go left, \"R\" to go right): ";
    cin >> move;
    while(move != 'U' && move != 'u' && move != 'D' && move != 'd' && move != 'L' && move != 'l' && move != 'R' && move != 'r' ){
        cout << "Wrong move, please try again" << endl;
        cout << "(\"U\" to go up, \"D\" to go down, \"L\" to go left, \"R\" to go right): ";
        cin >> move;
    } 
    return move;
}

//when meet wumpus
bool WumpusUI::monsterMove(WumpusWorld* ww){
    char move = '0';
    cout << endl;
    cout << "Wow! A Scream! That must be Wumpus! Shoot it!" << endl;
    cout << "Shoot or not (\"Y\" or \"N\"): ";
    cin >> move;
    while(move != 'y' && move != 'Y' && move != 'n' && move != 'N'){
        cout << "No! Choose whether to shoot! (\"Y\" or \"N\"): ";
        cin >> move;
    }
    if(move == 'Y' || move == 'y'){
        if(ww->shoot()){
            cout << endl << "Wonderful! You make it sleep forever!" << endl;
            return true;
        }
        else{
            cout << endl << "Oh! You don't have arrow!" << endl;
            return false;
        }
    }
    return false;
}

//when meet gold
bool WumpusUI::goldMove(WumpusWorld* ww){
    char move = '0';
    cout << endl;
    cout << "Wow! Gold! Grab it!" << endl;
    cout << "Grab or not (\"Y\" or \"N\"): ";
    cin >> move;
    while(move != 'y' && move != 'Y' && move != 'n' && move != 'N'){
        cout << "No! Choose whether to Grad Gold! (\"Y\" or \"N\"): ";
        cin >> move;
    }
    if(move == 'Y' || move == 'y'){
        if(ww->grab()){
            cout << endl << "Wonderful! You Grab it!" << endl;
            return true;
        }
        else{
            cout << endl << "Oh! You can't get the gold!" << endl;
            return false;
        }
    }
    return false;
}

//game over
void WumpusUI::gameOver(char type){
    if(type == 'M'){
        cout << endl << "You Died Under the Claw of wumpus" << endl;
    }
    else if(type == 'G'){
        cout << endl << "You Win by Getting all the gold" << endl;
    }
    else if(type == 'P'){
        cout << endl << "You Died Under the Cold Dark Pit" << endl;
    }
}

///////////////////////////--------------wumpus start---------------//////////////////////////////////////////
//constructor
WumpusImp::WumpusImp(int boardSize, int pitNum, int wumpusNum, int goldNum){
    this->ww = new WumpusWorld(boardSize, pitNum, wumpusNum, goldNum);
    this->wui = new WumpusUI();
}

//start the game
void WumpusImp::gamePlay(){
    //start game UI
    bool* status = NULL;
    bool end = false;
    status = this->ww->getStatus();
    this->wui->gameStart();
    cout << "----------------------------------------------------------" << endl;
    this->wui->displayBoard(this->ww);
    this->wui->displayStatus(this->ww, status);
    delete status;
    status = NULL;

    while(!end){
        //make move
        status = this->ww->move(this->wui->getMove());

        //special move
        if(status[4]){
            this->wui->monsterMove(this->ww);
        }
        if(status[2]){
            this->wui->goldMove(this->ww);
        }
        status = this->ww->getStatus();
        
        //check game over
        end = this->ww->checkEnd();
        if(end){
            if(status[4])
                this->wui->gameOver('M');
            else if(this->ww->noGold())
                this->wui->gameOver('G');
            else
                this->wui->gameOver('P');
            break;
        }

        //show map
        cout << endl << endl;
        this->wui->displayBoard(this->ww);
        this->wui->displayStatus(this->ww, status);
        delete status;
    }
    cout << endl;
    cout << "the entire board is: " << endl;
    this->wui->finalDisplayBoard(this->ww);
}

///////////////////////////--------------wumpus by computer---------------//////////////////////////////////////////
struct WumpusAgent::kbCNF{
    string** board;
    int boardSize;
    bool arrow;
    kbCNF(string** board, int boardSize){
        this->board = board;
        this->boardSize = boardSize;
        this->arrow = true;
    }
    ~kbCNF(){
        if(this->board != NULL){
            for(int row = 0; row < this->boardSize; row++){
                if(this->board[row] != NULL){
                    delete[] this->board[row];
                    this->board[row] = NULL;
                }
            }
            delete[] this->board;
            this->board = NULL;
        }
    }
};

//constructor of WumpusAgent
WumpusAgent::WumpusAgent(int boardSize, int pitNum, int wumpusNum, int goldNum){
    this->ww = new WumpusWorld(boardSize, pitNum, wumpusNum, goldNum);
    string** board = new string*[boardSize];
    for(int row = 0; row < boardSize; row++){
        board[row] = new string[boardSize];
        for(int column = 0; column < boardSize; column++){
            board[row][column] = " ";
        }
    }
    this->cnf = new kbCNF(board, boardSize);
    this->cnf->board[0][0] = "*";
}

//function helps translate into clause
void WumpusAgent::helpTrans(string* adjacents, string* moveSym, string* totalClas, string temp, int adjNum, int* totNum){
    if(adjNum == 0){
        totalClas[*totNum++] = temp + "V" + adjacents[adjNum];
        totalClas[*totNum++] = temp + "V" + moveSym[adjNum];
        return;
    }
    helpTrans(adjacents, moveSym, totalClas, temp + "V" + adjacents[adjNum], adjNum - 1, totNum);
    helpTrans(adjacents, moveSym, totalClas, temp + "V" + moveSym[adjNum], adjNum - 1, totNum);
}

//change location
string* WumpusAgent::move(int* myPos, string move){
    string* totalClas = new string[100];
    string me = "L" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    string* adjacents = new string[4];
    string* moveSym = new string[4];
    string* result = new string[2];
    int adjNum = 0;
    int totNum = 0;

    //translate into clause
    if(myPos[0] != 0)
        adjacents[adjNum++] = "M" + to_string(myPos[0] - 1) + "," + to_string(myPos[1]);
    if(myPos[0] != this->ww->getBoardSize() - 1)
        adjacents[adjNum++] = "M" + to_string(myPos[0] + 1) + "," + to_string(myPos[1]);
    if(myPos[1] != 0)
        adjacents[adjNum++] = "M" + to_string(myPos[0]) + "," + to_string(myPos[1] - 1);
    if(myPos[1] != this->ww->getBoardSize() - 1)
        adjacents[adjNum++] = "M" + to_string(myPos[0]) + "," + to_string(myPos[1] + 1);

    moveSym[0] = "D", moveSym[1] = "U", moveSym[2] = "L", moveSym[3] = "R";

    //first translate
    this->helpTrans(adjacents, moveSym, totalClas, "-" + me, adjNum - 1, &totNum);

    //second translate
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum++] = "-" + adjacents[adjId] + "V" + "-" + moveSym[adjId] + "V" + me;
    }

    //third translate
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum++] = "-" + adjacents[adjId] + "V" + "L" + adjacents[adjId].substr(1);
    }

    //fourth translate
    for(int adjId = 0; adjId < adjNum; adjId++){
        for(int adjId2 = adjId; adjId2 < adjNum; adjId2++){
            totalClas[totNum++] = "-" + moveSym[adjId] + "V" + "-" + moveSym[adjId2];
        }
    }

    //fifth translate
    totalClas[totNum++] = move;
    totalClas[totNum++] = me;

    //update my position and visited position
    DPLL* dpll = NULL;
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] = "-" + adjacents[adjId];
        dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
        if(!dpll->execution()){
            result[0] = adjacents[adjId];
            break;
        }
    }

    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] = "-L" + adjacents[adjId].substr(1);
        dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
        if(!dpll->execution()){
            result[1] = "L" + adjacents[adjId].substr(1);
            break;
        }
    }

    delete[] totalClas;
    delete[] adjacents;
    delete[] moveSym;

    return result;
}

//change status
string* WumpusAgent::changeStatus(int* myPos, string* wumpus, string* pit, string* status, int wumpusNum, int pitNum, int statusNum){

    string* totalClas = new string[100];
    string* adjacents = new string[4];
    string* result = new string[20];
    int adjNum = 0;
    int totNum = 0;
    int resNum = 1;

    //translate into clause
    if(myPos[0] != 0)
        adjacents[adjNum++] = to_string(myPos[0] - 1) + "," + to_string(myPos[1]);
    if(myPos[0] != this->ww->getBoardSize() - 1)
        adjacents[adjNum++] = to_string(myPos[0] + 1) + "," + to_string(myPos[1]);
    if(myPos[1] != 0)
        adjacents[adjNum++] = to_string(myPos[0]) + "," + to_string(myPos[1] - 1);
    if(myPos[1] != this->ww->getBoardSize() - 1)
        adjacents[adjNum++] = to_string(myPos[0]) + "," + to_string(myPos[1] + 1);

    //first translate
    totalClas[totNum] = "-strench";
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] += "VW" + adjacents[adjId];
    }
    totNum ++;
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum++] = "-W" + adjacents[adjId] + "Vstench";
    }

    //second translate
    totalClas[totNum] = "-breeze";
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] += "VP" + adjacents[adjId];
    }
    totNum ++;
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum++] = "-P" + adjacents[adjId] + "Vbreeze";
    }

    //third translate
    totalClas[totNum++] = "-glitterVG" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    totalClas[totNum++] = "-G" + to_string(myPos[0]) + "," + to_string(myPos[1]) + "Vglitter";

    //fourth translate
    totalClas[totNum++] = "-screamVW" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    totalClas[totNum++] = "-W" + to_string(myPos[0]) + "," + to_string(myPos[1]) + "Vscream";

    //fifth translate
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum++] = "-W" + adjacents[adjId] + "V-P" + adjacents[adjId];
    }

    //sixth translate
    for(int wumpusId = 0; wumpusId < wumpusNum; wumpusId++){
        totalClas[totNum++] = wumpus[wumpusId];
    }
    for(int pitId = 0; pitId < pitNum; pitId++){
        totalClas[totNum++] = pit[pitId];
    }
    for(int statusId = 0; statusId < statusNum; statusId++){
        totalClas[totNum++] = status[statusId];
    }

    //update status
    DPLL* dpll = NULL;
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] = "-W" + adjacents[adjId];
        dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
        if(!dpll->execution()){
            result[resNum++] = "W" + adjacents[adjId];
        }
        else{
            totalClas[totNum] = "W" + adjacents[adjId];
            delete dpll;
            dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
            if(!dpll->execution()){
                result[resNum++] = "-W" + adjacents[adjId];
            }
            else{
                result[resNum++] = "!W" + adjacents[adjId];
            }
        }
    }

    delete dpll;
    dpll = NULL;
    for(int adjId = 0; adjId < adjNum; adjId++){
        totalClas[totNum] = "-P" + adjacents[adjId];
        dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
        if(!dpll->execution()){
            result[resNum++] = "P" + adjacents[adjId];
        }
        else{
            totalClas[totNum] = "P" + adjacents[adjId];
            delete dpll;
            dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
            if(!dpll->execution()){
                result[resNum++] = "-P" + adjacents[adjId];
            }
            else{
                result[resNum++] = "!P" + adjacents[adjId];
            }
        }
    }

    delete dpll;
    dpll = NULL;
    totalClas[totNum] = "-G" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
    if(!dpll->execution()){
        result[resNum++] = "G" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    }
    else{
        result[resNum++] = "-G" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    }

    delete dpll;
    dpll = NULL;
    totalClas[totNum] = "-W" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    dpll = new DPLL(new ClaForDPLL(totalClas, totNum + 1));
    if(!dpll->execution()){
        result[resNum++] = "W" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    }
    else{
        result[resNum++] = "-W" + to_string(myPos[0]) + "," + to_string(myPos[1]);
    }

    result[0] = to_string(resNum - 1);

    delete[] totalClas;
    delete[] adjacents;
    delete dpll;

    return result;
}

//add knowledge base
string* WumpusAgent::checkOK(string* pit, string* wumpus, int pitNum, int wumpusNum){
    string* totClas = new string[300];
    string* result = new string[this->ww->getBoardSize() * this->ww->getBoardSize() + 1];
    int totNum = 0;
    int resNum = 1;

    //first translate
    for(int row = 0; row < this->ww->getBoardSize(); row++){
        for(int column = 0; column < this->ww->getBoardSize(); column++){
            totClas[totNum++] = "-OK"+ to_string(row) +"," + to_string(column) +"V-P" + to_string(row) +"," + to_string(column);
            totClas[totNum++] = "-OK"+ to_string(row) +"," + to_string(column) +"V-W" + to_string(row) +"," + to_string(column);
            totClas[totNum++] = "P" + to_string(row) +"," + to_string(column) + "VW" + to_string(row) +"," + to_string(column) \
                                    + "VOK" + to_string(row) +"," + to_string(column);
        }
    }

    //second translate
    for(int pitId = 0; pitId < pitNum; pitId++){
        totClas[totNum++] = pit[pitId];
    }
    for(int wumpusId = 0; wumpusId < wumpusNum; wumpusId++){
        totClas[totNum++] = wumpus[wumpusId];
    }

    //update OK
    DPLL* dpll = NULL;
    for(int row = 0; row < this->ww->getBoardSize(); row++){
        for(int column = 0; column < this->ww->getBoardSize(); column++){
            totClas[totNum] = "-OK"+ to_string(row) +"," + to_string(column);
            dpll = new DPLL(new ClaForDPLL(totClas, totNum + 1));
            if(!dpll->execution()){
                result[resNum++] = "OK" + to_string(row) +"," + to_string(column);
            }
            delete dpll;
            dpll = NULL;
        }
    }
    result[0] = to_string(resNum - 1);
    delete[] totClas;
    totClas = NULL;

    return result;
}

//shoot
string WumpusAgent::shoot(string arrow, string shoot){
    string* totClas = new string[4];
    string result;
    totClas[0] = "-arrowVshootVnewArrow";
    totClas[1] = arrow;
    totClas[2] = shoot;

    totClas[3] = "-newArrow";
    DPLL* dpll = new DPLL(new ClaForDPLL(totClas, 4));
    if(dpll->execution()){
        result = "arrow";
    }
    else{
        result = "-arrow";
    }
    delete dpll;
    return result;
}

//update map
void WumpusAgent::updateMap(bool* status){
    int* myPos = new int[2];        //my position
    int pitNum = 0;                 //number of pit status
    int wumpusNum = 0;              //number of wumpus status 
    string* result = NULL;          //status result
    string* pit = new string[this->cnf->boardSize * this->cnf->boardSize];     //status of pit
    string* wumpus = new string[this->cnf->boardSize * this->cnf->boardSize];  //status of wumpus
    string curStatus[4] = {"stench", "breeze", "glitter", "scream"};

    //scan the map
    for(int row = 0; row < this->cnf->boardSize; row++){
        for(int column = 0; column < this->cnf->boardSize; column++){
            if(this->cnf->board[row][column] == " "){
                continue;
            }
            else if(this->cnf->board[row][column] == "*"){
                myPos[0] = row;
                myPos[1] = column;
            }
            else if(this->cnf->board[row][column] == "P"){
                pit[pitNum++] = "P" + to_string(row) + "," + to_string(column);
            }
            else if(this->cnf->board[row][column] == "W"){
                wumpus[wumpusNum++] = "W" + to_string(row) + "," + to_string(column);
            }
        }
    }

    //update status
    if(!status[0]){
        curStatus[0] = "-" + curStatus[0];
    }
    if(!status[1]){
        curStatus[1] = "-" + curStatus[1];
    }
    if(!status[2]){
        curStatus[2] = "-" + curStatus[2];
    }
    if(!status[4]){
        curStatus[3] = "-" + curStatus[3];
    }

    result = this->changeStatus(myPos, wumpus, pit, curStatus, wumpusNum, pitNum, 4); 

    //update map
    int resNum = stoi(result[0]);
    int mayNum = 0;
    int** mayPos = new int*[resNum];
    for(int resId = 0; resId < resNum; resId++){

        int start = 1;
        if(result[resId + 1][0] == '-' || result[resId + 1][0] == '!'){
            start = 2;
        }

        int pos1 = stoi(result[resId + 1].substr(start, result[resId + 1].find(",") - start));
        int pos2 = stoi(result[resId + 1].substr(result[resId + 1].find(",") + 1));
        string mark = result[resId + 1].substr(0, start);

        if(mark[0] == '-'){     //situations of -G or -W
            int mayId = 0;
            for(;mayId < mayNum; mayId++){
                if(mayPos[mayId][0] == pos1 && mayPos[mayId][1] == pos2){
                    if(this->cnf->board[pos1][pos2] != "X")
                        this->cnf->board[pos1][pos2] = "S";
                    break;
                }
            }
            if(mayId == mayNum){
                mayPos[mayNum] = new int[2];
                mayPos[mayNum][0] = pos1;
                mayPos[mayNum][1] = pos2;
                mayNum++;
            }
            continue;
        }

        if(start == 1){
            this->cnf->board[pos1][pos2] = mark;
        }
        else if(this->cnf->board[pos1][pos2] == "X" || this->cnf->board[pos1][pos2] == "S"){
            continue;
        }
        else if(this->cnf->board[pos1][pos2] == " "){
            this->cnf->board[pos1][pos2] = mark;
            continue;
        }
        else if(this->cnf->board[pos1][pos2] == "!W"){
            this->cnf->board[pos1][pos2] = mark;
        }
    }

    this->cnf->board[myPos[0]][myPos[1]] = "*";

    delete[] pit;
    delete[] wumpus;
    for(int i = 0; i < mayNum; i++){
        delete[] mayPos[i];
    }
    delete[] mayPos;
}

//get unvisited and OK position
bool* WumpusAgent::ask(string* kb, string* check, int kbNum, int checkNum){
    string* clause = new string[kbNum + 1];
    bool* result = new bool[checkNum];
    DPLL* dpll = NULL;

    for(int claId = 0; claId < kbNum; claId++){
        clause[claId] = kb[claId];
    }
    for(int checkId = 0; checkId < checkNum; checkId++){
        clause[kbNum] = "-" + check[checkNum];
        dpll = new DPLL(new ClaForDPLL(clause, kbNum + 1));
        result[checkId] = !(dpll->execution());
    }
    delete[] clause;
    return result;
}

//get a route
int* WumpusAgent::planRoute(int** posRoutes, int* myPos, int routeNum){
    int* pos = new int[2];
    int minDis = 2 * this->ww->getBoardSize() + 1;
    int selId = -1;

    for(int routeId = 0; routeId < routeNum; routeId++){
        int newDis = abs(posRoutes[routeId][0] - myPos[0]) + abs(posRoutes[routeId][1] - myPos[1]);
        if(newDis < minDis){
            minDis = newDis;
            selId = routeId;
        }
    }
    
    pos[0] = posRoutes[selId][0];
    pos[1] = posRoutes[selId][1];

    return pos;
}

//show the board
void WumpusAgent::showBoard(){
    for(int row = this->cnf->boardSize - 1; row >= 0; row--){
        cout << "|";
        for(int column = 0; column < this->cnf->boardSize; column++){
            cout << setw(3) << this->cnf->board[row][column] << setw(3) << "|";
        }
        cout << endl;
    }
}

//show original board
void WumpusAgent::showOriBoard(){
    //display board
    int* myPos = this->ww->getMyPos();
    int* curPos = new int[2];

    for(int row = this->ww->getBoardSize() - 1; row >= 0; row--){
        cout << "|";
        for(int column = 0; column < this->ww->getBoardSize(); column++){
            curPos[0] = row;
            curPos[1] = column;
            if(myPos[0] == row && myPos[1] == column){
                if(this->ww->checkPit(curPos))
                    cout << setw(3) << "*/P" << setw(3) << "|";
                else if(this->ww->checkWumpus(curPos))
                    cout << setw(3) << "*/W" << setw(3) << "|";
                else if(this->ww->checkGold(curPos))
                    cout << setw(3) << "*/G" << setw(3) << "|";
                else
                    cout << setw(3) << "*" << setw(3) << "|";
            }
            else if(this->ww->checkPit(curPos)){
                cout << setw(3) << "P" << setw(3) << "|";
            }
            else if(this->ww->checkWumpus(curPos)){
                cout << setw(3) << "W" << setw(3) << "|";
            }
            else if(this->ww->checkGold(curPos)){
                cout << setw(3) << "G" << setw(3) << "|";
            }
            else{
                cout << setw(3) << " " << setw(3) << "|";
            }
        }
        cout << endl;
    }
}

//play wumpus
void WumpusAgent::playWumpus(){
    bool end = false;
    bool* status = NULL;
    int* myPos = new int[2];        //my position
    int pitNum = 0;                 //number of pit status
    int wumpusNum = 0;              //number of wumpus status
    int visitNum = 0;               //number of visit status
    int safeNum = 0;                //number of safe status 
    int mayWumNum = 0;              //number of may have wumpus
    int mayPitNum = 0;              //number of may have pits
    int step = 0;
    string* visitedLocate = new string[this->cnf->boardSize * this->cnf->boardSize];    //places has been visited
    string* pit = new string[this->cnf->boardSize * this->cnf->boardSize];     //status of pit
    string* wumpus = new string[this->cnf->boardSize * this->cnf->boardSize];  //status of wumpus
    string* mayWumpus = new string[this->cnf->boardSize * this->cnf->boardSize];    //status of may have wumpus
    string* mayPits = new string[this->cnf->boardSize * this->cnf->boardSize];      //status of may have pits
    string* OK = new string[this->cnf->boardSize * this->cnf->boardSize];    //status of sage
    string arrow = "";

    cout << ">>>>>>>>>>>>>>>game start<<<<<<<<<<<<<<<<<" << endl;

    //game not end
    while(!end){
        step += 1;
        cout << "---------in step " << step << "---------" << endl;

        //get current status
        status = this->ww->getStatus();

        //update map
        this->updateMap(status);
        
        this->showBoard();
        int wait = 0;
        cout << endl;
        this->showOriBoard();

        cout << endl;

        //scan the map
        for(int row = 0; row < this->cnf->boardSize; row++){
            for(int column = 0; column < this->cnf->boardSize; column++){
                if(this->cnf->board[row][column] == " "){
                    continue;
                }
                else if(this->cnf->board[row][column] == "*"){
                    myPos[0] = row;
                    myPos[1] = column;
                }
                else if(this->cnf->board[row][column] == "P"){
                    pit[pitNum++] = "P" + to_string(row) + "," + to_string(column);
                }
                else if(this->cnf->board[row][column] == "W"){
                    wumpus[wumpusNum++] = "W" + to_string(row) + "," + to_string(column);
                }
                else if(this->cnf->board[row][column] == "X"){
                    visitedLocate[visitNum++] = "V" + to_string(row) + "," + to_string(column);
                }
                else if(this->cnf->board[row][column] == "S"){
                    OK[safeNum++] = "OK" + to_string(row) + "," + to_string(column);                    
                }
                else if(this->cnf->board[row][column] == "!W"){
                    mayWumpus[mayWumNum++] = "MW" + to_string(row) + "," + to_string(column); 
                }
                else if(this->cnf->board[row][column] == "!P"){
                    mayPits[mayPitNum++] = "MP" + to_string(row) + "," + to_string(column);
                }
            }
        }
        if(this->cnf->arrow){
            arrow = "arrow";
        }
        else{
            arrow = "-arrow";
        }

        //check whether current gold
        if(this->ww->checkGold(myPos)){
            this->ww->grab();
            end = this->ww->checkEnd();
            if(end)
                break;
        }

        //check safe unvisited position
        int* pos = NULL;
        if(safeNum > 0){
            int** OKRoutes = new int*[safeNum];
            for(int routeId = 0; routeId < safeNum; routeId++){
                OKRoutes[routeId] = new int[2];
                OKRoutes[routeId][0] = stoi(OK[routeId].substr(2, OK[routeId].find(",") - 1));
                OKRoutes[routeId][1] = stoi(OK[routeId].substr(OK[routeId].find(",") + 1));
            }
            pos = this->planRoute(OKRoutes, myPos, safeNum);
            
            for(int routeId = 0; routeId < safeNum; routeId++){
                delete[] OKRoutes[routeId];
                OKRoutes[routeId] = NULL;
            }
            delete[] OKRoutes;
            OKRoutes = NULL;
        }

        //try to kill a wumpus
        if(pos == NULL){
            string check = "arrow";
            if(ask(&arrow, &check, 1, 1)[0]){
                if(mayWumNum != 0){
                    int** mwRoutes = new int*[mayWumNum];
                    for(int mwId = 0; mwId < wumpusNum; mwId++){
                        mwRoutes[mwId] = new int[2];
                        mwRoutes[mwId][0] = stoi(mayWumpus[mwId].substr(2, mayWumpus[mwId].find(",") - 2));
                        mwRoutes[mwId][1] = stoi(mayWumpus[mwId].substr(mayWumpus[mwId].find(",") + 1));
                    }
                    pos = this->planRoute(mwRoutes, myPos, mayWumNum);
                    for(int mwId = 0; mwId < mayWumNum; mwId++){
                        delete[] mwRoutes[mwId];
                        mwRoutes[mwId] = NULL;
                    }
                    delete[] mwRoutes;
                    mwRoutes = NULL;
                    arrow = this->shoot(arrow, "shoot");
                    this->cnf->board[pos[0]][pos[1]] = "X";
                }
                else if(wumpusNum != 0){
                    int** wRoutes = new int*[wumpusNum];
                    for(int wId = 0; wId < wumpusNum; wId++){
                        wRoutes[wId] = new int[2];
                        wRoutes[wId][0] = stoi(wumpus[wId].substr(1, wumpus[wId].find(",") - 1));
                        wRoutes[wId][1] = stoi(wumpus[wId].substr(wumpus[wId].find(",") + 1));
                    }
                    pos = this->planRoute(wRoutes, myPos, wumpusNum);
                    for(int wId = 0; wId < wumpusNum; wId++){
                        delete[] wRoutes[wId];
                        wRoutes[wId] = NULL;
                    }
                    delete[] wRoutes;
                    wRoutes = NULL;
                    arrow = this->shoot(arrow, "shoot");
                    this->cnf->board[pos[0]][pos[1]] = "X";
                }
            }
        }

        //take a risk
        if(pos == NULL){
            //may have wumpus
            if(mayWumNum != 0){
                int** mwRoutes = new int*[mayWumNum];
                for(int mwId = 0; mwId < mayWumNum; mwId++){
                    mwRoutes[mwId] = new int[2];
                    mwRoutes[mwId][0] = stoi(mayWumpus[mwId].substr(2, mayWumpus[mwId].find(",") - 2));
                    mwRoutes[mwId][1] = stoi(mayWumpus[mwId].substr(mayWumpus[mwId].find(",") + 1));
                }
                pos = this->planRoute(mwRoutes, myPos, mayWumNum);

                for(int mwId = 0; mwId < mayWumNum; mwId++){
                    delete[] mwRoutes[mwId];
                    mwRoutes[mwId] = NULL;
                }
                delete[] mwRoutes;
                mwRoutes = NULL;
            }
            else if(mayPitNum != 0){
                int** mpRoutes = new int*[mayPitNum];
                for(int mpId = 0; mpId < mayPitNum; mpId++){
                    mpRoutes[mpId] = new int[2];
                    mpRoutes[mpId][0] = stoi(mayPits[mpId].substr(2, mayPits[mpId].find(",") - 2));
                    mpRoutes[mpId][1] = stoi(mayPits[mpId].substr(mayPits[mpId].find(",") + 1));
                }

                pos = this->planRoute(mpRoutes, myPos, mayPitNum);
                for(int mpId = 0; mpId < mayPitNum; mpId++){
                    delete[] mpRoutes[mpId];
                    mpRoutes[mpId] = NULL;
                }
                delete[] mpRoutes;
                mpRoutes = NULL;
            }
        }

        //have search the entire map
        if(pos == NULL){
            end = true;
        }

        //delete
        safeNum = 0;
        pitNum = 0;
        wumpusNum = 0;
        visitNum = 0;
        mayWumNum = 0;
        mayPitNum = 0;

        delete[] status;
        status = NULL;

        //do the move
        if(pos != NULL){
            this->ww->setMove(pos);
            this->ww->shoot();
            end = this->ww->checkEnd();
            this->cnf->board[myPos[0]][myPos[1]] = "X";
            myPos[0] = pos[0];
            myPos[1] = pos[1];
            this->cnf->board[myPos[0]][myPos[1]]= "*";
            delete[] pos;
            pos = NULL;
        }
    }

    cout << "--------------Game Over--------------" << endl;
    //final show map
    if(this->ww->noGold()){
        cout << "YOU WIN BY GETTING A GOLD" << endl;
    }
    else{
        cout << "YOU LOSS" << endl;
        if(this->ww->checkPit(myPos)){
            cout << "FAILLING INTO PIT" << endl;
        }
        else{
            cout << "KILLING BY WUMPUS" << endl;
        }
        cout << endl;
    }
    cout << endl;
    this->showBoard();

    delete[] visitedLocate;
    delete[] pit;
    delete[] wumpus;
    delete[] mayWumpus;
    delete[] mayPits;
    delete[] OK;
    delete[] myPos;
}
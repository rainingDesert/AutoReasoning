#ifndef WUMPUS_H
#define WUMPUS_H
#include <string>
using namespace std;

class WumpusWorld{
private:
    int boardSize;
    int pitNum;
    int wumpusNum;
    int goldNum;
    int statusNum;
    int** pitPos;
    int** wumpusPos;
    int** goldPos;
    int* myPos;
    bool arrow;
    string* statusName;
    bool checkStench(int*);
    bool checkBreeze(int*);
    bool checkGlitter(int*);
    bool checkBump(int*);
    bool checkScream(int*);
    void doMove(char);
    void doShoot();
    void createWorld();
public:
    WumpusWorld(int, int, int, int);
    int** getPit();
    int** getWumpus();
    int** getGold();
    int* getMyPos();
    int getBoardSize();
    int getStatusNum();
    string* getStatusName();
    bool checkPit(int*);
    bool checkGold(int*);
    bool checkWumpus(int*);
    bool* move(char);
    bool* getStatus();
    bool grab();
    bool shoot();
    bool checkEnd();
    bool noGold();
    void setMove(int*);
};

class WumpusUI{
public:
    WumpusUI();
    void displayBoard(WumpusWorld*);
    void finalDisplayBoard(WumpusWorld*);
    void displayStatus(WumpusWorld*, bool*);
    void gameStart();
    char getMove();
    bool monsterMove(WumpusWorld*);
    bool goldMove(WumpusWorld*);
    void gameOver(char);
};

//user plays the game
class WumpusImp{
private:
    WumpusWorld* ww;
    WumpusUI* wui;
public:
    WumpusImp(int, int, int, int);
    void gamePlay();
};

//computer plays the game
class WumpusAgent{
private:
    struct kbCNF;

    kbCNF* cnf;

    void helpTrans(string*, string*, string*, string, int, int*);

    string* move(int*, string);    //change location
    string* changeStatus(int*, string*, string*, string*, int, int, int);     //change status
    string* checkOK(string*, string*, int, int);     //check OK or not
    string shoot(string, string);       //change certain and arrow
    void updateMap(bool*);          //update current map

    WumpusWorld* ww;

    bool* ask(string*, string*, int, int);
    void showBoard();
    void showOriBoard();
public:
    WumpusAgent(int, int, int, int);
    void playWumpus();
    int* planRoute(int**, int*, int);
};

#endif
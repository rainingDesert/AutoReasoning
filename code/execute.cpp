#include "KB.h"
#include "algorithm.h"
#include <iostream>
#include <string>
using namespace std;

struct Cla{
    int* symbol;    //symbols of current clause
    bool* wheNot;   //whether not operator exists
    int symNum;     //number of symbols of current clause
    Cla(int* symbol, int symNum, bool* wheNot){
        this->symbol = new int[symNum];
        this->wheNot = new bool[symNum];
        for(int symId = 0; symId < symNum; symId ++){
            this->symbol[symId] = symbol[symId];
            this->wheNot[symId] = wheNot[symId];
        }
        this->symNum = symNum;
    }
    ~Cla(){
        if(this->symbol != NULL){
            delete[] this->symbol;
            this->symbol = NULL;
        }
        if(this->wheNot != NULL){
            delete[] this->wheNot;
            this->wheNot = NULL;
        }
    }
};

int main(){
    KBG* kb1 = new WumpusWorld();
    TTE* tte = new TTE(kb1);
    bool result = tte->execution();
    cout << "final result is: " << result << endl;
    delete tte;

    string clas[10] = {"-P(1,1)", "-B(1,1)", "B(2,1)", "-B(1,1)VP(1,2)VP(2,1)", "-P(1,2)VB(1,1)", "-P(2,1)VB(1,1)", "-B(2,1)VP(1,1)VP(2,2)VP(3,1)", "-P(1,1)VB(2,1)", "-P(2,2)VB(2,1)", "-P(3,1)VP(2,1)"};
    string res[1] = {"P(1,2)"};
    // KBG* cla = new Clause(clas, res, 10, 1);
    // cla->showClas();
    // cla->PLResolve(cla->getCla(0), cla->getCla(7));
    // cla->PLResolve(cla->getCla(3), cla->getCla(4));

    // string clas[2] = {"P", "-PVQ"};
    // string res[1] = {"Q"};
    KBG* kb = new Clause(clas, res, 10, 1);
    tte = new TTE(kb);
    result = tte->execution();
    cout << "final result is: " << result << endl;
    delete tte;
}
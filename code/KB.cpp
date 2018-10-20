#include "KB.h"
#include <iostream>
#include <iomanip>
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

KBG::KBG(){}

KBG::KBG(int len){  //update length
    this->num = len;
}

bool KBG::conPLTrue(bool* boolArray){  //should be implement in children class
    cerr << "you should be implemented" << endl;
    return false;
};

bool KBG::resPLTrue(bool* boolArray){  //should be implement in children class
    cerr << "you should be implemented" << endl;
    return false;
};

Cla* KBG::PLResolve(Cla* cla1, Cla* cla2){  //should be implement in children class
    cerr << "you should be implemented" << endl;
    return NULL;
}

int KBG::getNum(){
    return this->num;
}

Cla* KBG::getCla(int claId){    //should be implement in children class
    cerr << "you should be implemented" << endl;
    return NULL;
}

//////////////// use for test
void KBG::showClas(){}
void KBG::showSymbols(){}

/////////////////////////--------------Clause---------------///////////////////////////////////
void Clause::showClas(){
    cout << "condition" << endl;
    for(int claId = 0; claId < this->claNum; claId++){
        for(int symId = 0; symId < this->clas[claId]->symNum; symId++){
            cout << this->clas[claId]->wheNot[symId] << this->symbols[this->clas[claId]->symbol[symId]] << " ";
        }
        cout << endl;
    }

    cout << "result" << endl;
    for(int claId = 0; claId < this->resNum; claId++){
        for(int symId = 0; symId < this->res[claId]->symNum; symId++){
            cout << this->res[claId]->wheNot[symId] << this->symbols[this->res[claId]->symbol[symId]] << " ";
        }
        cout << endl;
    }
}

Clause::Clause(string* clauses, string* res, int claNum, int resNum): KBG(){
    this->claNum = claNum;
    this->resNum = resNum;
    int num = this->breakSymbol(clauses, res);
    this->num = num;
} 

//break clause and initial this->symbol, this->cals, this->res
int Clause::breakSymbol(string* clauses, string* res){

    bool* wheNot = new bool[100];     //whether not exists
    int* claSyms = new int[100];      //symbols of each clause
    this->symbols = new string[1000];   //symbols
    this->clas = new Cla*[this->claNum];  //clauses
    this->res = new Cla*[this->resNum];     //result clauses
    int symClaNum = 0;
    int symNum = 0;
    int start = 0;
    int charLen = 0;

    //calculate symbols for clauses
    int round = 0;
    int limitNum = 0;
    string* tempClause = NULL;
    while(round < 2){
        if(round == 0){     //save condition
            tempClause = clauses;
            limitNum = this->claNum;
            if(tempClause == NULL){
                cerr << "clauses can not be null" << endl;
                return -1;
            }
        }
        else if(round == 1){        //save result
            tempClause = res;
            limitNum = this->resNum;
            if(tempClause == NULL){
                break;
            }
        }
        for(int claId = 0; claId < limitNum; claId++){
            start = 0;
            symClaNum = 0;
            wheNot[symClaNum] = true;
            charLen = tempClause[claId].size();
            for(int charId = 0; charId < charLen + 1; charId++){
                if(charId == 0 && tempClause[claId][charId] == '-'){    //check whether a not in the first symbol
                    start += 1;
                    wheNot[symClaNum] = false;
                }
                if(charId == charLen || tempClause[claId][charId] == 'V' || tempClause[claId][charId] == 'v'){      //end of current symbol
                    string curStr = tempClause[claId].substr(start, charId - start);

                    //update symbol list
                    int symId = 0;
                    for(symId = 0; symId < symNum; symId++){
                        if(this->symbols[symId] == curStr){     //symbol already exists
                            break;
                        }
                    }
                    if(symId == symNum){
                        this->symbols[symNum++] = curStr;
                    }
                    start = charId + 1;

                    //update clause list
                    claSyms[symClaNum++] = symId;

                    // cout << symClaNum-1 << " " << wheNot[symClaNum-1] << " " << this->symbols[claSyms[symClaNum]] << endl;

                    if(charId != charLen){
                        if(tempClause[claId][charId + 1] == '-'){
                            wheNot[symClaNum] = false;
                            start += 1;
                        }
                        else
                            wheNot[symClaNum] = true;
                    }
                }
            }

            //update clas
            if(round == 0)
                this->clas[claId] = new Cla(claSyms, symClaNum, wheNot);
            else
                this->res[claId] = new Cla(claSyms, symClaNum, wheNot);
        }
        round++;
    }

    return symNum;
}

//do the judgement
bool Clause::getBoolResult(bool* boolArray, Cla** clas, int num){
    bool judge = false;

    for(int claId = 0; claId < num; claId++){
        judge = false;
        for(int symId = 0; symId < clas[claId]->symNum; symId ++){
            if(int(clas[claId]->wheNot[symId]) + int(boolArray[clas[claId]->symbol[symId]]) != 1){
                judge = true;
                break;
            }
        }
        if(!judge){
            return false;
        }
    }
    return true;
}

//show the judgement with condition
bool Clause::conPLTrue(bool* boolArray){
    return this->getBoolResult(boolArray, this->clas, this->claNum);
}

//show the judgement with result
bool Clause::resPLTrue(bool* boolArray){
    return this->getBoolResult(boolArray, this->res, this->resNum);
}

//resolve PL
Cla* Clause::PLResolve(Cla* cla1, Cla* cla2){
    int* symbol = new int[cla1->symNum + cla2->symNum];
    int* cla2Symbol = new int[cla2->symNum];
    bool* wheNot = new bool[cla1->symNum + cla2->symNum];
    bool del = false;
    int symClaNum = 0;

    //copy symbols of clause 2
    cla2Symbol = new int[cla2->symNum];
    for(int cla2Id = 0; cla2Id < cla2->symNum; cla2Id++){
        cla2Symbol[cla2Id] = cla2->symbol[cla2Id];
    }

    //do resolution
    for(int cla1Id = 0; cla1Id < cla1->symNum; cla1Id++){
        bool combine = false;
        for(int cla2Id = 0; cla2Id < cla2->symNum; cla2Id++){
            if(!del && (cla1->symbol[cla1Id] == cla2->symbol[cla2Id])){
                combine = true;
                cla2Symbol[cla2Id] = -1;

                if(int(cla1->wheNot[cla1Id]) + int(cla2->wheNot[cla2Id]) != 1){ //can not del
                    symbol[symClaNum] = cla1->symbol[cla1Id];
                    wheNot[symClaNum++] = cla1->wheNot[cla1Id];
                }
                else{
                    del = true;
                }
                break;
            }
        }

        if(!combine){
            symbol[symClaNum] = cla1->symbol[cla1Id];
            wheNot[symClaNum++] = cla1->wheNot[cla1Id];
        }
    }

    for(int cla2Id = 0; cla2Id < cla2->symNum; cla2Id++){
        if(cla2Symbol[cla2Id] != -1){
            symbol[symClaNum] = cla2->symbol[cla2Id];
            wheNot[symClaNum++] = cla2->wheNot[cla2Id];
        }
    }

    Cla* newCla = new Cla(symbol, symClaNum, wheNot);

    cout << "new clause" << endl;
    for(int symId = 0; symId < newCla->symNum; symId++){
        cout << newCla->wheNot[symId] << this->symbols[newCla->symbol[symId]] << " ";
    }
    cout << endl;

    return newCla;
}

//get clause
Cla* Clause::getCla(int claId){
    return this->clas[claId];
}

/////////////////////////////////////////////
void Clause::showSymbols(){
    for(int i = 0; i < this->num; i++){
        cout << setw(7) << this->symbols[i] << " ";
    }
    cout << endl;
}

//constructor for Modus Ponens
ModusPonens::ModusPonens() : KBG(2){
    this->limitLen = 2;
};

//show the judgement with Modus Ponens' condition
//boolArray[0] -> P, boolArray[1] -> Q
bool ModusPonens::conPLTrue(bool* boolArray){
    if(this->num != this->limitLen){
        cerr << "wrong length" << endl;
        return false;
    }

    if(boolArray[0] && boolArray[1])    
        return true;
    return false;
};

//show the judgement with Modus Ponens' result
bool ModusPonens::resPLTrue(bool* boolArray){
    return boolArray[1];
}

//constructor for Wumpus World
WumpusWorld::WumpusWorld() : KBG(7){
    this->limitLen = 7;
};

//show the judgement with Wumpus World's condition
//boolArray[0] -> P(1,1), boolArray[1] -> P(1,2), boolArray[2] -> P(2,1), boolArray[3] -> P(2,2), boolArray[4] -> P(3,1)
//boolArray[5] -> B(1,1), boolArray[6] -> B(2,1)
bool WumpusWorld::conPLTrue(bool* boolArray){
    if(this->num != this->limitLen){
        cerr << "wrong length" << endl;
        return false;
    }

    if(boolArray[0])       //not P(1,1)   
        return false;
    if(boolArray[5])        //not B(1,1)
        return false;
    if(!boolArray[6])       //B(2,1)
        return false;

    if(boolArray[1] || boolArray[2])    //B(1,1) <-> (P(1,2) V P(2,1)) with B(1,1) must be false
        return false;
    if(!(boolArray[3] || boolArray[4]))     //B(2,1) <-> (P(1,1) V P(2,2) V P(3,1)) with B(2,1) must be true, P(1,1) must be false
        return false;

    return true;
};

//show the judgement with Wumpus World's result
bool WumpusWorld::resPLTrue(bool* boolArray){
    return boolArray[3];
}
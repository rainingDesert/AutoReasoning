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

int KBG::getNum(){
    return this->num;
}

/////////////////////////--------------Clause---------------///////////////////////////////////
Clause::Clause(string* clauses, int claNum, string* res = NULL, int resNum = 0): KBG(){
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

    //format array
    string* tempSymbols = new string[symNum];
    for(int symId = 0; symId < symNum; symId++){
        tempSymbols[symId] = this->symbols[symId];
    }
    delete[] this->symbols;
    this->symbols = tempSymbols;

    delete[] wheNot;
    wheNot = NULL;
    delete[] claSyms;
    claSyms = NULL;

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

///////////////////////////////////////--------------ClaForDPLL-----------------////////////////////////////////
//constructor of ClaForDPLL
ClaForDPLL::ClaForDPLL(string* clauses, int claNum): Clause(clauses, claNum){}

//check the value of a clause
char ClaForDPLL::checkClause(Cla* clause, char* model){
    char claJud = '-';
    bool falCheck = true;

    //inside a clause
    for(int symId = 0; symId < clause->symNum; symId++){ 
        char modSym = model[clause->symbol[symId]];
        if(modSym != '-'){      //model include such symbol
            if(modSym - '0' + int(clause->wheNot[symId]) != 1){
                claJud = '1';
                break;
            }
        }
        else{   //no symbol inside model
            falCheck = false;
        }
    }

    //check final result
    if(claJud == '-' && falCheck){
        claJud = '0';
    }
    return claJud;
}

//check CNF
char ClaForDPLL::CNFCheck(char* model){
    char claJud = '-';
    bool falCheck = true;  //whether clause false can be possible
    bool trueCheck = true;  //whether final true can be possible

    for(int claId = 0; claId < this->claNum; claId++){  //check each clause
        claJud = '-';
        falCheck = true;

        //inside a clause
        claJud = this->checkClause(this->clas[claId], model);

        if(claJud == '0'){
            return '0';
        }
        else if(claJud == '-'){
            trueCheck = false;
        }
    }

    //final result
    if(trueCheck){
        return '1';
    }
    else{
        return '-';
    }
}

//find pure symbol
int* ClaForDPLL::FindPureSymbol(bool* symbols, char* model){
    int* result = NULL;
    char* wheNot = new char[this->num];
    bool* same = new bool[this->num];
    for(int wId = 0; wId < this->num; wId++){
        wheNot[wId] = '-';
        same[wId] = true;
    }

    //check each symbol in clause
    for(int claId = 0; claId < this->claNum; claId++){
        //check whether current clause has a truth value
        if(this->checkClause(this->clas[claId], model) != '-')
            continue;

        //inside a clause
        for(int symId = 0; symId < this->clas[claId]->symNum; symId++){
            if(wheNot[this->clas[claId]->symbol[symId]] == '-'){
                wheNot[this->clas[claId]->symbol[symId]] = (char)('0' + (int)this->clas[claId]->wheNot[symId]);
            }
            else if(wheNot[this->clas[claId]->symbol[symId]] - '0' + int(this->clas[claId]->wheNot[symId]) == 1){
                same[this->clas[claId]->symbol[symId]] = false;
            }
        }
    }

    //get a pure symbol
    for(int symId = 0; symId < this->num; symId++){
        if(!symbols[symId])
            continue;
        if(!same[symId])
            continue;
        result = new int[2];
        result[0] = symId;
        if(wheNot[symId] == '-')    //value of current symbol does not matter
            result[1] = '0';
        else
            result[1] = wheNot[symId] - '0';    
        break;
    }

    delete[] wheNot;
    wheNot = NULL;
    delete[] same;
    same = NULL;

    return result;
}

//find unit symbol
int* ClaForDPLL::FindUnitSymbol(char* model){
    int* result = new int[2];
    int symAvNum = 0;

    //check each clause
    for(int claId = 0; claId < this->claNum; claId++){
        if(checkClause(this->clas[claId], model) != '0')
            continue;
        symAvNum = 0;
        for(int symId = 0; symId < this->clas[claId]->symNum; symId++){
            if(model[this->clas[claId]->symbol[symId]]== '-'){
                symAvNum += 1;
                result[0] = symId;
                result[1] = int(this->clas[claId]->wheNot[symId]);
            }
        }
        //check whether exist unit symbol
        if(symAvNum == 1){
            return result;
        }
    }
    delete[] result;
    return NULL;
}

//resolve PL
Cla* ClaForDPLL::PLResolve(Cla* cla1, Cla* cla2){
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

    return newCla;
}

#include "algorithm.h"
#include "KB.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
using namespace std;

//constructor for algorithm factory
Alg::Alg(KBG* kb){
    this->kb = kb;
}

//initial constructor
Alg::Alg(){
    this->kb = NULL;
}

//destructor for algorithm factory
Alg::~Alg(){
    if(this->kb != NULL){
        delete this->kb;
        this->kb = NULL;
    }
}

//execution function
bool Alg::execution(){
    cerr << "you should implement this" << endl;
    return false;
}

/////////////////////////////////////---------------TTE--------------------///////////////////////////////////

//constructor for Truth Table Entailment
TTE::TTE(KBG* kb) : Alg(kb){}

//destructor for Truth Table Entailment
TTE::~TTE(){}

//do TT-Entails
bool TTE::TTEntails(){
    int num = kb->getNum();
    int symbolNum = num;    //number of symbol
    bool* model = new bool[num];   //list of model
    bool result = this->TTCheckAll(model, symbolNum, num);     //do Truth Table Entailment to get result.

    //delete symbols
    if(model != NULL){
        delete[] model;
        model = NULL;
    }

    return result;
}

//implement Truth Table Entailment
bool TTE::TTCheckAll(bool* model, int symbolNum, int totNum){
    if(symbolNum == 0){     //all symbols have been added to model
        if(this->kb->conPLTrue(model))
            return this->kb->resPLTrue(model);
        else    //if condition false, result can be anything
            return true;
    }

    //add symbol into model
    model[totNum - symbolNum] = true;
    if(this->TTCheckAll(model, symbolNum - 1, totNum)){
        model[totNum - symbolNum] = false;
        return this->TTCheckAll(model, symbolNum - 1, totNum);
    }
    return false;
}

//execution function overload
bool TTE::execution(){
     bool result = this->TTEntails();
     return result;
}

/////////////////////////////////////---------------DPLL--------------------///////////////////////////////////
//constructor of DPLL
DPLL::DPLL(KBG* cfd): Alg(){
    this->cfd = (ClaForDPLL*)cfd;
}

//destructor of DPLL
DPLL::~DPLL(){
    if(this->cfd != NULL){
        delete this->cfd;
        this->cfd = NULL;
    }
}

//DPLL statisfied
bool DPLL::DPLLSatisfied(){
    char* model = new char[this->cfd->getNum()];
    bool* symbols = new bool[this->cfd->getNum()];
    for(int symId = 0; symId < this->cfd->getNum(); symId++){
        model[symId] = '-';
        symbols[symId] = true;
    }
    return this->DPLLImp(symbols, model);
}

//do DPLL
bool DPLL::DPLLImp(bool* symbols, char* model){
    int* pure = NULL;
    int* unit = NULL;
    bool* newSymbols = NULL;
    char* newModel = NULL;

    //check whether reach result
    if(this->cfd->CNFCheck(model) == '1'){
        return true;
    }
    if(this->cfd->CNFCheck(model) == '0'){
        return false;
    }
    
    newSymbols = new bool[this->cfd->getNum()];
    newModel = new char[this->cfd->getNum()];
    for(int symId = 0; symId < this->cfd->getNum(); symId++){
        newSymbols[symId] = symbols[symId];
        newModel[symId] = model[symId];
    }

    //get pure symbol
    pure = this->cfd->FindPureSymbol(newSymbols, newModel);
    if(pure != NULL){
        newSymbols[pure[0]] = false;
        newModel[pure[0]] = (char)(pure[1] + '0');
        bool result = DPLLImp(newSymbols, newModel);

        delete[] newSymbols;
        newSymbols = NULL;
        delete[] newModel;
        newModel = NULL;
        delete[] pure;
        pure = NULL;
        return result;
    }

    //get unit symbol
    pure = this->cfd->FindUnitSymbol(newModel);
    if(pure != NULL){
        newSymbols[pure[0]] = false;
        newModel[pure[0]] = (char)(pure[1] + '0');
        bool result = DPLLImp(newSymbols, newModel);

        delete[] newSymbols;
        newSymbols = NULL;
        delete[] newModel;
        newModel = NULL;
        delete[] pure;
        pure = NULL;
        return result;
    }

    //normal truth table
    for(int symId = 0; symId < this->cfd->getNum(); symId++){
        if(newSymbols[symId]){
            newSymbols[symId] = false;
            newModel[symId] = '1';
            bool result = DPLLImp(newSymbols, newModel);
            if(!result){
                newModel[symId] = '0';
                result = DPLLImp(newSymbols, newModel);
            }
            
            delete[] newSymbols;
            newSymbols = NULL;
            delete[] newModel;
            newModel = NULL;
            return result;
        }
    }

    //would not be here
    cerr << "something wrong, you should not be here" << endl;
    return false;
}

//execution overload
bool DPLL::execution(){
    bool result = this->DPLLSatisfied();
    return result;
}
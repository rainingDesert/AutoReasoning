#include "algorithm.h"
#include "KB.h"
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

//constructor for algorithm factory
Alg::Alg(KBG* kb){
    this->kb = kb;
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

//constructor for Truth Table Entailment
TTE::TTE(KBG* kb) : Alg(kb){
    this->temp = new bool*[int(pow(2, kb->getNum()))];
    for(int i = 0; i < int(pow(2, kb->getNum())); i++){
        this->temp[i] = new bool[kb->getNum() + 2];
    }
    this->top = 0;
}

//destructor for Truth Table Entailment
TTE::~TTE(){
    if(this->temp != NULL){
        for(int i = 0; i < int(pow(2, this->kb->getNum())); i++){
            if(this->temp[i] != NULL){
                delete[] this->temp[i];
                this->temp[i] = NULL;
            }
        }
        delete[] this->temp;
        this->temp = NULL;
    }
}

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
        ///////////
        for(int i = 0; i < this->kb->getNum(); i++){
            this->temp[this->top][i] = model[i];
        }
        this->temp[this->top][this->kb->getNum()] = this->kb->conPLTrue(model);
        this->temp[this->top][this->kb->getNum() + 1] = this->kb->resPLTrue(model);
        this->top ++;
        ///////////
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
     cout << this->top << endl;
    //////////////////
    cout << setw(7) << "P(1,1)" << " " << setw(7) << "P(1,2)" << " " << setw(7) << "P(2,1)" << " " << setw(7)  << "P(2,2)" << " ";
    cout << setw(7) << "P(3,1)" << " " << setw(7) << "B(1,1)" << " " << setw(7) << "B(2,1)" << " " << setw(7) << "con" << " ";
    cout << setw(7) << "res" << " " << endl;
    // cout << setw(7) << "P" << " " << setw(7) << "Q" << " " << setw(7) << "con" << " " << setw(7) << "res" << " " << endl; 
    this->kb->showSymbols();
    for(int j = 0; j < this->top; j++){
        for(int i = 0; i < this->kb->getNum() + 2; i++){
            cout << setw(7) << this->temp[j][i] << " ";
        }
        cout << endl;
    }
    //////////////////
     return result;
}
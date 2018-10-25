#include "KB.h"
#include "algorithm.h"
#include "execute.h"
#include "wumpus.h"
#include <iostream>
#include <string>
#include "time.h"
using namespace std;

/////////////////////////////////////---------------Library of Knowledge Base----------------///////////////////////////////////
struct LibraryOfKB::CNF{
    string* clas;
    string* res;
    int clasNum;
    int resNum;
    CNF(string* clas, string* res, int clasNum, int resNum){
        this->clas = new string[clasNum];
        this->res = new string[resNum];
        for(int i = 0; i < clasNum; i++){
            this->clas[i] = clas[i];
        }
        for(int i = 0; i < resNum; i++){
            this->res[i] = res[i];
        }
        this->clasNum = clasNum;
        this->resNum = resNum;
    }
    ~CNF(){
        if(this->clas != NULL){
            delete[] this->clas;
            this->clas = NULL;
        }
        if(this->res != NULL){
            delete[] this->res;
            this->res = NULL;
        }
    }
};

///////////////////////////-------Execute with TTE and DPLL----------//////////////////////
//do execution
void doExec(string* clas, string* res, int clasNum, int resNum){
    cout << "-----------------------TTE------------------------" << endl;
    clock_t t;
    double sumTime = 0;
    Exec* e = NULL;
    string* temp = new string[1];
    bool trueCheck = false;
    bool falseCheck = false;

    
    for(int i = 0; i < resNum; i++){
        // check true
        t = clock();
        temp[0] = res[i];
        if(e != NULL){
            delete e;
            e = NULL;
        }

        e = new TTEExec(clas, temp, clasNum, 1);
        trueCheck = e->execution();

        //check false
        temp[0] = "-" + res[i];
        delete e;
        e = new TTEExec(clas, temp, clasNum, 1);
        falseCheck = e->execution();

        delete e;
        e = NULL;

        t = clock() - t;
        sumTime += (double)t;

        if((int)trueCheck + (int)falseCheck == 1){
            cout << res[i] << " is " << trueCheck << endl;
            // cout << "As " << res[i] << " is " << trueCheck << " && -" << res[i] << " is " << falseCheck << endl;
        }
        else{
            cout << res[i] << ": not determine" << endl;
            // cout << "As " << res[i] << " is " << trueCheck << " && -" << res[i] << " is " << falseCheck << endl;
        }
    }
    cout << "Time for running TTE is " << sumTime/CLOCKS_PER_SEC << " seconds" << endl;
    
    cout << endl << "-----------------------DPLL------------------------" << endl;
    sumTime = 0;
    for(int i = 0; i < resNum; i++){
        // check true
        t = clock();
        temp[0] = "-" + res[i];
        if(e != NULL){
            delete e;
            e = NULL;
        }

        e = new DPLLExec(clas, temp, clasNum, 1);
        trueCheck = !e->execution();

        //check false
        temp[0] = res[i];
        delete e;
        e = new DPLLExec(clas, temp, clasNum, 1);
        falseCheck = !e->execution();

        t = clock() - t;
        sumTime += (double)t;

        if((int)trueCheck + (int)falseCheck == 1){
            cout << res[i] << " is " << trueCheck << endl;
            // cout << "As " << res[i] << " is " << trueCheck << " && -" << res[i] << " is " << falseCheck << endl;
        }
        else{
            cout << res[i] << ": not detemine" << endl;
            // cout << "As " << res[i] << " is " << trueCheck << " && -" << res[i] << " is " << falseCheck << endl;
        }
    }
    cout << "Time for running TTE is " << sumTime/CLOCKS_PER_SEC << " seconds" << endl;
}

void exec(){
    LibraryOfKB::CNF* cnf = NULL;

    cout << ">>>>>>>>>>>>>>>>>For Modus Ponens<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::ModusPonens();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For Wumpus World<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::Wumpus();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For Horned<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::Horn();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For LTT1<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::LTT1();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For LTT2<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::LTT2();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For MLTT<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::MLTT();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For TDE1<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::TDE1();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);

    cout << endl << ">>>>>>>>>>>>>>>>>For TDE2<<<<<<<<<<<<<<<<<<<"<< endl;
    cnf = LibraryOfKB::TDE2();
    doExec(cnf->clas, cnf->res, cnf->clasNum, cnf->resNum);  
}

int main(){
    exec();
}
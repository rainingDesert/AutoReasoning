#include "KB.h"
#include "algorithm.h"
#include "execute.h"
#include "wumpus.h"
#include <iostream>
#include <string>
#include "time.h"
using namespace std;

/////////////////////////////////////---------------Exec----------------///////////////////////////////////
//constructor for execution class
Exec::Exec(string* kbCNF, string* resCNF, int kbNum, int resNum){
    this->kbCNF = kbCNF;
    this->resCNF = resCNF;
    this->kbNum = kbNum;
    this->resNum = resNum;
}

//need to be overloaded
bool Exec::execution(){
    cerr << "you should overload this function" << endl;
    return false;
}

/////////////////////////////////////---------------TTEExec----------------///////////////////////////////////
//constructor for TTEExec class
TTEExec::TTEExec(string* kbCNF, string* resCNF, int kbNum, int resNum): Exec(kbCNF, resCNF, kbNum, resNum){}

//execution for tte
bool TTEExec::execution(){
    TTE* tte = new TTE(new Clause(kbCNF, kbNum, resCNF, resNum));
    bool result = tte->execution();
    delete tte;

    return result;
}

/////////////////////////////////////---------------DPLLExec----------------///////////////////////////////////
//constructor for DPLLExec class
DPLLExec::DPLLExec(string* kbCNF, string* notResCNF, int kbNum, int resNum): Exec(kbCNF, notResCNF, kbNum, resNum){}

//execution for DPLL
bool DPLLExec::execution(){
    string* totCNF = new string[this->kbNum + this->resNum];
    for(int ClaId = 0; ClaId < this->kbNum; ClaId++){
        totCNF[ClaId] = this->kbCNF[ClaId];
    }
    for(int ClaId = 0; ClaId < this->resNum; ClaId++){
        totCNF[ClaId + this->kbNum] = this->resCNF[ClaId];
    }
    Alg* dpll = new DPLL(new ClaForDPLL(totCNF, this->kbNum + this->resNum));
    bool result = dpll->execution();

    return result;
}

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

//Modus Ponens
LibraryOfKB::CNF* LibraryOfKB::ModusPonens(){
    string clas[2] = {"P", "-PVQ"};
    string res[1] = {"Q"};
    CNF* newCNF = new CNF(clas, res, 2, 1);
    return newCNF;
}

//simple wumpus world
LibraryOfKB::CNF* LibraryOfKB::Wumpus(){
    string clas[10] = {"-P(1,1)", "-B(1,1)", "B(2,1)", "-B(1,1)VP(1,2)VP(2,1)", "-P(1,2)VB(1,1)", "-P(2,1)VB(1,1)", "-B(2,1)VP(1,1)VP(2,2)VP(3,1)", "-P(1,1)VB(2,1)", "-P(2,2)VB(2,1)", "-P(3,1)VP(2,1)"};
    string res[1] = {"P(1,2)"};
    CNF* newCNF = new CNF(clas, res, 10, 1);
    return newCNF;
}

//horn
LibraryOfKB::CNF* LibraryOfKB::Horn(){
    string clas[6] = {"-Mythical(U)VImmortal(U)", "Mythical(U)V-Immortal(U)", "Mythical(U)Vmammal(U)", "-Immortal(U)VHorned(U)", "-mammal(U)VHorned(U)", "-Horned(U)VMagical(U)"};
    string res[3] = {"Mythical(U)", "Magical(U)", "Horned(U)"};
    CNF* newCNF = new CNF(clas, res, 6, 3);
    return newCNF;
}

//LTT1
LibraryOfKB::CNF* LibraryOfKB::LTT1(){
    string clas[8] = {"-AmyVCal", "-AmyVAmy", "-CalV-AmyVAmy","-BobV-Cal", "CalVBob", "-CalVBobV-Amy", "-BobVCal", "AmyVCal"};
    string res[3] = {"Amy", "Bob", "Cal"};
    CNF* newCNF = new CNF(clas, res, 8, 3);
    return newCNF;
}

//LTT2
LibraryOfKB::CNF* LibraryOfKB::LTT2(){
    string clas[7] = {"-AmyV-Cal", "CalVAmy", "-BobVAmy", "-BobVCal", "-AmyV-CalVBob", "-CalVBob", "-BobVCal"};
    string res[3] = {"Amy", "Bob", "Cal"};
    CNF* newCNF = new CNF(clas, res, 7, 3);
    return newCNF;
}

//MLTT
LibraryOfKB::CNF* LibraryOfKB::MLTT(){
    string clas[36] = {"-AmyVHal", "-AmyVIda", "-HalV-IdaVAmy",\
                      "-BobVAmy", "-BobVLee", "-AmyV-LeeVBob",\
                      "-CalVBob", "-CalVGil", "-BobV-GilVCal",\
                      "-DeeVEli", "-DeeVLee", "-EliV-LeeVDee",\
                      "-EliVCal", "-EliVHal", "-CalV-HalVEli",\
                      "-FayVDee", "-FayVIda", "-DeeV-IdaVFay",\
                      "-GilV-Eli", "-GilV-Jay", "EliVJayVGil",\
                      "-HalV-Fay", "-HalV-Kay", "FayVKayVHal",\
                      "-IdaV-Gil", "-IdaV-Kay", "GilVKayVIda",\
                      "-JayV-Amy", "-JayV-Cal", "AmyVCalVJay",\
                      "-KayV-Dee", "-KayV-Fay", "DeeVFayVKay",\
                      "-LeeV-Bob", "-LeeV-Jay", "BobVJayVLee"};
    string res[12] = {"Amy", "Hal", "Bob", "Lee", "Cal", "Dee", "Eli", "Fay", "Ida", "Gil", "Jay","Kay"};
    CNF* newCNF = new CNF(clas, res, 36, 12);
    return newCNF;
}

//TDE1
LibraryOfKB::CNF* LibraryOfKB::TDE1(){
    string clas[25] = {"-AVX", "-XVA",\
                     "-BVYVZ", "-YVB", "-ZVB",\
                     "-CVA", "-CVB", "-AV-BVC",\
                     "-DVX", "-DVY", "-XV-YVD",\
                     "-EVX", "-EVZ", "-XV-ZVE",\
                     "-FV-EV-D", "-FVEVD", "DV-EVF", "-DVEVF",\
                     "-GV-CVF", "CVG", "-FVG",\
                     "-HV-GVA", "GVH", "H", "-AVH"};
    string res[4] = {"X", "Y", "Z", "W"};
    CNF* newCNF = new CNF(clas, res, 25, 4);
    return newCNF;
}

//TDE2
LibraryOfKB::CNF* LibraryOfKB::TDE2(){
    string clas[14] = {"-AVX", "-XVA",\
                    "-HV-GVA", "GVH", "H", "-AVH",\
                    "-CVA", "-AV-BVC", "-AV-DVC", "-AV-EVC", "-AV-FVC", "-AV-GVC", "-AV-HVC",\
                    "CVG"};
    string res[4] = {"X", "Y", "Z", "W"};
    CNF* newCNF = new CNF(clas, res, 14, 4);
    return newCNF;
}
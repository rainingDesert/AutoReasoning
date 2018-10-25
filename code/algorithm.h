#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "KB.h"

//algorithm factory
class Alg{
protected:
    KBG* kb;
public:
    Alg();
    Alg(KBG*);
    virtual ~Alg();
    virtual bool execution();
};

//Truth Table for Entailment
class TTE : public Alg{
private:
    bool TTEntails();  //do TT-Entails
    bool TTCheckAll(bool*, int, int);  //function that implement TT-Entails
public:
    TTE(KBG*);
    ~TTE();
    bool execution();
};

//DPLL
class DPLL : public Alg{
private:
    ClaForDPLL* cfd;
    bool DPLLSatisfied();   //called by exection
    bool DPLLImp(bool*, char*);     //do DPLL
public:
    DPLL(KBG*);
    ~DPLL();
    bool execution();
};



#endif
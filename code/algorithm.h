#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "KB.h"

//algorithm factory
class Alg{
protected:
    KBG* kb;
public:
    Alg(KBG*);
    ~Alg();
    virtual bool execution();
};

//Truth Table for Entailment
class TTE : Alg{
private:
    bool** temp;     //you should delete this in the end
    int top;
    bool TTEntails();  //do TT-Entails
    bool TTCheckAll(bool*, int, int);  //function that implement TT-Entails
public:
    TTE(KBG*);
    ~TTE();
    bool execution();
};



#endif
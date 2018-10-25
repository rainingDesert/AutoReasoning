#ifndef EXECUTE_H
#define EXECUTE_H

#include <string>
using namespace std;

//do execution
class Exec{
protected:
    string* kbCNF;
    string* resCNF;
    int kbNum;
    int resNum;
public:
    Exec(string*, string*, int, int);
    virtual bool execution();
};

//TTE execution
class TTEExec: public Exec{
public:
    TTEExec(string*, string*, int, int);
    bool execution();
};

//DPLL execution
class DPLLExec: public Exec{
public:
    DPLLExec(string*, string*, int, int);
    bool execution();
};

class LibraryOfKB{
public:
    struct CNF;
    static CNF* ModusPonens();
    static CNF* Wumpus();
    static CNF* Horn();
    static CNF* LTT1();
    static CNF* LTT2();
    static CNF* MLTT();
    static CNF* TDE1();
    static CNF* TDE2();
};

#endif
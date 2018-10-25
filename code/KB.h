#ifndef KB_H
#define KB_H
#include <string>
using namespace std;

struct Cla;

class KBG{
protected:
    int num;
public:
    KBG();
    KBG(int);
    virtual bool conPLTrue(bool*);
    virtual bool resPLTrue(bool*);

    int getNum();
};

//Clause
class Clause: public KBG{
protected:
    Cla** clas;
    Cla** res;
    string* symbols;
    int claNum;
    int resNum;
    int breakSymbol(string*, string*);
    bool getBoolResult(bool*, Cla**, int);
public:
    Clause(string*, int, string*, int);       //input string should be conditions and result
    bool conPLTrue(bool*);
    bool resPLTrue(bool*);
};

//Clause for DPLL
class ClaForDPLL: public Clause{
private:
    char checkClause(Cla*, char*);  //return '-' to be no result, '0' to be false, '1' to be true
public:
    ClaForDPLL(string*, int);
    char CNFCheck(char*);       //check CNF for DPLL    return '-' to be no result, '0' to be false, '1' to be true
    int* FindPureSymbol(bool*, char*);       //find pure symbol
    int* FindUnitSymbol(char*);      //find unit symbol
    static Cla* PLResolve(Cla*, Cla*);
};

#endif
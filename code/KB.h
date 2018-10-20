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
    virtual Cla* PLResolve(Cla*, Cla*);
    virtual Cla* getCla(int);

    //////////////// use for test
    virtual void showClas();
    virtual void showSymbols();

    int getNum();
};

//Clause
class Clause: public KBG{
private:
    Cla** clas;
    Cla** res;
    string* symbols;
    int claNum;
    int resNum;
    int breakSymbol(string*, string*);
    bool getBoolResult(bool*, Cla**, int);
public:
    Clause(string*, string*, int, int);       //input string should be conditions and result
    bool conPLTrue(bool*);
    bool resPLTrue(bool*);
    Cla* PLResolve(Cla*, Cla*);
    Cla* getCla(int);

    //////////////// use for test
    void showClas();
    void showSymbols();
};

//Modus Ponens
class ModusPonens: public KBG{
private:
    int limitLen;
public:
    ModusPonens();
    bool conPLTrue(bool*);
    bool resPLTrue(bool*);
};

//Wumpus World
class WumpusWorld: public KBG{
private:
    int limitLen;
public:
    WumpusWorld();
    bool conPLTrue(bool*);
    bool resPLTrue(bool*);
};

#endif
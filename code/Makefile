generation = algorithm.o KB.o
basicProblems =  $(generation) execute.o exeBasic.o
wumpusProblem = $(generation) wumpus.o
wumpusUser = $(wumpusProblem) exeUserWumpus.o
wumpusComp = $(wumpusProblem) exeCompWumpus.o

$(generation): algorithm.h KB.h
execute.o: execute.h
wumpus.o: wumpus.h

all: runBasic userPlayWumpus compPlayWumpus
.PHONY: all
runBasic: $(basicProblems)
	g++ -o runBasic $(basicProblems)
userPlayWumpus: $(wumpusUser)
	g++ -o userPlayWumpus $(wumpusUser)
compPlayWumpus: $(wumpusComp)
	g++ -o compPlayWumpus $(wumpusComp)

.PHONY: clean
clean:
		-rm runBasic userPlayWumpus compPlayWumpus *.o

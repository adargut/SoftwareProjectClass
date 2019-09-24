CC = gcc
OBJS = main.o CommandExecutioner.o ErrorHandler.o FileHandler.o Game.o HistoryHandler.o MainAux.o Parser.o ParserUtils.o Solver.o SolverUtils.o
EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c MainAux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
CommandExecutioner.o: CommandExecutioner.c HistoryHandler.h MainAux.h FileHandler.h  Solver.h ErrorHandler.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
ErrorHandler.o: ErrorHandler.c Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
FileHandler.o: FileHandler.c Game.h Solver.h MainAux.h ErrorHandler.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Game.o: Game.c MainAux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
HistoryHandler.o: HistoryHandler.c Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MainAux.o: MainAux.c Parser.h Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c ParserUtils.h Game.h CommandExecutioner.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
ParserUtils.o: ParserUtils.c CommandExecutioner.h ErrorHandler.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Solver.o: Solver.c MainAux.h Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
SolverUtils.o: SolverUtils.h Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c

clean:
	rm -f *.o $(EXEC)
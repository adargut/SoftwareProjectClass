cmake_minimum_required(VERSION 3.13)
project(Sudoku C)

set(CMAKE_C_STANDARD 90)
set(GCC_COVERAGE_COMPILE_FLAGS "-fmessage-length=0 -ansi -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


add_executable(Sudoku main.c Game.c Game.h MainAux.c MainAux.h Parser.c Parser.h Solver.c Solver.h FileHandler.h FileHandler.c ErrorHandler.c ErrorHandler.h HistoryHandler.c HistoryHandler.h CommandExecutioner.c CommandExecutioner.h ParserUtils.c ParserUtils.h SolverUtils.c SolverUtils.h gurobi_c.h)


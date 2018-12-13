
cmake_minimum_required(VERSION 3.1)

project(tictactoe-mcts)

set(SRCS
    main.cpp
    Simulator.cpp
    Solver.cpp
    )
    
set(HEADERS
    Simulator.h
    Solver.h
    )

set (CMAKE_CXX_STANDARD 11)

add_executable(tictactoe ${SRCS} ${HEADERS})

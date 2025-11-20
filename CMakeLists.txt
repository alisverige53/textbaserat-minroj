cmake_minimum_required(VERSION 3.10)
project(minroj)

set(CMAKE_CXX_STANDARD 17)

add_executable(minroj
    main.cpp
    MinesweeperGame.cpp
    Board.cpp
)

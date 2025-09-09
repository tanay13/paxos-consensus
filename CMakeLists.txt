cmake_minimum_required(VERSION 3.10)
project(paxos_project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(paxos
    src/main.cpp
    src/node.cpp
    src/acceptor.cpp
    src/proposer.cpp
    src/learner.cpp
)


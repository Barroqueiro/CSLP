cmake_minimum_required(VERSION 2.8)

project( Golomb )
add_executable( test TestGolomb.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)

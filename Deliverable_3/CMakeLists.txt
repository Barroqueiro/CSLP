cmake_minimum_required(VERSION 2.8)

project( Golomb )
add_executable( testEncode TestGolombEncode.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
add_executable( testDecode TestGolombDecode.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)

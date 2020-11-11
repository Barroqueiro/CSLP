cmake_minimum_required(VERSION 2.8)

project( Golomb )
add_executable( testEncode TestGolombEncode.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
add_executable( testDecodeN TestGolombDecodeN.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
add_executable( testDecodeT TestGolombDecodeT.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)

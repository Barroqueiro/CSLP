cmake_minimum_required(VERSION 2.8)

project( Golomb )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( test Test.cpp LosslessEncoder.cpp Preditor.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
target_link_libraries( test ${OpenCV_LIBS} )

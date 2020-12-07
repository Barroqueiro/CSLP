cmake_minimum_required(VERSION 2.8)

project( Golomb )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( testE TestEncode.cpp LosslessHybrid.cpp Preditor.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
add_executable( testD TestDecode.cpp LosslessHybrid.cpp Preditor.cpp Golomb.cpp RBitStream.cpp WBitStream.cpp)
target_link_libraries( testE ${OpenCV_LIBS} )
target_link_libraries( testD ${OpenCV_LIBS} )

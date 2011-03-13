project(knapsack)
cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtCore REQUIRED)

set(knapsack_SRCS ImprovedApproximateMultipleKnapsack.cpp
                  MultipleKnapsack.cpp
                  ItemVector.cpp
                  ItemVectorWithPredecessor.cpp
                  FloatItemWithIndex.cpp
                  FloatItem.cpp
                  ProfitItemWithIndex.cpp
                  ProfitItem.cpp
                  SmallBinPacking.cpp
                  ItemWithIndex.cpp
                  ChainedSet.cpp
                  SubsetAssignment.cpp
                  RelativeItemSize.cpp
                  FillRemainingMultipleKnapsack.cpp
                  ApproximateMultipleKnapsack.cpp
                  GreedyMultipleKnapsack.cpp
                  ApproximateKnapsack.cpp
                  KnapsackTestInstance.cpp
                  Test.cpp
                  Knapsack.cpp
                  Item.cpp
                  main.cpp
                  InstanceParser.cpp)

qt4_automoc(${knapsack_SRCS})

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(knapsack ${knapsack_SRCS})

target_link_libraries(knapsack ${QT_QTCORE_LIBRARY})
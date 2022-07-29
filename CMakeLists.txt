cmake_minimum_required(VERSION 3.2)
project (BarChartRace VERSION 1.0.0 LANGUAGES CXX )

#=== Main App ===

include_directories( source )

add_executable( bcr 
                source/main.cpp
               )

target_compile_features( bcr PUBLIC cxx_std_17 )


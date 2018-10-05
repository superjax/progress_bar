cmake_minimum_required (VERSION 2.8.11)
project (progress_bar)

add_library(progress_bar INTERFACE)
target_include_directories(progress_bar INTERFACE include)

include_directories(include)

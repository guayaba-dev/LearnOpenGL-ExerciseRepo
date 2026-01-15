cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLFW 3.3 QUIET)


file(GLOB SRC_FILES src/*.cpp src/*.c)
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/cap5/Exer1.cpp")
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/cap5/Exer2.cpp")

add_library(${PROJECT_NAME}_lib ${SRC_FILES})

add_executable(
  ${PROJECT_NAME}-cap5-Exer1 src/cap5/Exer1.cpp
)

add_executable(                                 
  ${PROJECT_NAME}-cap5-Exer2 src/cap5/Exer2.cpp 
)                                               


target_link_libraries(
  ${PROJECT_NAME}-cap5-Exer1 
  PRIVATE
  GL
  glfw
  ${PROJECT_NAME}_lib
)

target_link_libraries(       
  ${PROJECT_NAME}-cap5-Exer2 
  PRIVATE                    
  GL                         
  glfw                       
  ${PROJECT_NAME}_lib        
)                            

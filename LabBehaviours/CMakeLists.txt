cmake_minimum_required(VERSION 2.8.3)
project(LabBehaviours)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  geometry_msgs
)


find_package(Boost REQUIRED COMPONENTS system)

#add_message_files(DIRECTORY msg FILES Num.msg)

catkin_package(
)

include_directories(
 include
 ${/usr/include/c++/5.4.0}
 ${catkin_INCLUDE_DIRS}
)


LINK_DIRECTORIES("/usr/local/lib")

add_executable(LabBehaviours src/GoAruco.cpp)

target_link_libraries(LabBehaviours
${catkin_LIBRARIES}
)

add_dependencies(LabBehaviours GoAruco)

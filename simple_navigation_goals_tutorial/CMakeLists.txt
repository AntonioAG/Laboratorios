cmake_minimum_required(VERSION 2.8.3)
project(simple_navigation_goals_tutorial)

find_package(catkin REQUIRED COMPONENTS message_generation actionlib move_base_msgs roscpp tf nav_msgs std_msgs geometry_msgs)
catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

# Build the executable
add_executable(simple_navigation_goals src/simple_navigation_goals.cpp)
add_executable(Aruco src/Aruco.cpp)
add_executable(Origen src/Origen.cpp)
add_executable(Behaviours src/Behaviours2.cpp)
#add_executable(simple_navigation_goals src/exploracion.cpp)
# Add a build order dependency on nav_msgs

# This ensures that all msg headers are built before your executable
if(catkin_EXPORTED_TARGETS)
  add_dependencies(simple_navigation_goals ${catkin_EXPORTED_TARGETS})
  add_dependencies(Aruco ${catkin_EXPORTED_TARGETS})
  add_dependencies(Origen ${catkin_EXPORTED_TARGETS})
  add_dependencies(Behaviours ${catkin_EXPORTED_TARGETS})
#  add_dependencies(exploracion ${catkin_EXPORTED_TARGETS})
endif()
# Link against the catkin libraries
target_link_libraries(simple_navigation_goals ${catkin_LIBRARIES})
target_link_libraries(Aruco ${catkin_LIBRARIES})
target_link_libraries(Origen ${catkin_LIBRARIES})
target_link_libraries(Behaviours ${catkin_LIBRARIES})
#target_link_libraries(exploracion ${catkin_LIBRARIES})

# Install the executable
#install(TARGETS simple_navigation_goals
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
#install(TARGETS exploracion
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

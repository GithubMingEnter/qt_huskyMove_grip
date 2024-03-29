##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(class1_ros_qt_demo)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS rviz roscpp std_msgs rosmsg
    rospy
    message_generation
    cv_bridge
    image_transport
    tf
    sensor_msgs)
find_package(Qt5 REQUIRED Core Widgets Multimedia)
find_package(OpenCV REQUIRED)

#add serial
find_package(Qt5SerialPort REQUIRED)
set(QT_LIBRARIES Qt5::Widgets Qt5::Multimedia Qt5::SerialPort)
#set(QT_LIBRARIES )
include_directories(
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    )
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything.
add_message_files(
  FILES
  Robotiq2FGripper_robot_input.msg
  Robotiq2FGripper_robot_output.msg
)
generate_messages(
        DEPENDENCIES
        std_msgs

)
catkin_package( CATKIN_DEPENDS roscpp rospy std_msgs message_runtime)
##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically
# included via the dependency call in package.xml
#rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/class1_ros_qt_demo/*.hpp *.h)

QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(class1_ros_qt_demo ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(class1_ros_qt_demo ${QT_LIBRARIES} ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
install(TARGETS class1_ros_qt_demo RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

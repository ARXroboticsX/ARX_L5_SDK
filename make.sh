#!/bin/bash
workspace=$(pwd)

source ~/.bashrc

gnome-terminal -t "master1" -x  bash -c "cd ${workspace}/master1; rm -rf build devel .catkin_workspace src/CMakeLists.txt; catkin_make clean ;catkin_make; exec bash;"

gnome-terminal -t "follow1" -x  bash -c "cd ${workspace}/follow1; rm -rf build devel .catkin_workspace src/CMakeLists.txt; catkin_make clean ;catkin_make; exec bash; "



gnome-terminal -t "master2" -x  bash -c "cd ${workspace}/master2; rm -rf build devel .catkin_workspace src/CMakeLists.txt;catkin_make clean ; catkin_make; exec bash; "

gnome-terminal -t "follow2" -x  bash -c "cd ${workspace}/follow2; rm -rf build devel .catkin_workspace src/CMakeLists.txt; catkin_make clean ;catkin_make; exec bash; "




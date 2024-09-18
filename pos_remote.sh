#!/bin/bash
workspace=$(pwd)

source ~/.bashrc




 gnome-terminal -t "follow1" -x sudo bash -c "cd ${workspace}/pos_follow1; source devel/setup.bash && roslaunch arm_control L5.launch; exec bash;"
 sleep 1 
 gnome-terminal -t "follow2" -x sudo bash -c "cd ${workspace}/pos_follow2; source devel/setup.bash && roslaunch arm_control L5.launch; exec bash;"
 sleep 1 
 gnome-terminal -t "gripper" -x sudo bash -c "cd ${workspace}/gripper; source devel/setup.bash && roslaunch arm_control L5.launch; exec bash;"


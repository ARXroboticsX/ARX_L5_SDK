#!/bin/bash
source ~/.bashrc

workspace=$(pwd)

sleep 1

# gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S sudo ip link set up can2 type can bitrate 1000000;candump can2;exec bash;"

gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S slcand -o -f -s8 /dev/arxcan2 can2;sudo ifconfig can2 up;exec bash;"



sleep 1

gnome-terminal -t "launcher" -x bash -c "source ~/.bashrc;\
cd ${workspace};\
source devel/setup.bash;roslaunch arm_control L5.launch;exec bash;"





#!/bin/bash
source ~/.bashrc

workspace=$(pwd)

sleep 1

# gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S sudo ip link set up can0 type can bitrate 1000000;candump can0;exec bash;"

gnome-terminal -t "can" -x bash -c "source ~/.bashrc;source /opt/ros/noetic/setup.bash;echo 123 | sudo -S slcand -o -f -s8 /dev/arxcan0 can0;sudo ifconfig can0 up;exec bash;"



sleep 1

gnome-terminal -t "launcher" -x bash -c "source ~/.bashrc;\
cd ${workspace};\
source devel/setup.bash;roslaunch arm_control L5.launch;exec bash;"





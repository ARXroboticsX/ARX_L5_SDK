#include <ros/ros.h>
#include <cmath>
#include <iostream>
#include <std_msgs/Float32MultiArray.h>
#include "utility.h"
#include "Hardware/can.h"
#include "Hardware/motor.h"
#include "Hardware/teleop.h"
#include "App/arm_control.h"
#include "App/keyboard.h"
#include "App/play.h"
#include "App/solve.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <atomic>
#include <signal.h>
#include "arm_control/PosCmd.h"
#include "arm_control/JointControl.h"
#include "arm_control/JointInformation.h"
#include "arm_control/ChassisCtrl.h"

int CONTROL_MODE=1; 
command cmd;

bool app_stopped = false;
void sigint_handler(int sig);
void safe_stop(can CAN_Handlej);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "arm_node"); 
    ros::NodeHandle node;
    Teleop_Use()->teleop_init(node);

    arx_arm ARX_ARM((int) CONTROL_MODE, 0.0);

    ros::Publisher pub_joint = node.advertise<arm_control::JointControl>("/joint_control", 10);
    ros::Publisher pub_pos = node.advertise<arm_control::PosCmd>("/ARX_VR_L", 10);

    arx5_keyboard ARX_KEYBOARD;

    ros::Rate loop_rate(200);
    can CAN_Handlej;

    std::thread keyThread(&arx5_keyboard::detectKeyPress, &ARX_KEYBOARD);
    sleep(1);

    while(ros::ok())
    {   

        char key = ARX_KEYBOARD.keyPress.load();
        ARX_ARM.getKey(key);

        ARX_ARM.get_joint();
        if(!ARX_ARM.is_starting){
             cmd = ARX_ARM.get_cmd();
        }
        ARX_ARM.update_real(cmd);
    
////topic ////////////////////////////////////////////////////
        //发送关节数据
                    arm_control::JointInformation msg_joint;   

                    for(int i=0;i<6;i++)
                    {
                        msg_joint.joint_pos[i] = ARX_ARM.current_pos[i];
                        msg_joint.joint_vel[i] = ARX_ARM.current_vel[i];
                        msg_joint.joint_cur[i] = ARX_ARM.current_torque[i];
                    }    
                    msg_joint.joint_vel[6] = ARX_ARM.current_vel[6];
                    msg_joint.joint_cur[6] = ARX_ARM.current_torque[6];
                    msg_joint.joint_pos[6]=ARX_ARM.current_pos[6]*5; // 映射放大
    
                    pub_joint.publish(msg_joint);

        //发送末端姿态
                    arm_control::PosCmd msg_pos_back;            
                    msg_pos_back.x      =ARX_ARM.fk_end_pos[0];
                    msg_pos_back.y      =ARX_ARM.fk_end_pos[1];
                    msg_pos_back.z      =ARX_ARM.fk_end_pos[2];
                    msg_pos_back.roll   =ARX_ARM.fk_end_pos[3];
                    msg_pos_back.pitch  =ARX_ARM.fk_end_pos[4];
                    msg_pos_back.yaw    =ARX_ARM.fk_end_pos[5];
                    msg_pos_back.gripper=ARX_ARM.current_pos[6]*5;

                    pub_pos.publish(msg_pos_back);      

////topic ////////////////////////////////////////////////////


        ros::spinOnce();
        loop_rate.sleep();

        CAN_Handlej.arx_1();
        
    }
    CAN_Handlej.arx_2();
    return 0;
}


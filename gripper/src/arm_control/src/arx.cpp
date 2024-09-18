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
float gripper[4]={};
float arx_height=0;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "arm_gripper"); 
    ros::NodeHandle node;
    Teleop_Use()->teleop_init(node);

    arx_arm ARX_ARM((int) CONTROL_MODE, 0.0);//最后一个参数就是末端到电机6转子前端的距离


        ros::Subscriber sub_pos1 = node.subscribe<arm_control::JointInformation>("joint_information", 10, 
                                    [&ARX_ARM](const arm_control::JointInformation::ConstPtr& msg) {
                                        gripper[0] = msg->joint_cur[6];
                                        gripper[1] = msg->joint_vel[6];
                                    });

        ros::Subscriber sub_pos2 = node.subscribe<arm_control::JointInformation>("joint_information2", 10, 
                                    [&ARX_ARM](const arm_control::JointInformation::ConstPtr& msg) {
                                        gripper[2] = msg->joint_cur[6];
                                        gripper[3] = msg->joint_vel[6];
                                    });

        ros::Subscriber sub_pos3 = node.subscribe<arm_control::PosCmd>("LIFT", 10, 
                                    [&ARX_ARM](const arm_control::PosCmd::ConstPtr& msg) {
                                        arx_height = msg->tempFloatData[4];
                                    });

        // ros::Publisher pub_current = node.advertise<arm_control::JointInformation>("joint_information", 10);
        ros::Publisher pub_pos = node.advertise<arm_control::PosCmd>("/gripper", 10);
            
            
    arx5_keyboard ARX_KEYBOARD;

    ros::Rate loop_rate(200);
    // can CAN_Handlej;

    // std::thread keyThread(&arx5_keyboard::detectKeyPress, &ARX_KEYBOARD);
    sleep(1);

    while(ros::ok())
    { 


                    //发送末端姿态
                    arm_control::PosCmd msg_pos_back;     

                        msg_pos_back.tempFloatData[0] = -gripper[0] ;//左扭矩
                        msg_pos_back.tempFloatData[1] = gripper[1] ;//左速度
                        msg_pos_back.tempFloatData[2] = -gripper[2] ;//右扭矩
                        msg_pos_back.tempFloatData[3] = gripper[3] ;//右速度
                        msg_pos_back.tempFloatData[4] = arx_height ;        
                        msg_pos_back.tempFloatData[5] = 5 ;  
                        msg_pos_back.tempIntData[0] = 15;

                    pub_pos.publish(msg_pos_back);      

////topic ////////////////////////////////////////////////////


        ros::spinOnce();
        loop_rate.sleep();
    
    }

    return 0;
}


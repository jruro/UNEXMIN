#include <iostream>
#include <ros/ros.h>
#include <random_numbers/random_numbers.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <decision_making/TAO.h>
#include <decision_making/TAOStdProtocols.h>
#include <decision_making/ROSTask.h>
#include <decision_making/DecisionMaking.h>
#include <string>	//es necesaria (?)

#include <std_msgs/String.h>
#include <sstream>

using namespace std;
using namespace decision_making;

void chatterCallback (const std_msgs::String::ConstPtr& Num1){
	ROS_INFO("Recibiendo mensaje: [%s]", Num1->data.c_str());
}

void chatterCallback2 (const std_msgs::String::ConstPtr& Num2){
	ROS_INFO("Recibiendo mensaje: [%s]", Num2->data.c_str());
}

void chatterCallback3 (const std_msgs::String::ConstPtr& Num3){
	ROS_INFO("Recibiendo mensaje: [%s]", Num3->data.c_str());
}

void chatterCallback4 (const std_msgs::String::ConstPtr& Num4){
	ROS_INFO("Recibiendo mensaje: [%s]", Num4->data.c_str());
}

void chatterCallback5 (const std_msgs::String::ConstPtr& Num5){
	ROS_INFO("Recibiendo mensaje: [%s]", Num5->data.c_str());
}

void chatterCallback6 (const std_msgs::String::ConstPtr& Num6){
	ROS_INFO("Recibiendo mensaje: [%s]", Num6->data.c_str());
}

void chatterCallback7 (const std_msgs::String::ConstPtr& Num7){
	ROS_INFO("Recibiendo mensaje: [%s]", Num7->data.c_str());
}

void chatterCallback8 (const std_msgs::String::ConstPtr& Num8){
	ROS_INFO("Recibiendo mensaje: [%s]", Num8->data.c_str());
}

void chatterCallback9 (const std_msgs::String::ConstPtr& Num9){
	ROS_INFO("Recibiendo mensaje: [%s]", Num9->data.c_str());
}

void chatterCallback10 (const std_msgs::String::ConstPtr& Num10){
	ROS_INFO("Recibiendo mensaje: [%s]", Num10->data.c_str());
}

void chatterCallback11 (const std_msgs::String::ConstPtr& Num11){
	ROS_INFO("Recibiendo mensaje: [%s]", Num11->data.c_str());
}

void chatterCallback12 (const std_msgs::String::ConstPtr& Num12){
	ROS_INFO("Recibiendo mensaje: [%s]", Num12->data.c_str());
}

void chatterCallback13 (const std_msgs::String::ConstPtr& Num13){
	ROS_INFO("Recibiendo mensaje: [%s]", Num13->data.c_str());
}

void chatterCallback14 (const std_msgs::String::ConstPtr& Num14){
	ROS_INFO("Recibiendo mensaje: [%s]", Num14->data.c_str());
}

void chatterCallback15 (const std_msgs::String::ConstPtr& Num15){
	ROS_INFO("Recibiendo mensaje: [%s]", Num15->data.c_str());
}

void chatterCallback16 (const std_msgs::String::ConstPtr& Num16){
	ROS_INFO("Recibiendo mensaje: [%s]", Num16->data.c_str());
}

void chatterCallback17 (const std_msgs::String::ConstPtr& prueba){
	ROS_INFO("Recibiendo mensaje: [%s]", prueba->data.c_str());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "monitor_node");
	ros::NodeHandle nh;

	ros_decision_making_init(argc, argv);		//inicializamos el decision_making de ros

	// ROS spinner for topic subscriptions
	ros::MultiThreadedSpinner spinner(17);

	ROS_INFO("Receiving data from all robots...");

	ros::Subscriber sub1 = nh.subscribe("chatter_ok", 1000, chatterCallback);
	ros::Subscriber sub2 = nh.subscribe("chatter_ok_2", 1000, chatterCallback2);
	ros::Subscriber sub3 = nh.subscribe("chatter_ok_3", 1000, chatterCallback3);
	ros::Subscriber sub4 = nh.subscribe("chatter_ok_4", 1000, chatterCallback4);
	ros::Subscriber sub5 = nh.subscribe("chatter_ok_5", 1000, chatterCallback5);
	ros::Subscriber sub6 = nh.subscribe("chatter_ok_6", 1000, chatterCallback6);
	ros::Subscriber sub7 = nh.subscribe("chatter_ok_7", 1000, chatterCallback7);
	ros::Subscriber sub8 = nh.subscribe("chatter_ok_8", 1000, chatterCallback8);
	ros::Subscriber sub9 = nh.subscribe("chatter_ok_9", 1000, chatterCallback9);
	ros::Subscriber sub10 = nh.subscribe("chatter_ok_10", 1000, chatterCallback10);
	ros::Subscriber sub11 = nh.subscribe("chatter_ok_11", 1000, chatterCallback11);
	ros::Subscriber sub12 = nh.subscribe("chatter_ok_12", 1000, chatterCallback12);
	ros::Subscriber sub13 = nh.subscribe("chatter_ok_13", 1000, chatterCallback13);
	ros::Subscriber sub14 = nh.subscribe("chatter_ok_14", 1000, chatterCallback14);
	ros::Subscriber sub15 = nh.subscribe("chatter_ok_15", 1000, chatterCallback15);
	ros::Subscriber sub16 = nh.subscribe("chatter_ok_16", 1000, chatterCallback16);

	ros::Subscriber sub17 = nh.subscribe("chatter_prueba", 1000, chatterCallback17);

	//ros::spinOnce();
	ros::spin();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////                      ///////////////////////////////////////
/////////////////////////////////////// LIBRERÍAS UTILIZADAS ///////////////////////////////////////
///////////////////////////////////////                      ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <string>
#include <gazebo_msgs/SetModelState.h>
#include <std_msgs/String.h>
#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
//////////////////////     FUNCIONES move_to_crossing        ///////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void move_to_crossing_1(){

	ros::NodeHandle nh;
	ros::ServiceClient client = nh.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");

	//UNEXMIN Position
	geometry_msgs::Point unexmin_position;
	unexmin_position.x = 0.121085;
	unexmin_position.y = 0.004355;
	unexmin_position.z = 10.115476;

	//UNEXMIN orientation
	geometry_msgs::Quaternion unexmin_orientation;
	unexmin_orientation.x = -0.004744;
	unexmin_orientation.y = -0.010436;
	unexmin_orientation.z = -0.027381;
	unexmin_orientation.w = 0.0;

	geometry_msgs::Pose unexmin_pose;
	unexmin_pose.position = unexmin_position;
	unexmin_pose.orientation = unexmin_orientation;

	//設定ModelState
	gazebo_msgs::ModelState unexmin_modelstate;
	unexmin_modelstate.model_name = (std::string) "unexmin1";
	unexmin_modelstate.pose = unexmin_pose;

	gazebo_msgs::SetModelState srv;
	srv.request.model_state = unexmin_modelstate;

	if(client.call(srv))
	{
			ROS_INFO("moving UNEXMIN_1 to crossing 1...");
			return;
	}
	else
	{
			ROS_ERROR("Failed move UNEXMIN! Error msg:%s",srv.response.status_message.c_str());
			return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
//////////////////////             FUNCIÓN MAIN              ///////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	ros::init(argc, argv, "nodo_prueba");
	ros::NodeHandle nh;

	// ROS spinner for topic subscriptions
	ros::MultiThreadedSpinner spinner(1);

	ROS_INFO("Sending message to monitor...");
	ros::Publisher monitor_pub_2 = nh.advertise<std_msgs::String>("chatter_prueba", 1000);
	ros::Rate loop_rate(1);
	int count = 0;

	std_msgs::String prueba;
	std::stringstream sss;
	sss << "robot_prueba ready to GO!!"; //<< count;
	prueba.data = sss.str();
	ROS_INFO("Message from robot_prueba: %s", prueba.data.c_str());

	ROS_INFO("Moving through the mine...");

	while (ros::ok()){

		monitor_pub_2.publish(prueba);
		loop_rate.sleep();
		++count;

		if (count == 2){
			break;
		}
	}

	move_to_crossing_1();
	ROS_INFO("Done");

	ros::spin();

	return 0;
}

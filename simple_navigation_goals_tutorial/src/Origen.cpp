#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
using namespace std;
//int arucoFound = 10000;
/*
void arucoCallback(Mensaje aruco msg){
	arucoFound = msg -> Valor;
}
*/
int main(int argc, char** argv){
	ros::init(argc, argv, "Origen");
//	ros::Rate loop_rate(1);
//	ros::Subscriber aruco_listener = n.subscribe("/aruco", 1, arucoCallback);
	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);

	//wait for the action server to come up
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}

	move_base_msgs::MoveBaseGoal goal;

	//we'll send a goal to the robot to move x meters forward
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = 8.5;
	goal.target_pose.pose.position.y = -8.5;
	goal.target_pose.pose.orientation.w = 1.0;
	ROS_INFO("Sending Aruco goal");
	ac.sendGoal(goal);
	ac.waitForResult();

//	while (ros::ok){
//		if (arucoFound != 10000){
			if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
		ROS_INFO("The robot has reached the Aruco");
//			break;
		}
//		}
		else 
		{
		ROS_INFO("The base failed to reach the goal for some reason");
		}
//		ros::spinOnce();
//		loop_rate.sleep();
//	}

  return 0;
}



#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
	ros::init(argc, argv, "simple_navigation_goals");

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

// Starting point: (8.5,-8.5)
  // First goal (-4.0,-4.0)
	goal.target_pose.pose.position.x = -7.5;
	goal.target_pose.pose.position.y = -7.5;
	goal.target_pose.pose.orientation.w = 1.0;
	ROS_INFO("Sending first goal");
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the robot has reached the first goal");
	else
		ROS_INFO("The base failed to reach the goal for some reason");
 
   
  // Second goal (-6.0,6.0)
	goal.target_pose.pose.position.x = -7.5;
	goal.target_pose.pose.position.y = 7.5;
	goal.target_pose.pose.orientation.z = 1.5; 
	ROS_INFO("Sending second goal");
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the robot has reached the second goal");
	else
		ROS_INFO("The base failed to reach the goal for some reason");
   

  // Third goal (4.0,4.0)
	goal.target_pose.pose.position.x = 7.5;
	goal.target_pose.pose.position.y = 7.5;
	goal.target_pose.pose.orientation.w = 1.0; 
	ROS_INFO("Sending third goal");
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the robot has reached the third goal");
	else
		ROS_INFO("The base failed to reach the goal for some reason");



  // Fourth goal (6.0,0.0)
	goal.target_pose.pose.position.x = 4.0;
	goal.target_pose.pose.position.y = 0.0;
	goal.target_pose.pose.orientation.z = -1.5; 
	ROS_INFO("Sending fourth goal");
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the robot has reached the fourth goal");
	else
		ROS_INFO("The base failed to reach the goal for some reason");



   
  // Final goal (0.0,0.0)
	goal.target_pose.pose.position.x = 0.0;
	goal.target_pose.pose.position.y = 0.0;
	goal.target_pose.pose.orientation.w = 1.0; 
	ROS_INFO("Sending final goal");
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the robot has reached the final goal");
	else
		ROS_INFO("The base failed to reach the goal for some reason");

  return 0;
}


#include "ros/ros.h"
#include <geometry_msgs/PoseStamped.h>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "GoAruco");
  ros::NodeHandle n;

  ros::Publisher pose_pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1);
  geometry_msgs::PoseStamped msg_pose;
  ros::Rate loop_rate(100);
  float x,y,w;
  int count=0;
while (count<30)
{

//  cout << "Please enter x: ";
//  cin  >> x;
//  cout << "Please enter y: ";
//  cin  >> y;
//  cout << "Please enter w: ";
//  cin  >> w;

  msg_pose.header.stamp = ros::Time::now();
  msg_pose.header.frame_id = "map";
  msg_pose.pose.position.x = 1.0;
  msg_pose.pose.position.y = 0.0;
  msg_pose.pose.position.z = 0.0;
  msg_pose.pose.orientation.w = 1.0;

  pose_pub.publish(msg_pose);

  ros::spinOnce();
  loop_rate.sleep();
  count++;
}
return 0;
}

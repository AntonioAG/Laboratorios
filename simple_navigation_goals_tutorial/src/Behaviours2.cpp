#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/Odometry.h>
#include <math.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
using namespace std;

int state = 1;

std_msgs::Bool arucoFound;
geometry_msgs::Vector3 objetivoDetectado;
geometry_msgs::Vector3 moneda;

bool objetivoCumplido = false;
bool exploraaux = false;
bool goalaux = false;
float x,y;
nav_msgs::Odometry odom_msgs;

move_base_msgs::MoveBaseGoal goal;
void goToPosition(float x,float y){
	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);

	//wait for the action server to come up
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}
	//we'll send a goal to the robot to move to the position in the map
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();
	
	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	//Seria conveniente calcular en angulo en que tiene que terminar
	//Aunque como suele darse vueltas en el sitio al llegar tampoco parece vital
	//Podría ser importante para las monedas, podría usarse el laser para obtener la orientacion
	goal.target_pose.pose.orientation.w = 1.0;
	ROS_INFO("Sending goal");
	ac.sendGoal(goal);

 // return 0;
}

void Avanza_recto(){
	for (int i=0;i<1500;i++){
		geometry_msgs::Twist vel_msgs;
		ros::NodeHandle nh;
		ros::Publisher pubPosition = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
		vel_msgs.linear.x = 0.4;
		vel_msgs.angular.z = 0.0;
		pubPosition.publish(vel_msgs);
	}
}

//Las siguientes funciones deberían leer los topics donde publican
//los programas de vision para saber si hay objetos de interés en el camino

void arucoCallback(const std_msgs::Bool::ConstPtr& msg){
arucoFound.data = msg -> data;
}
void objetivoDetectadoCallback(const geometry_msgs::Vector3::ConstPtr& msg){
objetivoDetectado.x = msg -> x;
objetivoDetectado.y = msg -> y;
objetivoDetectado.z = msg -> z;
}
void monedaCallback(const geometry_msgs::Vector3::ConstPtr& msg){
moneda.x = msg -> x;
moneda.y = msg -> y;
moneda.z = msg -> z;
}
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg){
odom_msgs.pose.pose.position.x = msg -> pose.pose.position.x;
odom_msgs.pose.pose.position.y = msg -> pose.pose.position.y;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Behaviours");
	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);

	//wait for the action server to come up
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}
	ros::NodeHandle n;
	ros::Subscriber subaruco = n.subscribe("/aruco_detectado", 1, arucoCallback);
	ros::Subscriber subobjetivo = n.subscribe("/objetivo_detectado", 1, objetivoDetectadoCallback);
	ros::Subscriber submoneda = n.subscribe("/moneda", 1, monedaCallback);
	ros::Subscriber subodom = n.subscribe("/odom", 1, odomCallback);
	ros::Rate loop_rate(50);
	cout << "Please enter x: ";
	cin  >> x;
	cout << "Please enter y: ";
	cin  >> y;
	arucoFound.data = false;
	objetivoDetectado.z = 0.0;
	moneda.z = 0.0;
	while (ros::ok)
	{
		switch (state) 
		{
			case 1: //Ir al aruco
				if (!goalaux&&!arucoFound.data&&moneda.z == 0.0){
					ROS_INFO("Voy a por el aruco");
					goToPosition(x,y);
					goalaux = true;
				}
				else if (moneda.z == 1.0){
					ac.cancelAllGoals ();
					state = 3;
					goalaux = false;
				}
				else if (arucoFound.data){
					ac.cancelAllGoals ();
					state = 2;
					goalaux = false;
				}
			break;
			case 2: //Explorar
				if (!exploraaux)
				{
					ROS_INFO("Estoy explorando");
					int a = system("roslaunch explore_lite explore.launch &");
					exploraaux = true;
				}
				if (moneda.z == 1.0)
				{
					system("rosnode kill explore");
					ac.cancelAllGoals ();
					exploraaux = false;
					state = 3;
				}
				if (objetivoDetectado.z == 1.0)
				{
					system("rosnode kill explore");
					ac.cancelAllGoals ();
					exploraaux = false;
					state = 4;
				}
			break;
			case 3: //Ir a coger una moneda
			if (!goalaux){
				goToPosition(moneda.x,moneda.y);//x e y se deben asignar en el callback
				ROS_INFO("Voy a por una moneda");
				goalaux = true;
				}
				if (sqrt(pow(odom_msgs.pose.pose.position.x-moneda.x,2.0)+pow(odom_msgs.pose.pose.position.y-moneda.y,2.0)) < 0.6) //Si ya estamos cerca, vamos rectos
				{
					ROS_INFO("Ya casi tengo la moneda, cancelo la orden anterior");
					ac.cancelAllGoals ();
					ROS_INFO("Avanzo recto");
					Avanza_recto();	
					
					if ((!objetivoCumplido)&&(objetivoDetectado.z == 0.0)&&(arucoFound.data))
					{
						state = 2;	//Si todavía no se ha encontrado el objetivo, se sigue explorando
						ac.cancelAllGoals ();
						goalaux = false;
					}
					else if ((objetivoCumplido)&&(objetivoDetectado.z == 1.0)&&(arucoFound.data))
					{
						state = 5;	//Si la mision ya se ha terminado, se sigue el camino hacia el origen
						ac.cancelAllGoals ();
						goalaux = false;
					}
					else if ((!objetivoCumplido)&&(objetivoDetectado.z == 1.0)&&(arucoFound.data))
					{
						state = 4;	//Si se ha encontrado el objetivo pero no se ha llegado, se sigue hacia él
						ac.cancelAllGoals ();
						goalaux = false;
					}
					else if (!arucoFound.data)
					{
						state = 1;	//Si todavía no se ha llegado al Aruco, se sigue el camino
						ac.cancelAllGoals ();
						goalaux = false;
					}
				}
			break;
			case 4: //Ir al objetivo
			if (!goalaux){
				ROS_INFO("Ya puedo ver el objetivo, voy a por el");
				goToPosition(objetivoDetectado.x,objetivoDetectado.y);//Estas tienen que venir del callback de objetivoDetectado
				goalaux = true;
				}
				if (sqrt(pow(odom_msgs.pose.pose.position.x - objetivoDetectado.x,2.0)+pow(odom_msgs.pose.pose.position.y - objetivoDetectado.y,2.0)) < 0.6) 
				{
					ROS_INFO("Ya he llegado, ahora deberia esperar un poco aqui");
					objetivoCumplido = true;
					state = 5;
					ac.cancelAllGoals ();
					goalaux = false;
				}
			break;
			case 5: //Volver al origen
			if (!goalaux){
				goToPosition(8.5,-8.5);
				ROS_INFO("Estoy de camino al punto de partida");
				goalaux = true;
			}
				if (moneda.z == 1.0)
					{
						state = 3;
						ac.cancelAllGoals ();
						goalaux = false;
					}
			break;
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
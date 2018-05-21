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

bool objetivoCumplido = false;
bool exploraaux = false;
float x,y;

geometry_msgs::Vector3 moneda;
nav_msgs::Odometry odom_msgs;

void goToPosition(float x,float y){
	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);

	//wait for the action server to come up
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}

	move_base_msgs::MoveBaseGoal goal;

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
	
	geometry_msgs::Twist vel_msgs;
	ros::NodeHandle nh;
	ros::Publisher pubPosition = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	vel_msgs.linear.x = 0.4;
	vel_msgs.angular.z = 0.0;
	pubPosition.publish(vel_msgs);
	ros::Duration(3).sleep(); // sleep for second and a half
//	sleep(3000);
	vel_msgs.linear.x = 0.0;
	pubPosition.publish(vel_msgs);
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
	ros::NodeHandle n;
	ros::Subscriber subaruco = n.subscribe("/aruco_detected", 1, arucoCallback);
	ros::Subscriber subobjetivo = n.subscribe("/objetivo_detected", 1, objetivoDetectadoCallback);
	ros::Subscriber submoneda = n.subscribe("/moneda", 1, monedaCallback);
	ros::Subscriber subodom = n.subscribe("/odom", 1, odomCallback);
	ros::Rate loop_rate(0.5);
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
				if (!arucoFound.data&&moneda.z == 0.0){
					ROS_INFO("Voy a por el aruco");
					goToPosition(x,y);
				}
				else if (moneda.z == 1.0){
					state = 3;
				}
				else if (arucoFound.data){
					state = 2;
				}
			break;
			case 2: //Explorar (aquí falta por hacer)
			ROS_INFO("Estoy explorando");
				if (!exploraaux)
				{
					int a = system("gnome-terminal -e roslaunch explore_lite explore.launch &");
					exploraaux = true;
				}
				if (moneda.z == 1.0)
				{
					system("rosnode kill explore");
					exploraaux = false;
					state = 3;
				}
				else if (objetivoDetectado.z == 1.0)
				{
					system("rosnode kill explore");
					exploraaux = false;
					state = 4;
				}
			break;
			case 3: //Ir a coger una moneda
			ROS_INFO("Voy a por una moneda");
				goToPosition(moneda.x,moneda.y);//x e y se deben asignar en el callback
				if (sqrt(pow(odom_msgs.pose.pose.position.x-moneda.x,2.0)+pow(odom_msgs.pose.pose.position.y-moneda.y,2.0)) < 0.6) //Si ya estamos cerca, vamos rectos
				{
					ROS_INFO("Ya casi tengo la moneda, cancelo la orden anterior");
					system("rostopic pub /move_base/cancel actionlib_msgs/GoalID -- {}");
//					ros::Duration(5).sleep(); // sleep for second
					ROS_INFO("Ya casi tengo la moneda, avanzo recto");
					Avanza_recto();	//medio resuelto
					moneda.z == 0.0;	//Una vez recogida se pone el valor a 0
					
					if ((!objetivoCumplido)&&(objetivoDetectado.z == 0.0)&&(arucoFound.data))
					{
						state = 2;	//Si todavía no se ha encontrado el objetivo, se sigue explorando
					}
					else if ((objetivoCumplido)&&(objetivoDetectado.z == 1.0)&&(arucoFound.data))
					{
						state = 5;	//Si la mision ya se ha terminado, se sigue el camino hacia el origen
					}
					else if ((!objetivoCumplido)&&(objetivoDetectado.z == 1.0)&&(arucoFound.data))
					{
						state = 4;	//Si se ha encontrado el objetivo pero no se ha llegado, se sigue hacia él
					}
					else if (!arucoFound.data)
					{
						state = 1;	//Si todavía no se ha llegado al Aruco, se sigue el camino
					}
				}
			break;
			case 4: //Ir al objetivo (aquí quedan detalles)
			ROS_INFO("Ya puedo ver el objetivo, voy a por el");
				goToPosition(objetivoDetectado.x,objetivoDetectado.y);//Estas tienen que venir del callback de objetivoDetectado
				if (sqrt(pow(odom_msgs.pose.pose.position.x - objetivoDetectado.x,2.0)+pow(odom_msgs.pose.pose.position.y - objetivoDetectado.y,2.0)) < 0.6) 
				{
					ROS_INFO("Ya he llegado, ahora deberia esperar un poco aqui");
					objetivoCumplido = true;
					state = 5;
				}
			break;
			case 5: //Volver al origen
			ROS_INFO("Esty de camino al punto de partida");
				goToPosition(8.5,-8.5);
				if (moneda.z == 1.0)
					{
						state = 3;
					}
			break;
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
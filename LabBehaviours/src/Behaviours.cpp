#include "ros/ros.h"
#include <geometry_msgs/PoseStamped.h>

using namespace std;

int state = 1;
int moneda = 0;
int arucoFound = 0;
int objetivoDetectado = 0;
int objetivoCumplido = 0;

//Las siguientes funciones deberían leer los topics donde publican
//los programas de vision para saber si hay objetos de interés en el camino

void arucoCallback
void objetivoDetectadoCallback
void monedaCallback

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Behaviours");
	ros::NodeHandle n;
	ros::Rate loop_rate(100);

	swtich (state) 
	{
		case 1: 
		{
			system("rosrun simple_navigation_goals_tutorial Aruco");
			if (moneda == 1)
				state = 3;
			else
				state = 2;
		}
		case 2:
		{
			system("roslaunch explore_lite explore.launch");//Hay que modificar el explore para que muera si detecta moneda
			if (moneda == 1)
			{
				system("rosnode kill explore");
				state = 3;
			}
			else if (objetivoDetectado == 1)
			{
				system("rosnode kill explore");
				state = 4;
			}
		}
		case 3:
		{
			ir a por moneda; //medio resuelto
			Avanza recto;	//medio resuelto
			moneda == 0;	//Una vez recogida se pone el valor a 0
			if ((objetivoCumplido == 0)&&(objetivoDetectado == 0))
			{
				state = 2;	//Si todavía no se ha encontrado el objetivo, se sigue explorando
			}
			else if (objetivoCumplido == 1)
			{
				state = 5;	//Si la mision ya se ha terminado, se sigue el camino hacia el origen
			}
			else
				state = 4;	//Si se ha encontrado el objetivo pero no se ha llegado a él, se sigue el camino hacia el objetivo
		}
		case 4:
		{
			ir a por objetivo //medio resuelto
			objetivoCumplido == 1;
			state = 5;
		}
		case 5:
		{
			system("rosrun simple_navigation_goals_tutorial Aruco");
			if (moneda == 1)
				state = 3;

		}
	}

	return 0;
}

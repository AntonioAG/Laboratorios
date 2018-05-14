# LABORATORIO DE AUTOMÁTICA Y ROBÓTICA


### Paquete labrob_gazebo
* * *

Para usar el mundo de Gazebo del paquete 'labrob_gazebo' sigue los siguientes pasos:

 * Copia este paquete (carpeta labrob_gazebo con todo su contenido) en tu espacio de trabajo (catkin_ws) y compila catkin_make.

 * Configura la variable de entorno GAZEBO_MODEL_PATH para indicar que los modelos de Gazebo se carguen desde el directorio 'models' del propio paquete. Para ello, incluye la siguiente línea en tu .bashrc:

    `export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:~/catkin_ws/src/labrob_gazebo/models`

 * Lanza el mundo en Gazebo
 
    `roslaunch labrob_gazebo sim_world.launch`

### Paquete ros_plugins
* * *

Para usar el mundo de Gazebo del paquete 'ros_plugins' sigue los siguientes pasos:

 * Copia este paquete (carpeta ros_plugins con todo su contenido) en tu espacio de trabajo (catkin_ws) y compila catkin_make.

 * Configura los directorios en las lineas 223 a la 254 de ros_plugins.cpp:

    `std::ifstream in_walls_1("/home/ramon/workspace/robotics_lab/src/ros-muar-upm/labrob_gazebo/models/barrier_1/model.sdf");`

 * Cambiar `/home/ramon/workspace/robotics_lab/src/ros-muar-upm/` a `/home/NOMBRE_PC/catkin_ws/src/` donde NOMBRE_PC es el nombre de ubuntu.

 * Lanza el mundo en Gazebo (en plugin_sim.launch le puedes asignar el valor del ARUCO que quieres que aparezca en el entorno)
 
    `roslaunch labrob_gazebo plugin_sim.launch`

### TABLA DE ARUCOS (Las imagenes de los arucos se encuentran en /labrob_gazebo/models/aruco_cube/materials/textures)
 		     
 * Avion             ARUCO ID
	* Azul	   |	1
 	* Verde	   |	2
    	* Rojo	   |	3
--------------------------------
 * Bicicleta 
	* Azul	   |	4
 	* Verde	   |	5
    	* Rojo     |	6
--------------------------------
 * Barco 
	* Azul	   |	7
 	* Verde	   |	8
    	* Rojo	   |	9
--------------------------------
 * Botella 
	* Azul	   |	10
 	* Verde	   |	11
    	* Rojo	   |	12
--------------------------------

### Referencia de Coordenadas

 * La referencia se situa en el origen de el eje de referencia del entorno de Gazebo, El robot movil que se utilize debe empezar desde la coordenada (x,y) = (8.5, -8.5);

### Prueba Final (29/01/2018)

 * Se han añadido dos entornos para utilizar en la prueba final. En /labrob_gazebo/worlds estan final_test.world y final_test_2.world. Ademas, se ha añadido otro launcher llamado plugin_sim_final.launch. Este launcher añade el puntoi de informacion y se encarga de la contabilizacion de los puntos de las monedas y bombas.



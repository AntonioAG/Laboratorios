Para lanzar el mundo se debe ejecutar:

roslaunch labrob_gazebo labrob.launch

Para probar la navegación se debe lanzar:

rostun simple_navigation_goals_tutorial Behaviours

Al ejecutar este nodo se piden las coordenadas x e y del aruco, en coordenadas del mapa.

Puesto que la visión aún no está completa, se debe simular la publicación en los topics como si se hubiera detectado el aruco, una moneda o el objetivo de la misión. Entonces el robot reaccionará, yendo a por los objetivos o bien explorando el mapa en busca de ellos.

En cualquier momento tras introducir las coordenadas del aruco, se puede simular que se ha visto una moneda, para ello se debe publicar:

rostopic pub /moneda geometry_msgs/Vector3 "x: 5.0     
y: -8.0
z: 1.0" 

Siendo X e Y las coordenadas del mapa y Z un indicador que indica al nodo que debe ir a por la moneda

A continuación se debe publicar un comando con Z = 0.0 para que no siga yendo a por la moneda tras llegar.

rostopic pub /moneda geometry_msgs/Vector3 "x: 5.0     
y: -8.0
z: 0.0" 

HAY QUE DEJAR QUE LLEGUE A LA MONEDA
Una vez llegue a la posición indicada, el robot recuperará la tarea anterior.

Para simular que se ha llegado al aruco debe publicarse:

rostopic pub /aruco_detectado std_msgs/Bool "data: true"

Inmediatamente comenzará a explorar el mapa en busca del objetivo.

Para simular que se ha encontrado el objetivo se lanza el comando:

rostopic pub /objetivo_detectado geometry_msgs/Vector3 "x: 5.0
y: -8.0
z: 1.0" 

Al igual que en el caso de la moneda, se debe cambiar z = 0.0 tras enviar este comando para que no se atasque en el estado de búsqueda del objetivo.

Finalmente, tras llegar al objeto de interés, el robot volverá automáticamente a la posición de partida.
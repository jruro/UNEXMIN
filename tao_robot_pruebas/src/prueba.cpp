////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////                      ///////////////////////////////////////
/////////////////////////////////////// LIBRERÍAS UTILIZADAS ///////////////////////////////////////
///////////////////////////////////////                      ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ros/ros.h>
#include <random_numbers/random_numbers.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <decision_making/TAO.h>
#include <decision_making/TAOStdProtocols.h>
#include <decision_making/ROSTask.h>
#include <decision_making/DecisionMaking.h>
//#include <string>	//es necesaria (?)
#include <gazebo_msgs/SetModelState.h>

#include <std_msgs/String.h>
#include <sstream>

using namespace std;
using namespace decision_making;

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
///////////////////////////////     CONSTANTES Y MACROS      ///////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_SCAN_ANGLE_RAD -45.0/180*M_PI
#define MAX_SCAN_ANGLE_RAD +45.0/180*M_PI
#define MIN_DIST_TO_OBSTACLE 1.0 		//unidad en metros

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
///////////////////////////////          VARIABLES           ///////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

random_numbers::RandomNumberGenerator _randomizer;	//Generador de numeros aleatorios
ros::Publisher _velocityPublisher;			//Publicador de la velocidad (?)

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
///////////////////////////////       MODELO GLOBAL          ///////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

struct WorldModel : public CallContextParameters {

	bool obstacleDetected;		//Warning de obstáculo detectado
	bool driveFinished;		//Warning de conducción finalizada
	bool turnFinished;		//Warning de giro finalizado
	//bool turnLeft;			//booleano que usamos para indicar el giro a derecha o a izq

	string str() const {
		stringstream s;		//Creamos cadena 's'
		s << "obstacleDetected = " << obstacleDetected;	//Sacamos por pantalla si el obstaculo ha sido detectado, pero mediante la variable struct WM
		return s.str();
	}
};
#define WM TAO_CONTEXT.parameters<WorldModel>()

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
///////////////////////////////        MÁQUINA TAO           ///////////////////////////////////////
///////////////////////////////                              ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TAO_HEADER(Drive)	//Creamos dos subplans, Drive y
TAO_HEADER(Turn)	// Turn

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////                TAO PRINCIPAL           ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


TAO(Wandering){

	TAO_PLANS{
		Wandering
	}
	TAO_START_PLAN(Wandering);	//Definimos comienzo del plan
	TAO_BGN{
		TAO_PLAN(Wandering){
			TAO_START_CONDITION(true);

			WM.driveFinished = false;	//Definimos que los Warnings de conduccion finalizada y giro finalizado sean falsos porque lo que queremos es que se ejecute el movimientos al comienzo de la maquina TAO
			WM.turnFinished = false;

			TAO_ALLOCATE(AllocFirstReady){
				TAO_SUBPLAN(Drive);	//Definimos los subplans en la zona de Allocate
				TAO_SUBPLAN(Turn);
			}
			TAO_STOP_CONDITION (WM.driveFinished || WM.turnFinished);
			TAO_NEXT(NextFirstReady){
				TAO_NEXT_PLAN(Wandering);	//Especie de bucle para que se llame continuamente al mismo plan Wandering, para que no se pare y se ejecute una sola vez
			}
		}
	}
	TAO_END
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////                SUBPLANS           //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TAO(Drive){

	TAO_PLANS{
		Drive,
	}
	TAO_START_PLAN(Drive);
	TAO_BGN{
		TAO_PLAN(Drive){
			TAO_START_CONDITION(!WM.obstacleDetected);	//El subplan comienza cuando no se da el warning de obstaculo detectado
			TAO_CALL_TASK(driveTask);	//Llamamos a una tarea dentro del subplan llamada tarea de conducir, es decir, mientras el subplan está ejecutado, se ejecuta la tarea de conduccion
			TAO_ALLOCATE_EMPTY
			TAO_STOP_CONDITION(WM.obstacleDetected);
			TAO_NEXT_EMPTY
			WM.driveFinished = true;
		}
	}
	TAO_END
}

TAO(Turn){

	TAO_PLANS{
		Turn,
	}
	TAO_START_PLAN(Turn);
	TAO_BGN{
		TAO_PLAN(Turn){
			TAO_START_CONDITION(WM.obstacleDetected);
			TAO_CALL_TASK(turnTask);
			TAO_ALLOCATE_EMPTY
			TAO_STOP_CONDITION(true);
			TAO_NEXT_EMPTY
			WM.turnFinished=true;
		}
	}
	TAO_END
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
//////////////////////        IMPLEMENTACIÓN DE TAREAS       ///////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                CONDUCCIÓN             ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TaskResult driveTask(string name, const CallContext& context, EventQueue& eventQueue){

	ROS_INFO ("Driving unexmin...");	//Mandamos por ROS_INFO que el robot está en la tarea de conducir
	geometry_msgs::Twist forwardMessage;
	forwardMessage.linear.x = 1000.0;	//definimos, a traves de la libreria geometry_msgs y por tanto utilizando los mensajes que nos ofrece ROS a traves de dicha libreria, la velocidad de la conduccion, en 0.9 m/s

	// Realizamos una espera preventiva, publicando el mensaje driveForward hasta que el eventQueue finalice, publicando dicho mensaje con un intervalo de 100 milisegundos

	while (!eventQueue.isTerminated()){
		_velocityPublisher.publish(forwardMessage);
		boost::this_thread::sleep(boost::posix_time::milliseconds(100.0));
	}

	//Una vez que hayamos salido del while, eso significa que dejamos de publicar el mensaje yp or lo tanto hemos detectado un obstaculo

	//ROS_INFO("Obstacle detected!! Please wait until turn is finished...");

	return TaskResult::SUCCESS();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                 GIRO                  ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TaskResult turnTask(string name, const CallContext& context, EventQueue& eventQueue){

	ROS_INFO("Turning robot1...");

	bool turnLeft= _randomizer.uniformInteger(0,1);

	geometry_msgs::Twist turnMessage;
	turnMessage.angular.z = 2 * (turnLeft ? 1 : -1);	// ?

	int timeToTurnMs = _randomizer.uniformInteger(2000, 4000);
	int turnLoops = timeToTurnMs/100;

	for (int i = 0; i<turnLoops; i++){

		_velocityPublisher.publish(turnMessage);
		boost::this_thread::sleep(boost::posix_time::milliseconds(100.0));

	}

	return TaskResult::SUCCESS();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
//////////////////////  SUSCRIPCIONES ROS Y FUNCIÓN MAIN     ///////////////////////////////////////
//////////////////////                                       ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////                 LASERSCAN                ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void onLaserScanMessage(const sensor_msgs::LaserScan::Ptr laserScanMessage, CallContext* context){

	bool obstacleFound = false;

	int minIndex = ceil((MIN_SCAN_ANGLE_RAD-laserScanMessage->angle_min) / laserScanMessage->angle_increment);	//Definimos el valor minimo posible para el LaserScan

	int maxIndex = floor((MAX_SCAN_ANGLE_RAD-laserScanMessage->angle_min)/laserScanMessage->angle_increment);	//Definimos el valor maximo posible para el LaserScan

	for (int i = minIndex; i <= maxIndex; i++){	//si el robot se encuentra entre esos valores, pasa a detectar si se encuentra por debajo del limite permisivo, que son 0.8m

		if (laserScanMessage->ranges[i]<MIN_DIST_TO_OBSTACLE){

			obstacleFound = true;	//en caso de que se encuentre por debajo de dicho valor, se activa el warning de obstaculo encontrado
		}
	}

	context->parameters<WorldModel>().obstacleDetected = obstacleFound;	//es importante que asignemos obstaculo encontrado a obstaculo detectado. La primera variable pertenece a la suscripcion de ROS, pero la segunda pertenece a la maquina TAO del WorldModel. Esto permitirá a la máquina cambiar de subplan o de tarea, es decir, permitirá a la máquina evolucionar correctamente.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////                   MAIN                   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	ros::init(argc, argv, "nodo_prueba");
	ros::NodeHandle nh;

	ros_decision_making_init(argc, argv);		//inicializamos el decision_making de ros

	// ROS spinner for topic subscriptions
	ros::MultiThreadedSpinner spinner(2);

	// Tasks registration
	LocalTasks::registrate("driveTask", driveTask);	//registro de tarea de conduccion
	LocalTasks::registrate("turnTask", turnTask);	//registro de tarea de giro

	RosEventQueue eventQueue;
	CallContext context;
	context.createParameters(new WorldModel());	//creamos el WorldModel, es necesario crearlo desde el main para que lo podamos usar con la implementacion del struct de arriba

	// Subscription for the laser topic and velocity publisher creation
	ros::Subscriber laserSubscriber = nh.subscribe<void>("base_scan", 1,
					boost::function<void(const sensor_msgs::LaserScan::Ptr)>(boost::bind(onLaserScanMessage, _1, &context)));
	_velocityPublisher = nh.advertise<geometry_msgs::Twist>("cmd_vel", 100);

  eventQueue.async_spin();

	/*ros::ServiceClient client = nh.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");

	geometry_msgs::Pose start_pose;
	start_pose.position.x = 0;
	start_pose.position.y = 0;
	start_pose.position.z = 100;

	start_pose.orientation.x = -0.004744;
	start_pose.orientation.y = -0.010436;
	start_pose.orientation.z = -0.027381;
	start_pose.orientation.w = 0.0;

	geometry_msgs::Twist start_twist;
	start_twist.linear.x = 0;
	start_twist.linear.y = 0;
	start_twist.linear.z = 0;

	start_twist.angular.x = 0;
	start_twist.angular.y = 0;
	start_twist.angular.z = 1.5;

	gazebo_msgs::ModelState modelstate;
	modelstate.model_name = "UNEXMIN";
	modelstate.reference_frame = "world";
	modelstate.pose = start_pose;
	modelstate.twist = start_twist;

	gazebo_msgs::SetModelState setmodelstate;
	setmodelstate.request.model_state = modelstate;

	if(client.call(setmodelstate))
	{
			ROS_INFO("UNEXMIN magic moving success!!");
	}
	else
	{
			ROS_ERROR("Failed to magic move UNEXMIN! Error msg:%s",setmodelstate.response.status_message.c_str());
	}*/

	//client.call(setmodelstate);

	/*//UNEXMIN Position
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
	unexmin_modelstate.model_name = (std::string) "UNEXMIN";
	unexmin_modelstate.pose = unexmin_pose;

	gazebo_msgs::SetModelState srv;
	srv.request.model_state = unexmin_modelstate;

	if(client.call(srv))
	{
			ROS_INFO("UNEXMIN magic moving success!!");
	}
	else
	{
			ROS_ERROR("Failed to magic move UNEXMIN! Error msg:%s",srv.response.status_message.c_str());
	}*/

	int count = 0;

	ROS_INFO("Sending message to monitor...");
	ros::Publisher monitor_pub_2 = nh.advertise<std_msgs::String>("chatter_prueba", 1000);
	ros::Rate loop_rate(1);

	std_msgs::String prueba;
	std::stringstream sss;
	sss << "robot_prueba ready to GO!!"; //<< count;
	prueba.data = sss.str();
	ROS_INFO("Message from robot_prueba: %s", prueba.data.c_str());

	while (ros::ok()){

		monitor_pub_2.publish(prueba);
		loop_rate.sleep();
		++count;

		if (count == 2){
			break;
		}
	}

	ros::spinOnce();

	ROS_INFO("Starting wandering machine 1...");
	TaoWandering(&context, &eventQueue);

	//eventQueue.close();
	//ROS_INFO("TAO finished.");

	return 0;
}

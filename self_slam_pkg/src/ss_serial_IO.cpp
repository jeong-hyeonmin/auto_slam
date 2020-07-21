/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <self_slam_msgs/velocity.h>
#include <geometry_msgs/Twist.h>
#include <cstdlib>

using namespace std;




//sudo chmod 766 /dev/ttyUSB0


/*
serial::Serial ser;



void write_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data);float64 x
float64 y
float64 z
}



int main (int argc, char** argv){
    ros::init(argc, argv, "serial_IO");
    ros::NodeHandle nh;

    ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback);
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000);

    try
    {
        ser.setPort("/dev/ttyACM0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {float64 x
float64 y
float64 z
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }

    ros::Rate loop_rate(5);
    while(ros::ok()){

//      ser.write("a");

        ros::spinOnce();

        if(ser.available()){
            ROS_INFO_STREAM("Reading from serial port");
            std_msgs::String result;
            result.data = ser.read(ser.available());
            ROS_INFO_STREAM("Read: " << result.data);
            read_pub.publish(result);
        }
        loop_rate.sleep();

    }
}
*/



serial::Serial ser;

double linear_x;
double linear_y;
double angular_z;

int cnt = 0;
char rx_buffer[13];



void write_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data);
}

void goal_vel_callback(const geometry_msgs::Twist::ConstPtr& msg){
  ROS_INFO_STREAM("got cmd_vel linear x" << msg->linear.x);
  ROS_INFO_STREAM("got cmd_vel linear y" << msg->linear.y);
  ROS_INFO_STREAM("got cmd_vel linear z" << msg->linear.z);

  ROS_INFO_STREAM("got cmd_vel angular x" << msg->angular.x);
  ROS_INFO_STREAM("got cmd_vel angular y" << msg->angular.y);
  ROS_INFO_STREAM("got cmd_vel angular z" << msg->angular.z);

  ROS_INFO("cmd_vel come in");

 linear_x = msg->linear.x;
 linear_y = msg->linear.y;
 angular_z = msg->angular.z;

}




int main (int argc, char** argv){
    ros::init(argc, argv, "ss_serial_IO");
    ros::NodeHandle nh;

    ros::Subscriber write_sub = nh.subscribe("write", 1, write_callback); //send
    ros::Subscriber goal_sub = nh.subscribe("/cmd_vel",1, goal_vel_callback);


    
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1); //receive
    ros::Publisher velocity_pub1 = nh.advertise<self_slam_msgs::velocity>("velocity_x", 1); //receive
    ros::Publisher velocity_pub2 = nh.advertise<self_slam_msgs::velocity>("velocity_y", 1); //receive
    ros::Publisher velocity_pub3 = nh.advertise<self_slam_msgs::velocity>("velocity_th", 1); //receive



/*

    try
    {
        //ser.setPort("/dev/ttyACM0");
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable tdatao open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }

    */

    ros::Rate loop_rate(10);

    while(ros::ok()){

        ros::spinOnce();
        cnt++;

        if(ser.available()){
            ROS_INFO_STREAM("Reading from serial port");
            std_msgs::String receive[14];
            string str;


//            ser.readline(receive.data,500,"\n");
//            cout << receive.data << endl;


           for(int i = 0; i<14; i++)
            ser.read(receive[i].data);


           for(int i = 0; i<14; i++)
             cout << (receive[i].data) << endl;

           for(int i = 0; i<14; i++)
           {
             const char *change[14];
             change[i] = receive[i].data.c_str();

             int hello = atoi(change[i]);

             cout << "this is int value i = " << i << "   hello : "<<hello << endl;

           }


            //ser.read(Ch);

            //ROS_INFO("%c", Ch);



//            ROS_INFO_STREAM("Writing to serial port" << receive.data);

        }


//-------------------------------publish to subscriber(odometry)
        self_slam_msgs::velocity velocity_x;
        self_slam_msgs::velocity velocity_y;
        self_slam_msgs::velocity velocity_th;


        velocity_x.x = linear_x;
        velocity_y.y = linear_y;
        velocity_th.th = angular_z;

        velocity_pub1.publish(velocity_x);
        velocity_pub2.publish(velocity_y);
        velocity_pub3.publish(velocity_th);

//---------------------------------------------------------------------




//-------------------------send to nucelo board


        std_msgs::String send_x_over;
        std_msgs::String send_x_under;
        
        std_msgs::String send_z_over;
        std_msgs::String send_z_under;


        send_x_over.data = int(linear_x);
        send_x_under.data = int((linear_x - int(linear_x))*100);
        send_z_over.data = int(angular_z);
        send_z_under.data = int((angular_z - int(angular_z))*100);


        ser.write("*");
        ser.write(send_x_over.data);
        ser.write(".");
        ser.write(send_x_under.data);

        ser.write(",");
        ser.write(send_z_over.data);
        ser.write(".");
        ser.write(send_z_under.data);
        ser.write("#");



        loop_rate.sleep();


 //------------------------------------------
    }
}


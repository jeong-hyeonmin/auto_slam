#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <self_slam_msgs/velocity.h>

double msg_x;
double msg_y;
double msg_th;


void RecvcallBack1(const self_slam_msgs::velocity::ConstPtr& msg){
  msg_x = msg->x;
  ROS_INFO("receive x: %f",msg_x);
}

void RecvcallBack2(const self_slam_msgs::velocity::ConstPtr& msg){
  msg_y = msg->y;
  ROS_INFO("receive y: %f",msg_y);
}

void RecvcallBack3(const self_slam_msgs::velocity::ConstPtr& msg){
  msg_th = msg->th;
  ROS_INFO("receive th: %f",msg_th);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "ss_odometry_publisher");
  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom",50);
  
  ros::Subscriber serial_sub1 = n.subscribe("velocity_x",1,RecvcallBack1);
  ros::Subscriber serial_sub2 = n.subscribe("velocity_y",1,RecvcallBack2);
  ros::Subscriber serial_sub3 = n.subscribe("velocity_th",1,RecvcallBack3);
  
  
  tf::TransformBroadcaster odom_broadcaster;

  double x = 0.0;   //first position
  double y = 0.0;
  double th = 0.0;

  ROS_INFO("asdfasdf");



  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(10.0);    //10hz

  while(n.ok()){
    ros::spinOnce();

    double vx = msg_x;      // velocity of now
    double vy = msg_y;
    double vth = msg_th;

    current_time = ros::Time::now();

    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    odom_broadcaster.sendTransform(odom_trans);

    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();


    ROS_INFO("Hello my bro!");



  }


  ROS_INFO("Hello world!");
}

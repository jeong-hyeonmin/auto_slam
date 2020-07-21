#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ss_tf_publisher");
  ros::NodeHandle n;


  ros::Rate r(10);

  tf::TransformBroadcaster broadcaster;
  tf::Transform transform;




  while (n.ok()) {

    /*
    transform.setOrigin( tf::Vector3(0.0,0.0,1.0));
    transform.setRotation(tf::Quaternion(0,0,0,1));
    broadcaster.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link","laser"));

    r.sleep();
*/

    broadcaster.sendTransform(
          tf::StampedTransform(
            tf::Transform(
              tf::Quaternion(0,0,0,1),
              tf::Vector3(0.0,0.0,0.01)),
              ros::Time::now(),
              "base_footprint",
              "base_link"));

    broadcaster.sendTransform(
          tf::StampedTransform(
            tf::Transform(
              tf::Quaternion(0,0,0,1),
              tf::Vector3(0.0,0.0,0.01)),
              ros::Time::now(),
              "base_link",
              "laser"));


    r.sleep();


/*
    broadcaster.sendTransform(
          tf::StampedTransform(
            tf::Transform(tf::Quaternion(0,0,0,1), tf::Vector3(0.1,0.0,0.2)),
            ros::Time::now(),"base_link","base_laser"));

    r.sleep();


    */
  }
}

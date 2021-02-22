#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "final_assignment/random.h"
#include "final_assignment/manual_goal.h"
#include "std_srvs/SetBool.h"
using namespace nav_msgs;
using namespace std;
ros::Publisher pub_goal;
ros::Subscriber sub_position,sub_manual_goal;
ros::ServiceClient client_random,client_wall,client_manual_goal;
move_base_msgs::MoveBaseActionGoal goal;
nav_msgs::Odometry target;
int actual_choice=8,previous_choice=0,finish=0;
int target_x,target_y;
double distance_x,distance_y;
void chatterCallback ( const Odometry:: Ptr& ptr);
int main(int argc, char **argv)
{
  ros::init(argc, argv, "computer");
  ros:: NodeHandle n_random,n_wall,n_goal,n_position,n_manual_goal,n_go_to_point;
  ros::param::set("/state", 4);
  pub_goal= n_goal.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
  client_random= n_random.serviceClient<final_assignment::random>("random");
  client_wall= n_wall.serviceClient<std_srvs::SetBool>("/wall_follower_switch");
  sub_position= n_position.subscribe("odom", 1000, chatterCallback);
  client_manual_goal= n_manual_goal.serviceClient<final_assignment::manual_goal>("manual_goal");
  do{
    ros :: spinOnce();
  }while(finish==0);
  return 0;
}



void chatterCallback ( const Odometry:: Ptr& ptr){
  //final_assignment::wall_follow srv_wall;
  std_srvs::SetBool srv_wall;
  final_assignment::manual_goal srv_manual_goal;
  final_assignment::random srv_random;
  previous_choice=actual_choice;
  ros::param::get("/state", actual_choice);

  if(actual_choice==previous_choice){
    //choice not changed
    if(actual_choice==2){
      distance_x=target.pose.pose.position.x-ptr->pose.pose.position.x;
      distance_y=target.pose.pose.position.y-ptr->pose.pose.position.y;

      if((distance_x>=(-1.0) && distance_x<=1.0) && (distance_y>=-1.0 && distance_y<=1))
      {
        ROS_INFO("Target Reached");
        ros::param::set("/state", 4);
      }
    }
  }

  else {
    cout<<endl<<"Actual choice:"<<actual_choice<<endl;

    if(previous_choice==1){

    }

    if(previous_choice==2){

    }

    if(previous_choice==3){
      //stop following the wall
      ROS_INFO("Stop following the wall\n");
      srv_wall.request.data=false;
      client_wall.call(srv_wall);
}

    /*starting the new action*/

    if (actual_choice==1) {
      //random target
      srv_random.request.min=0;
      srv_random.request.max=5;
      client_random.call(srv_random);

      ROS_INFO("Request completed, random response:%f %f",srv_random.response.a,srv_random.response.b);
      goal.goal.target_pose.pose.position.x=srv_random.response.a;
      goal.goal.target_pose.pose.position.y=srv_random.response.b;
      goal.goal.target_pose.header.frame_id="map";
      goal.goal.target_pose.pose.orientation.w=1;
      ROS_INFO("Reaching the new goal\n");
      pub_goal.publish(goal);
    }

    else if (actual_choice==2) {
      ROS_INFO("Manually setting a new goal");
      srv_manual_goal.request.pos_x=ptr->pose.pose.position.x;
      srv_manual_goal.request.pos_y=ptr->pose.pose.position.y;
      client_manual_goal.call(srv_manual_goal);
      ROS_INFO("Response received from server manual goal\nActual position:\nx:%ld\ny:%ld\n",srv_manual_goal.response.x,srv_manual_goal.response.y);

      goal.goal.target_pose.header.frame_id="map";
      goal.goal.target_pose.pose.orientation.w=1;
      goal.goal.target_pose.pose.position.x=srv_manual_goal.response.x;
      goal.goal.target_pose.pose.position.y=srv_manual_goal.response.y;
      target.pose.pose.position.x=goal.goal.target_pose.pose.position.x;
      target.pose.pose.position.y=goal.goal.target_pose.pose.position.y;
      pub_goal.publish(goal);

      ROS_INFO("Reaching the new goal\n");
    }

    else if (actual_choice==3){
      ROS_INFO("Starting following the wall\n");
      srv_wall.request.data=true;
      client_wall.call(srv_wall);
    }
    else if (actual_choice==4 || actual_choice==0) {
      ROS_INFO("System stopped");
      goal.goal.target_pose.pose.position.x=ptr->pose.pose.position.x;
      goal.goal.target_pose.pose.position.y=ptr->pose.pose.position.y;
      goal.goal.target_pose.header.frame_id="map";
      goal.goal.target_pose.pose.orientation.w=1;
      pub_goal.publish(goal);
      if(actual_choice==0){

        ROS_INFO("System closed\n");
        finish=1;
      }

    }

  }
}

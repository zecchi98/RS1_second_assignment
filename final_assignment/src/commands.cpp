#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "final_assignment/manual_goal.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
using namespace nav_msgs;
void choice_to_position(int choice,int* x, int* y);
bool void_manual_goal(final_assignment::manual_goal::Request &req, final_assignment::manual_goal::Response &res);
int main(int argc, char **argv)
{

  char buffer[10];
  int nfds,fd_ready;
  struct timeval tv;
  fd_set rfds;
  tv.tv_sec=0;
  tv.tv_usec=100;



  ros::init(argc, argv, "commands");
  ros::Publisher pub_manual_goal;
  ros:: NodeHandle n_manual_goal;
  ros::ServiceServer service=n_manual_goal.advertiseService("/manual_goal", void_manual_goal);
  Odometry:: Ptr manual_goal;
  int state=4;

  ros::param::set("/state", state);
  do{
    system("clear");
    printf("0) Exit\n1) Set a random goal\n2) Manually set a goal\n3) Follow the wall\n4) Stop the robot \n");
    nfds=0+1;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    fd_ready=select(nfds,&rfds,NULL,NULL,&tv);

    if(FD_ISSET(0,&rfds))
    {

      bzero(buffer,3);
      read(0,buffer,2);
      state=atoi(buffer);
      ros::param::set("/state", state);

    }
    ros::spinOnce();
  }while(state!=0);
  return 0;
}

void choice_to_position(int choice,int* x, int* y){
  //this function will transform the numeric choice made into the real integer choice
  if(choice==1){
    *x=-4;
    *y=-3;
  }
  if(choice==2){
    *x=-4;
    *y=2;
  }
  if(choice==3){
    *x=-4;
    *y=7;
  }
  if(choice==4){
    *x=5;
    *y=-7;
  }
  if(choice==5){
    *x=5;
    *y=-3;
  }
  if(choice==6){
    *x=5;
    *y=-1;
  }
}

bool void_manual_goal(final_assignment::manual_goal::Request &req, final_assignment::manual_goal::Response &res){
  int choice,x,y;
  do{
    printf("\nRequest for setting a goal received\nActual position:\nx:%f\ny:%f\n",req.pos_x,req.pos_y);
    printf("1)(-4;-3)\n2)(-4;2)\n3)(-4;7)\n4)(5;-7)\n5)(5;-3)\n6)(5;-1)\n");
    printf("\n\nInsert here the correspondent number of your choice:\n choice: ");
    scanf("%d",&choice);
  }while(choice<1 || choice>6);
  choice_to_position(choice,&x,&y);
  res.x=x;
  res.y=y;
  return true;

}

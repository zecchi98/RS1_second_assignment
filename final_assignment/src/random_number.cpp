#include "ros/ros.h"
#include "std_msgs/String.h"
#include "final_assignment/random.h"


int randMToN(int M, int N);
bool myrandom(final_assignment::random::Request &req, final_assignment::random::Response &res);

int main(int argc, char **argv)
{
 	ros::init(argc, argv, "random_number");
 	ros::NodeHandle n1;
 	ros::ServiceServer service=n1.advertiseService("/random", myrandom); 
 	ros::spin();

 	return 0;

}

int randMToN(int M, int N)
{
	return M+(rand() / (RAND_MAX / (N-M) ) ) ;
}

bool myrandom(final_assignment::random::Request &req, final_assignment::random::Response &res)
{	int index;
	int position_x[6]={-4,-4,-4,5,5,5};
	int position_y[6]={7,2,-3,1,-3,-7 };
	index=randMToN(req.min, req.max);
	res.a=position_x[index];
	res.b=position_y[index];
	ROS_INFO("Random Target [%f, %f]", res.a, res.b);
	return true;
}

#RUN
How to run the package?
- roslaunch final_assignment move_base
- rosrun final_assignment commands
- rosrun final_assignment computer

#PACKAGE
How is the package composed?
The package is composed of 4 nodes and 3 servers.
NODES:
- commands
- computer
- random_number
- wall_follower

SERVERS:
- random
- manual_goal
- wall_follower_switch

How the package works?

By running the node commands it will be possible to choose the kind of action that the robot will performed. Thanks to the select() system call and a timeout, the processor will decide if an action has been requested from the keyboard. In case the timeout expires, the process will check for a service request, in this way the reading will be not blocking.
There will be 5 different possible choices:
1)random target
2)choose a target
3)follow the wall
4)stop
0)close.
The "computer" node will be the one that execute all the action, while "commands" will update the "state" parameter to comunicate with it.

Choice 1:
the "state" parameter is updated, "computer" understands the update and ask for a random choice to the "random_number" node. Then the movement is performed by "computer".

Choice 2:
the "state" parameter is updated, "computer" understands the update and ask for a specific position. This is performed by a service request to "commands" that can receive it thanks to the "select" system call. Through the keyboard it will then be possible to insert the new choice and the response will be given back to the computer who will complete the action.

Choice 3:
the "state" parameter is updated, "computer" understands the update and use the "wall_follower_switch" service to start following the wall

Choice 4:
the "state" parameter is updated, "computer" understands the update and stop the robot in the actual position. 

Choice 0:
the "state" parameter is updated, "computer" understands the update and stop the robot in the actual position and close the process.

#WHY THIS CHOICE
I choose to create this two kind of main nodes in order to sort of rapresents a keyboard/joystick/input node and an output node.
I choose to insert the "manual_goal" service inside the "commands.cpp" file just to limitate the number of nodes. But it would be obviously possible to insert it also inside another cpp file.

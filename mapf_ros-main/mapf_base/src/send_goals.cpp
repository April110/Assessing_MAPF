#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/String.h"
#include "mapf_msgs/GlobalPlan.h"
#include "mapf_msgs/SinglePlan.h"
#include "nav_msgs/Path.h"


#include <sstream>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void sub_callback(const mapf_msgs::GlobalPlan &msg_plan) {
    // mapf_msgs/GlobalPlan
    // Create goals
     // Create action client
    MoveBaseClient ac_0("/rb_0/move_base", true);
    while(!ac_0.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the rb_0/move_base/goal action server to come up");
    }

    MoveBaseClient ac_1("/rb_1/move_base", true);
    while(!ac_1.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the rb_1/move_base/goal action server to come up");
    }

    move_base_msgs::MoveBaseGoal goal_0, goal_1;
    for (int i = 0; i < msg_plan.makespan; i++) {
        goal_0.target_pose.header.frame_id = "map";
        goal_0.target_pose.header.stamp = ros::Time::now();
        goal_0.target_pose.pose = msg_plan.global_plan[0].plan.poses[i].pose;

        goal_1.target_pose.header.frame_id = "map";
        goal_1.target_pose.header.stamp = ros::Time::now();
        goal_1.target_pose.pose = msg_plan.global_plan[1].plan.poses[i].pose;

        
        // Send goal 1
        ROS_INFO("================== Sending Goal 0 ================= makespan %d", i);

        ac_0.sendGoal(goal_0);
        if(ac_0.waitForResult() == true){
             ROS_INFO("================== Goal 0 Arrived ================= makespan %d", i);

        };
       
        // Send goal 2
        ROS_INFO("================== Sending Goal 1 ================= makespan %d", i);
        ac_1.sendGoal(goal_1);
        if(ac_1.waitForResult()){
            ROS_INFO("================== Goal 1 Arrived ================= makespan %d", i);
        }
        
    }
    
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "send_goal_client");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/mapf_base/global_plan", 1000, sub_callback);
    ros::spin();

    return 0;
}
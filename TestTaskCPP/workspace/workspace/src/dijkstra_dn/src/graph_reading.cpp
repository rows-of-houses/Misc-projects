#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <ros/package.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>


int main(int argc, char **argv) {
    std::ifstream in;
    in.open("/home/dmitry/ros_projects/p_3/workspace/graph.txt");
    if (!in.is_open())
        ROS_INFO("Unable to open the file");
    int lin = 0;
    char c;
    while (in.get(c)) {
        if (c == '\n')
            ++lin;
    }
    int v_size = lin * lin;
    in.clear();
    in.seekg(0);

    ros::init(argc, argv, "publisher");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<std_msgs::Int32MultiArray>("vertices", 1);
    ros::Rate loop_rate(1);
    std_msgs::Int32MultiArray dat;

    int * a = new int[v_size];
    for (int i = 0; i < v_size; ++i)
        in >> a[i];

    dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
    dat.layout.dim[0].label = "vertices_array";
    dat.layout.dim[0].size = v_size;
    dat.layout.dim[0].stride = 1;
    dat.layout.data_offset = 0;

    std::vector<int> vec(v_size, 0);
    for (int i = 0; i < v_size; ++i)
        vec[i] = a[i];
    dat.data = vec;
    
    //for (int i = 0; i < v_size; ++i)
       // ROS_INFO("%d", dat.data[i]);

    delete [] a;
    in.close();
    while (ros::ok()) {
	if (pub.getNumSubscribers() != 0) {
	    //ROS_INFO("subscriber number: %d ", pub.getNumSubscribers());
            pub.publish(dat); 
            ROS_INFO("Published");
            return 0;
        }          
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

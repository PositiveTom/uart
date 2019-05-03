#include "ros/ros.h"
#include "boost/bind.hpp"
#include "boost/asio.hpp"
#include <vector>
#include "stdio.h"
unsigned char data[8];//4位16进制数基本可以表示任何数据，最大的数为65535
// std::vector<unsigned int> data(8);
unsigned char send_data[8];

unsigned char core[1] = {0xfe};
void data_protocol(unsigned char* data)//定义协议
{
    data[0] = 0xff;
    data[1] = 0xfe;
    data[4] = 0xff;
    data[7] = 0xff;


    send_data[0] = 0xff;
    send_data[1] = 0xfe;
    send_data[6] = 0x0d;
    send_data[7] = 0x0a;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "serial");
    ros::NodeHandle n;
    boost::asio::io_service io_server;
    boost::asio::serial_port sp(io_server,"/dev/ttyUSB0");//虚拟串口调试成功，开始上开发板,开发板也读取成功
    sp.set_option(boost::asio::serial_port::baud_rate(115200));//设置9600波特率
    sp.set_option(boost::asio::serial_port::flow_control());
    sp.set_option(boost::asio::serial_port::parity());
    sp.set_option(boost::asio::serial_port::stop_bits());
    sp.set_option(boost::asio::serial_port::character_size(8));

    ROS_INFO("hello");
    ros::Rate loop_rate(10);
    data_protocol(data);

    while(ros::ok())
    {
        //bool a = boost::asio::read(sp,boost::asio::buffer(data));//读取成功返回1,否在返回0

        bool b = boost::asio::write(sp,boost::asio::buffer(core));//发送数据成功，接下来要做的工作就是封装下位机,这些所有工作都要移植到RT1052上
        // if(a)
        // {
        //     ROS_INFO("copy that");
        //     for(int i=0; i<8; i++)
        //     {
        //         ROS_INFO("data[%d]:%x",i,data[i]);
        //         // printf("data[%d]:%x",i,data[i]);
        //     }
        // }
        // else
        // {
        //     ROS_INFO("wait");
        // }

        ros::spinOnce();
        loop_rate.sleep();
    }

    // ros::spin();
    return 0;
}
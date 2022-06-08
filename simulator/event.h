#pragma once
#include"net.h"
#include"random.h"
struct event						//定义事件结构体
{
	std::string type;
	double time;
	Mac source_mac;
	Mac destination_mac;
	int size;
	bool operator < (const event& b)const {
		return time > b.time;		//直接重载操作符生成小根堆
	}
	//初始化函数，定义事件发生事件t，源sou，目的地des，数据大小sz，类型tp
	event(double t, Mac sou, Mac des, int sz, std::string tp);	
};

class simulator {
public:
	NodeContainer my_node;							//定义节点
	Channel my_channel;								//定义信道
	//Random_creater my_random;
	double time = 0;								//定义开始时间
	double frame_time = 0;							//定义帧时
	double end_time = 0;							//定义结束时间
	std::priority_queue <event> class_B_events;		//B类事件列表
	std::vector <event> class_C_events;

	simulator(NodeContainer node, Channel channel);	//初始化
	void end(double end);							//设定结束时间
	void event_frame_init_ins(Mac source, Mac destination, int size, double event_time);	
													//插入事件，源mac，目标mac，数据包大小，发生时间
	void start();									//开始模拟
	void event_handler(event tmp);					//事件处理
	void channel_check();							//信道检测
													//事件初始化，源id，目标id，发送数据包的个数n，时间间隔随机数生成，数据包大小随机数生成
	void event_init(int source_id, int des_id, int n, Random_creater time_random, Random_creater datasize_random);

	int tot_send_cnt = 0;							//用于统计模拟过程中总共发送的数据帧个数
	int failed_send_cnt = 0;						//统计发送失败的数据帧个数
};
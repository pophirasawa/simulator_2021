#pragma once
#include<vector>
#include<map>
#include<queue>
#include<string>
struct Mac							//定义Mac类型
{
	int dat[12] = {};				//存储12位16进制数字
	std::string str;
	std::string to_string();		//把mac转为字符串显示
	bool operator < (const Mac &b) {//操作符重载，用于map
		return str < b.str;
	}
};

struct Frame {						//定义数据帧
	Mac source;						//源mac
	Mac destination;				//目标mac
	int size;						//帧大小
};

struct Channel {
	int rate;						//信道带宽
	bool jamed = 0;					//是否冲突
	std::vector<Frame> buffer;		//同时刻所有在信道发送的帧队列
	Channel(int my_rate = 0);
};

class Node {						//定义网络节点
public:
	int id;							//id
	Mac mac;						//节点mac地址
	std::vector<Frame> up_to_send;	//节点待发帧队列
	std::vector<Frame> up_to_resend;//节点待重发帧队列
	int const_jamed_counter;		//连续冲突计数
	int jamed_wait_time;			//冲突后应该等待的帧时个数
	int frame_size;					//帧大小
					
	Node(int my_id, int my_frame_size, Mac my_mac);

	bool want_to_send(void);					//是否有待发的数据帧
	void send_init(Mac destination, int size);	//想要发送数据包时，分为多个数据帧，加入待发队列
	bool send(Channel &my_channel);				//往信道发送
	void failed_to_send(Frame failed_frame);	//发送失败时的反应
	void succsee_to_send();						//成功发送时的反应
	Frame frame_to_send();						//查询此时应该发送的数据帧

};

Mac set_mac_random(void);						//mac初始化函数，随机设定mac地址
Mac set_mac_ordered(void);						//mac初始化函数，从0开始设定mac地址
struct cmp {									//定义比较函数，用于map
	bool operator()(const Mac  a,const Mac  b)const{
		return a.str < b.str;
	};
};
class NodeContainer								//节点容器
{
public:
	std::vector<Node> nodes;					//储存各个节点
	std::map<Mac, int,cmp> table;				//map用于通过mac快速查询节点id

	NodeContainer(int size = 1, int my_frame_size = 1, Mac(*setMac)(void) = set_mac_random);
};



#include"event.h"
#include<iostream>


event::event(double t, Mac sou, Mac des, int sz, std::string tp) {
	time = t;
	source_mac = sou;
	destination_mac = des;
	size = sz;
	type = tp;
}

simulator::simulator(NodeContainer node, Channel channel) {
	my_channel = channel;
	my_node = node;
	frame_time = 1.0*node.nodes[0].frame_size / my_channel.rate;
}

void simulator::end(double end) {
	end_time = end;
}

void simulator::event_frame_init_ins(Mac source, Mac destination, int size, double event_time) {
	class_B_events.push(event(event_time,source,destination,size,"send_init"));
}

void simulator::event_handler(event tmp) {
	//处理事件，通过event类型中的type判断
	if (tmp.type == "send_init") {
		//如果事件为sendinit，调用源id对应函数
		int id = my_node.table[tmp.source_mac];
		my_node.nodes[id].send_init(tmp.destination_mac, tmp.size);


		//输出log
		std::cout << "time: " << tmp.time<<"	";
		std::cout << tmp.source_mac.str << "-->" << tmp.destination_mac.str << "	";
		std::cout << "生成了大小为:" << tmp.size << " 的数据包\n";
	}
	else if (tmp.type == "send") {
		//尝试发送
		int id = my_node.table[tmp.source_mac];
		int flg=my_node.nodes[id].send(my_channel);

		if(flg)tot_send_cnt++;
	}
}

void simulator::channel_check(){
	//检测当前时间的信道状态
	if (my_channel.buffer.size() == 0)return;
	else if (my_channel.buffer.size() == 1) {	//若只有1个数据包，成功发送	
		Mac tmp = my_channel.buffer[0].source;
		int id = my_node.table[tmp];
		my_node.nodes[id].succsee_to_send();

		//log
		std::cout << "[send success]    ";
		std::cout << "time: " << time << "	";
		std::cout << tmp.str << "-->" << my_channel.buffer[0].destination.str << "	";
		std::cout << "成功发送了大小为" << my_channel.buffer[0].size << "bytes的数据包\n";

		my_channel.buffer.clear();
	}
	else {
		for (auto& i : my_channel.buffer) {		//若有多个数据包，调用发送失败函数
			Frame tmp = i;
			int id = my_node.table[i.source];
			my_node.nodes[id].failed_to_send(i);

			failed_send_cnt++;

			std::cout << "[send  failed]    ";
			std::cout << "time: " << time << "	";
			std::cout << i.source.str << "-->" <<i.destination.str << "	";
			std::cout << "信道冲突，未能成功发送大小" << my_channel.buffer[0].size << "bytes的数据包";
			std::cout << " 将在" << my_node.nodes[id].jamed_wait_time << "个时隙后重传\n";
		}
		my_channel.buffer.clear();
	}
}

void simulator::start() {
	double time_slot = 0;		//定义时隙时间
	while (1) {
								//处于时隙时间起点时
		for (auto &i:  my_node.nodes) {		//遍历所有节点，如果有节点想要发送就加入B类事件列表
			if (i.want_to_send()) {
				Frame tmp = i.frame_to_send();
				class_B_events.push(event(time_slot, tmp.source, tmp.destination, tmp.size,"send"));
			}
		}
		if (class_B_events.empty())break;	//如果B类事件列表为空，结束
		while (!class_B_events.empty()) {	//遍历B类事件列表，如果有事件发生事件在时隙起点到下个时隙起点的间隔中，处理事件
			event tmp = class_B_events.top();
			if (tmp.time < time_slot + frame_time) {
				time = tmp.time;
				event_handler(tmp);
				class_B_events.pop();
			}
			else break;
		}
		channel_check();					//检查信道状态
		time_slot = time_slot + frame_time;	//到达下一个时隙起点
		if (time > end_time)break;			
	}

	std::cout << "一共发送了: " << tot_send_cnt << "个数据帧\n";
	std::cout << "发送失败了：" << failed_send_cnt << "个数据帧\n";
}

void simulator::event_init(int source_id, int des_id, int n, Random_creater time_random, Random_creater datasize_random) {
	double tmpt = 0;
	for (int i = 0; i < n; i++) {
		//时间间隔生成
		tmpt += abs(time_random.random_normal_real());
		//数据包大小生成
		int sz = abs(datasize_random.random_normal_int());
		event_frame_init_ins(my_node.nodes[source_id].mac, my_node.nodes[des_id].mac, sz, tmpt);
	}
}
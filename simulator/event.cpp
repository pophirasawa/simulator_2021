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
	//�����¼���ͨ��event�����е�type�ж�
	if (tmp.type == "send_init") {
		//����¼�Ϊsendinit������Դid��Ӧ����
		int id = my_node.table[tmp.source_mac];
		my_node.nodes[id].send_init(tmp.destination_mac, tmp.size);


		//���log
		std::cout << "time: " << tmp.time<<"	";
		std::cout << tmp.source_mac.str << "-->" << tmp.destination_mac.str << "	";
		std::cout << "�����˴�СΪ:" << tmp.size << " �����ݰ�\n";
	}
	else if (tmp.type == "send") {
		//���Է���
		int id = my_node.table[tmp.source_mac];
		int flg=my_node.nodes[id].send(my_channel);

		if(flg)tot_send_cnt++;
	}
}

void simulator::channel_check(){
	//��⵱ǰʱ����ŵ�״̬
	if (my_channel.buffer.size() == 0)return;
	else if (my_channel.buffer.size() == 1) {	//��ֻ��1�����ݰ����ɹ�����	
		Mac tmp = my_channel.buffer[0].source;
		int id = my_node.table[tmp];
		my_node.nodes[id].succsee_to_send();

		//log
		std::cout << "[send success]    ";
		std::cout << "time: " << time << "	";
		std::cout << tmp.str << "-->" << my_channel.buffer[0].destination.str << "	";
		std::cout << "�ɹ������˴�СΪ" << my_channel.buffer[0].size << "bytes�����ݰ�\n";

		my_channel.buffer.clear();
	}
	else {
		for (auto& i : my_channel.buffer) {		//���ж�����ݰ������÷���ʧ�ܺ���
			Frame tmp = i;
			int id = my_node.table[i.source];
			my_node.nodes[id].failed_to_send(i);

			failed_send_cnt++;

			std::cout << "[send  failed]    ";
			std::cout << "time: " << time << "	";
			std::cout << i.source.str << "-->" <<i.destination.str << "	";
			std::cout << "�ŵ���ͻ��δ�ܳɹ����ʹ�С" << my_channel.buffer[0].size << "bytes�����ݰ�";
			std::cout << " ����" << my_node.nodes[id].jamed_wait_time << "��ʱ϶���ش�\n";
		}
		my_channel.buffer.clear();
	}
}

void simulator::start() {
	double time_slot = 0;		//����ʱ϶ʱ��
	while (1) {
								//����ʱ϶ʱ�����ʱ
		for (auto &i:  my_node.nodes) {		//�������нڵ㣬����нڵ���Ҫ���;ͼ���B���¼��б�
			if (i.want_to_send()) {
				Frame tmp = i.frame_to_send();
				class_B_events.push(event(time_slot, tmp.source, tmp.destination, tmp.size,"send"));
			}
		}
		if (class_B_events.empty())break;	//���B���¼��б�Ϊ�գ�����
		while (!class_B_events.empty()) {	//����B���¼��б�������¼������¼���ʱ϶��㵽�¸�ʱ϶���ļ���У������¼�
			event tmp = class_B_events.top();
			if (tmp.time < time_slot + frame_time) {
				time = tmp.time;
				event_handler(tmp);
				class_B_events.pop();
			}
			else break;
		}
		channel_check();					//����ŵ�״̬
		time_slot = time_slot + frame_time;	//������һ��ʱ϶���
		if (time > end_time)break;			
	}

	std::cout << "һ��������: " << tot_send_cnt << "������֡\n";
	std::cout << "����ʧ���ˣ�" << failed_send_cnt << "������֡\n";
}

void simulator::event_init(int source_id, int des_id, int n, Random_creater time_random, Random_creater datasize_random) {
	double tmpt = 0;
	for (int i = 0; i < n; i++) {
		//ʱ��������
		tmpt += abs(time_random.random_normal_real());
		//���ݰ���С����
		int sz = abs(datasize_random.random_normal_int());
		event_frame_init_ins(my_node.nodes[source_id].mac, my_node.nodes[des_id].mac, sz, tmpt);
	}
}
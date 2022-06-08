#include "net.h"
#include<time.h>
Node::Node(int my_id, int my_frame_size,Mac my_mac){
	id = my_id;
	mac = my_mac;
	frame_size = my_frame_size;
	const_jamed_counter = 0;
	jamed_wait_time = 0;
}

bool Node::want_to_send() {
	//�����Ƿ�Ҫ����
	return (!up_to_send.empty() || !up_to_resend.empty());
}

void Node::send_init(Mac destination, int size) {
	//�����ݰ���Ϊ��С�̶�������֡������������б���
	while (size > frame_size) {
		up_to_send.push_back(Frame{ mac,destination,frame_size });
		size -= frame_size;
	}
	up_to_send.push_back(Frame{ mac,destination,size });
}

bool Node::send(Channel& my_channel) {
	//����Ӧ�ŵ���������֡�������ǰ����ײ��ʱ���򲻷�������֡
	if (!up_to_resend.empty()) {
		if (jamed_wait_time) {
			jamed_wait_time--;
			return 0;
		}
		my_channel.buffer.push_back(up_to_resend[0]);
		up_to_resend.pop_back();
	}
	else {
		my_channel.buffer.push_back(up_to_send[0]);
		up_to_send.erase(up_to_send.begin());
	}
	return 1;
}

void Node::failed_to_send(Frame failed_frame) {
	//����ʧ�ܺ󣬰�ʧ������֡����ȴ����У�������++������ȴ�ʱ�䣬Ϊһ�����
	const_jamed_counter++;
	jamed_wait_time = rand() % (1 << (const_jamed_counter)) + failed_frame.source.dat[0];
	up_to_resend.push_back(failed_frame);
}

void Node::succsee_to_send() {
	//���ͳɹ���������ײ����
	const_jamed_counter = 0;
}

Frame Node::frame_to_send() {
	//����Ҫ���͵�����֡
	Frame tmp;
	tmp.size = -1;
	if (!up_to_resend.empty())return up_to_resend[0];
	else if (!up_to_send.empty())
		return up_to_send[0];
	else return tmp;
}
std::string Mac::to_string() {
	//��mac��ַ��Ϊ�ַ���
	std::string res;
	for (int i = 0; i < 12; i++) {
		if (dat[i] < 10)res += '0' + dat[i];
		else res += dat[i] - 10 + 'A';
		if (i == 1 || i == 3 || i == 5 || i == 7 || i == 9)res += ':';
	}
	str = res;
	return res;
}
NodeContainer::NodeContainer(int size, int my_frame_size,Mac (*setMac)(void)) {
	for (int i = 0; i < size; i++) {
		Mac tmp = setMac();
		nodes.push_back(Node(i,my_frame_size ,tmp));
		table[tmp] = i;
	}
}

Mac set_mac_random(void) {
	Mac res;
	for (int i = 0; i < 12; i++) {
		res.dat[i] = rand() % 16;
	}
	res.to_string();
	return res;
}

Mac set_mac_ordered(void) {
	static int cnt=0;
	int tmp = cnt;
	Mac res;
	for (int i = 11; i >= 0; i--) {
		res.dat[i] = tmp % 16;
		tmp /= 16;
	}
	cnt++;
	res.to_string();
	return res;
}

Channel::Channel(int my_rate) {
	rate = my_rate;
}
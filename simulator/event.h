#pragma once
#include"net.h"
#include"random.h"
struct event						//�����¼��ṹ��
{
	std::string type;
	double time;
	Mac source_mac;
	Mac destination_mac;
	int size;
	bool operator < (const event& b)const {
		return time > b.time;		//ֱ�����ز���������С����
	}
	//��ʼ�������������¼������¼�t��Դsou��Ŀ�ĵ�des�����ݴ�Сsz������tp
	event(double t, Mac sou, Mac des, int sz, std::string tp);	
};

class simulator {
public:
	NodeContainer my_node;							//����ڵ�
	Channel my_channel;								//�����ŵ�
	//Random_creater my_random;
	double time = 0;								//���忪ʼʱ��
	double frame_time = 0;							//����֡ʱ
	double end_time = 0;							//�������ʱ��
	std::priority_queue <event> class_B_events;		//B���¼��б�
	std::vector <event> class_C_events;

	simulator(NodeContainer node, Channel channel);	//��ʼ��
	void end(double end);							//�趨����ʱ��
	void event_frame_init_ins(Mac source, Mac destination, int size, double event_time);	
													//�����¼���Դmac��Ŀ��mac�����ݰ���С������ʱ��
	void start();									//��ʼģ��
	void event_handler(event tmp);					//�¼�����
	void channel_check();							//�ŵ����
													//�¼���ʼ����Դid��Ŀ��id���������ݰ��ĸ���n��ʱ������������ɣ����ݰ���С���������
	void event_init(int source_id, int des_id, int n, Random_creater time_random, Random_creater datasize_random);

	int tot_send_cnt = 0;							//����ͳ��ģ��������ܹ����͵�����֡����
	int failed_send_cnt = 0;						//ͳ�Ʒ���ʧ�ܵ�����֡����
};
#pragma once
#include<vector>
#include<map>
#include<queue>
#include<string>
struct Mac							//����Mac����
{
	int dat[12] = {};				//�洢12λ16��������
	std::string str;
	std::string to_string();		//��macתΪ�ַ�����ʾ
	bool operator < (const Mac &b) {//���������أ�����map
		return str < b.str;
	}
};

struct Frame {						//��������֡
	Mac source;						//Դmac
	Mac destination;				//Ŀ��mac
	int size;						//֡��С
};

struct Channel {
	int rate;						//�ŵ�����
	bool jamed = 0;					//�Ƿ��ͻ
	std::vector<Frame> buffer;		//ͬʱ���������ŵ����͵�֡����
	Channel(int my_rate = 0);
};

class Node {						//��������ڵ�
public:
	int id;							//id
	Mac mac;						//�ڵ�mac��ַ
	std::vector<Frame> up_to_send;	//�ڵ����֡����
	std::vector<Frame> up_to_resend;//�ڵ���ط�֡����
	int const_jamed_counter;		//������ͻ����
	int jamed_wait_time;			//��ͻ��Ӧ�õȴ���֡ʱ����
	int frame_size;					//֡��С
					
	Node(int my_id, int my_frame_size, Mac my_mac);

	bool want_to_send(void);					//�Ƿ��д���������֡
	void send_init(Mac destination, int size);	//��Ҫ�������ݰ�ʱ����Ϊ�������֡�������������
	bool send(Channel &my_channel);				//���ŵ�����
	void failed_to_send(Frame failed_frame);	//����ʧ��ʱ�ķ�Ӧ
	void succsee_to_send();						//�ɹ�����ʱ�ķ�Ӧ
	Frame frame_to_send();						//��ѯ��ʱӦ�÷��͵�����֡

};

Mac set_mac_random(void);						//mac��ʼ������������趨mac��ַ
Mac set_mac_ordered(void);						//mac��ʼ����������0��ʼ�趨mac��ַ
struct cmp {									//����ȽϺ���������map
	bool operator()(const Mac  a,const Mac  b)const{
		return a.str < b.str;
	};
};
class NodeContainer								//�ڵ�����
{
public:
	std::vector<Node> nodes;					//��������ڵ�
	std::map<Mac, int,cmp> table;				//map����ͨ��mac���ٲ�ѯ�ڵ�id

	NodeContainer(int size = 1, int my_frame_size = 1, Mac(*setMac)(void) = set_mac_random);
};



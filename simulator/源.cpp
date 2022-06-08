#include<iostream>
#include"net.h"
#include"event.h"
using namespace std;
int main() {
//	Mac a, b;
	//a.str = "1";
	//b.str = "2";
	//cout << (a < b);

	srand(time(0));

	NodeContainer nodes(5000,50,set_mac_random);
	Channel my_channel(100);
	//随机数种子
	Random_creater a;
	Random_creater b;
	Random_creater c;
	Random_creater d;
	a.average = 2, a.variance = 0.5;
	b.average = 50, b.variance = 1;
	//cout<<nodes.nodes[0].mac.to_string();
	simulator S(nodes,my_channel);

	for (int i = 1; i <=1000; i++) {
		Random_creater c, d;
		c.average = a.average, c.variance = a.variance;
		d.average = b.average, d.variance = b.variance;
		S.event_init(i, i + 1, 1,c, d);
	}
   // S.event_init(1, 2, 20, a, b);
	//S.event_init(2, 3, 20, c, d);
	S.end(100000000);
	S.start();
}
#pragma once
#include<cmath>
#include<time.h>
#include<random>
class Random_creater {
public:
	Random_creater();
	//ͳһ���������б�����
	double random_uniform_int();			//���ؾ��ȷֲ�����
	double random_uniform_real();			//���ؾ��ȷֲ�С��
	double random_normal_int();				//������̬�ֲ�����
	double random_normal_real();			//������̬�ֲ�С��
	double random_pareto_real();			//����pareto�ֲ�С��
	double random_const();					//����my_const
	
											//����Ϊ����
	double average, variance;				
	double my_const;
	double max, min;
	double a, b;
private:
	int seed;
	std::default_random_engine e;			//�������������
};
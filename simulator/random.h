#pragma once
#include<cmath>
#include<time.h>
#include<random>
class Random_creater {
public:
	Random_creater();
	//统一函数参数列表类型
	double random_uniform_int();			//返回均匀分布整数
	double random_uniform_real();			//返回均匀分布小数
	double random_normal_int();				//返回正态分布整数
	double random_normal_real();			//返回正态分布小数
	double random_pareto_real();			//返回pareto分布小数
	double random_const();					//返回my_const
	
											//以下为参数
	double average, variance;				
	double my_const;
	double max, min;
	double a, b;
private:
	int seed;
	std::default_random_engine e;			//随机数生成引擎
};
#include"random.h"

Random_creater::Random_creater() {
	
	seed = rand();
	e = std::default_random_engine(seed);
	average = variance = my_const = max = min =a=b= 1;
}

double Random_creater::random_uniform_int() {
	int maxx = max, minn = min;
	return rand() % int(maxx - minn) + minn;
}
double Random_creater::random_uniform_real() {
	static std::uniform_real_distribution<double> u(min,max);
	return u(e);
}

double Random_creater::random_normal_int() {
	static std::normal_distribution<double> u(average, variance);
	return lround(u(e));
}

double Random_creater::random_normal_real() {
	static std::normal_distribution<double> u(average, variance);
	return u(e);
}

double Random_creater::random_pareto_real(){
	double mx = max, mn = min;
	max = 1.0, min = 0;
	double res = random_normal_real();
	max = mx, min = mn;
	res=b* pow((1 - res), (-1.0) / a);
	return res;
}

double Random_creater::random_const() {
	return my_const;
}
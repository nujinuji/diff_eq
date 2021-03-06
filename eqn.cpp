// Integrate one variable ODE using Euler's method

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>

#define ak 0.004
#define bk 0.07
#define bs 0.82
#define k0 0.2 //kg = k0 = bg
#define k1 0.222
#define n 2
#define p 5

#define eta 1e-07 //accuracy of error

using namespace std;

/**
 * define derivative functoin
 */
double f(double x, double y) {
	return 0;
}


/**
 * dK/dt
 */
double dK(double s, double k) {
	return ak + bk * pow(k, n) / (pow(k0, n) + pow(k, n)) - k / (1 + k + s);
}


/**
 * dS/dt
 */
double dS(double s, double k) {
	return bs / (1 + pow((k / k1), p)) - s / (1 + k + s);
}


/**
 * dS/dt: feedback bypass adding ComS driving promoter PComG (2nd term in eqn)
 * the value of parameter k0 = kb = bg is not initialized
 * see eqn S13 for detail
 */
double dSg(double s, double k) {
	return bs / (1 + pow((k / k1), p)) + k0 * pow(k, n)/(pow(k0, n) + pow(k, n))
				 - s / (1 + k + s);
}

double test(double k, double s) {
	return s*s + k;
}


/**
 * [x0, x1]: estimated interval of solution
 * k: param of function dK, dS
 * function pointer to use function as parameter 
 */
double solve(double(*f)(double, double), double x1, double x0, double k) {
	double d, x2; // difference
	int i = 0;
	while(1) {
		x2 = x1 - (*f)(x1, k) * (x1 - x0) / ((*f)(x1, k) - (*f)(x0, k));
		d = x1 - x0;
		//cout << "x2: " << x2 << " " << "x1: " << x1 << endl;
		//cout << "the differentce is: " << fabs(d) << endl;
		//cout << "if < eta??: " << (fabs(d) < eta) << endl;
		if (fabs(d) < eta) {
			break;
		}
		x0 = x1; // update solution
		x1 = x2;
		
		i++;

		// the functions has possibly no solution is the number of steps > 10
		if(i > 10) {
			//cout << "!!!!!!!!!!!!!!!!!!!" << endl;
			break;
		}
	}
	return x1;
}


/**
 * x0, y0: initial value
 * h: step size
 */
vector<double> Euler(double x0, double y0, double h, int end){
	vector<double> y(end, 0); // initialization of the output vector
	y[0] = y0;
	double x = x0;
	for (double i = 1; i < end; i++) {
		y[i] = y[i - 1] + h * f(x, y[i - 1]);
		x += h; // increase by step size
	}
	return y;
}

/**
 * calculations of nullclines solutions
 * TODO: add nullclines for bypass, add noise term, add [comS][comK]-t
 * plot the phase path for a solution of 2-eqn first-order ODE using verctor
 * [y1, y2]
 */
int main(int argc, char* argv[]) {
	for(int i = 0 ; i < 450; i ++) {
		double x = i * 0.001;
		double y_s = solve(dS, 0, 6, x);
		double y_k = solve(dK, 0, 0.6, x);
		cout << x << " " << y_k << " " << y_s << endl;
	}
	//double x = solve(dK, 0, 6, 0.2);
	//cout << x << endl;
	return 0;
}

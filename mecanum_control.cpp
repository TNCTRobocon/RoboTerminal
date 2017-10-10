#include "mecanum_control.hpp"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

static const double eps = 0.001;

MecanumWheel::MecanumWheel(double a,double b,double l,double g){
	if(fabs(l) < eps){
		perror("error:length is too short/");
		exit(1);
	}else if(fabs(cos(b)) < eps){
		perror("error:beta is too short/");
		exit(1);
	}else if(fabs(g) < eps){
		perror("error:gamma is too short/");
		exit(1);
	}else{
		alpha = a;
		beta = b;
		length = l;
		gamma = g;
	}
}

double MecanumWheel::Calculate(double x,double y,double r){
	double w = alpha + beta;
	double s = sin(w),c = cos(w),cc=cos(beta);
	double move,spin;;
	//compute on move
	if(fabs(s)>eps&&fabs(c)>eps){
		move=x/c+y/s;
	}else if(fabs(s)>eps){
		move=y/s;
	}else{
		move=x/c;
	}
	//compute on rotation
	spin = r/cc/length;
	
	return gamma*(move+spin);
}

void MecanumWheel::SetLength(double x){
	if (fabs(x)>eps){
		length= x;
	}else{
		perror("error:length is too short/");
		exit(1);
	}
}



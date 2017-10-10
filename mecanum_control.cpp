#include "mecanum_control.hpp"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

static const double eps = 0.001;

MecanumWheel::MecanumWheel(double a, double b, double l, double g, double s)
{
    if (fabs(l) < eps) {
        perror("error:length is too short/");
        exit(1);
    }
    if (fabs(cos(b)) < eps) {
        perror("error:beta is too short/");
        exit(1);
    }
    if (fabs(g) < eps) {
        perror("error:gamma is too short/");
        exit(1);
    }
    alpha = a;
    beta = b;
    length = l;
    gamma = g;
    size = s;
}

MecanumWheel::MecanumWheel(const MecanumWheel& orig) :
alpha(orig.alpha), beta(orig.beta), length(orig.length), gamma(orig.gamma), size(orig.gamma)
{

}

double MecanumWheel::Calculate(double x, double y, double r)
{
    const double w = alpha + beta;
    const double s = sin(w), c = cos(w), cc = cos(beta);
    double move, spin, adjust;
    //compute on move
    if (fabs(s) > eps && fabs(c) > eps) {
        move = x / c + y / s;
    } else if (fabs(s) > eps) {
        move = y / s;
    } else {
        move = x / c;
    }
    //compute on rotation
    spin = r / cc / length;
    //adjust(補正値)
    adjust = gamma / (2.0 * M_PI * size);
    //計算
    return adjust * (move + spin);
}

void MecanumWheel::SetLength(double x)
{
    if (fabs(x) > eps) {
        length = x;
    } else {
        perror("error:length is too short/");
        exit(1);
    }
}



#include <math.h>
#include "constants.h"
#include "atmosphere_constants.h"

/* Get geopotential height from distance to the center of the earth. */
double geopotential_height(double R) {
	return (M_earth * G / g0) * (1/R_earth - 1/R);
}

double air_density(double R) {
	double altitude = R - R_earth;

	/* Find out in which layer the object is. */
	double h = geopotential_height(R);
	int i;
	for(i = 0; i < NUMBER_OF_LAYERS; i++)
	{
		if(h > hb[i])
			break;
	}
	double hb_ = hb[i];
	double Lb_ = Lb[i];
	double Tb_ = Tb[i];
	double pb_ = pb[i];

	double epsilon = 1.0e-7;
	if(fabs(Lb_) < epsilon)
	{
		return pb_ * exp((-g0 * M_air * (h - hb_)) / (R_star * Tb_));
	}
	else
	{
		double base = 1 - Lb_ * (h - hb_) / Tb_;
		double degree = 1 + (g0 * M_air) / (R_star * Lb_);
		return pb_ * pow(base, degree);
	}
}

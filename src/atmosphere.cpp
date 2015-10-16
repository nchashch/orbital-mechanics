#include <cmath>
#include "constants.h"
#include "atmosphere_constants.h"

/* Get geopotential height from distance to the center of the earth. */
float geopotential_height(float R) {
	return (M_earth * G / g0) * (1/R_earth - 1/R);
}

float air_density(float R) {
	float altitude = R - R_earth;

	/* Find out in which layer the object is. */
	float h = geopotential_height(R);
	int i;
	for(i = 0; i < NUMBER_OF_LAYERS; i++)
	{
		if(h > hb[i])
			break;
	}
	float hb_ = hb[i];
	float Lb_ = Lb[i];
	float Tb_ = Tb[i];
	float pb_ = pb[i];

	float epsilon = 1.0e-7;
	if(fabs(Lb_) < epsilon)
	{
		return pb_ * exp((-g0 * M_air * (h - hb_)) / (R_star * Tb_));
	}
	else
	{
		float base = 1 - Lb_ * (h - hb_) / Tb_;
		float degree = 1 + (g0 * M_air) / (R_star * Lb_);
		return pb_ * pow(base, degree);
	}
}

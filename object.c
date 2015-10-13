#include <math.h>
#include "object.h"

active_object_t integrate_step
		(active_object_t obj,
		vector_t acc,
		double dt);

active_object_t integrate
		(active_object_t obj,
		vector_t (*force)(active_object_t obj),
		double dt, double elapsed_time)
{
	double et_floored = floor(elapsed_time);
	int steps = et_floored;
	double last_step_dt = elapsed_time - et_floored;

	int i;
	for(i = 0; i < steps; i++)
	{
		vector_t acc = vdiv(force(obj), obj.m);
		obj = integrate_step(obj, acc, dt);
	}	

	vector_t acc = vdiv(force(obj), obj.m);
	obj = integrate_step(obj, acc, last_step_dt);

	return obj;
}

active_object_t integrate_step
		(active_object_t obj,
		vector_t acc,
		double dt)
{
	vector_t v0 = obj.v;
	vector_t r0 = obj.r;

	vector_t dv = vmul(acc, dt);
	vector_t dr = vmul(v0, dt);

	vector_t v1 = vadd(v0, dv);
	vector_t r1 = vadd(r0, dr);

	obj.v = v1;
	obj.r = r1;

	return obj;
}

/*
Asumptions:
Earth behaves like a point mass (accurate to some degree)
*/

vector_t gs_r = {0.0, 0.0, 0.0, 1.0}; /* Position of the earth */
double M_earth = 5.9742e24; /* Mass of the earth */
double R_earth = 3.671e6; /* Mean radius of the earth */
double G = 6.67408e-11; /* Gravitational constant */

vector_t gravity_force(active_object_t obj)
{
	vector_t dr = vsub(gs_r, obj.r);
	double distance = length(dr);
	double mu = M_earth * G; /* Standard gravitational parameter. */
	double scalar_part = mu * obj.m / (distance*distance);
	vector_t vector_part = norm(dr);
	vector_t force = vmul(vector_part, scalar_part);
	return force;
}

vector_t thrust_force(active_object_t obj)
{
	return obj.thrust;
}

double start_altitude;

double geopotential_height(double R);
double air_density(double R);

/* Fd = 0.5 * Cd * A * p * v^2 */
/* Cd is the drag coefficient */
/* p is the air density */
/* v is the body's velocity */
/* A is area of the body, normal to the flow. */

vector_t drag_force(active_object_t obj)
{
	double R = length(obj.r); /* Distance to the center of the earth. */
	vector_t v = obj.v;
	double v_length = length(v);
	double Cd = obj.Cd;
	double A = obj.A;
	double p = air_density(R);

	double scalar_part = -0.5 * Cd * A * p * v_length;
	vector_t F = vmul(v, scalar_part);
	return F;
}

/* International Standard Atmosphere data */

/*
0 Troposphere
1 Tropopause
2 Startosphere
3 Startosphere
4 Startopause
5 Mesosphere
6 Mesosphere
7 Mesopause
*/

const int NUMBER_OF_LAYERS = 8;

/* Base Atmospheric Density kg/m3 */
double pb[] = {
1.29837e+0,
3.63918e-1,
8.80348e-2,
1.32250e-2,
1.42753e-3,
8.61604e-4,
6.42109e-5,
6.95786e-6 };

/* Base Temperature K */
double Tb[] = {
+19.0 + 273.15,
-56.5 + 273.15,
-56.5 + 273.15,
-44.5 + 273.15,
-2.5 + 273.15,
-2.5 + 273.15,
-58.5 + 273.15,
-86.28 + 273.15 };

/* Base Lapse Rate K/m */
double Lb[] = {
-6.5e-3,   
+0.0e-3,   
+1.0e-3,   
+2.8e-3,   
+0.0e-3,   
-2.8e-3,   
-2.0e-3,   
+0.0e-3 }; 

/* Base Height m (geopotential) */
double hb[] = {
-6.10e2, 
+1.1e4,
+2.0e4,
+3.2e4,
+4.7e4,
-5.1e4,
-7.1e4,
+8.4852e4 };

double M_air = 0.0289644; /* Air molar mass */
double g0 = 9.80665; /* Gravitational acceleration */
double R_star = 8.31432; /* Universal gas constant for air */

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

/* Get geopotential height from distance to the center of the earth. */
double geopotential_height(double R) {
	return (M_earth * G / g0) * (1/R_earth - 1/R);
}

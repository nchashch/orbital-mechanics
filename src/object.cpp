#include <math.h>
#include "object.h"
#include "constants.h"

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

//TODO: Implement activate/deactivate object functions
active_object_t activate_object(inactive_object_t obj)
{

}

inactive_object_t deactivate_object(active_object_t obj, double t)
{
	/* Calculate orbital elements of the object. */
	vector_t r = obj.r;
	double R = length(r);
	vector_t v = obj.v;

	vector_t h = cross(r, v); /* Specific angular momentum */
	double h_length = length(h);
	vector_t K_hat = { 0.0, 0.0, 1.0, 0.0 };
	vector_t n = cross(K_hat, h); /* Node vector */
	double n_length = length(n);
	vector_t e = vsub(vdiv(cross(v, h), mu_earth), norm(r)); /* Eccentricity vector */
	double ecc = length(e); /* Eccentricity */

	double E = dot(v,v) / 2 - mu_earth / R; /* Specific kinetic energy */
	const double epsilon = 1.0e-9;
	double a;
	double p;
	if(fabs(ecc) < epsilon)
	{
		a = -mu_earth / (2 * E);
		p = a * (1 - ecc*ecc);
	}
	else
	{
		p = h_length / mu_earth;
		a = INFINITY;
	}

	double inc;
	double LAN;
	double AP;

	inc = acos ( h.d[2] / h_length );
	LAN = acos ( n.d[0] / n_length );
	AP = acos ( dot(n, e) / (n_length * ecc) );
	double true_anomaly = acos ( dot(e, r) / (ecc * R) );

	if(n.d[1] < 0)
	{
		LAN = 2 * M_PI - LAN;
	}

	if(e.d[2] < 0)
	{
		AP = 2 * M_PI - AP;
	}

	if(dot(r, v) < 0)
	{
		true_anomaly = 2 * M_PI - true_anomaly;
	}
}

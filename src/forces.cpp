#include "constants.h"
#include "atmosphere.h"
#include "vector.h"
#include "object.h"

vector_t gravity_force(active_object_t obj)
{
	vector_t dr = vsub(gs_r, obj.r);
	double distance = length(dr);
	double scalar_part = mu_earth * obj.m / (distance*distance);
	vector_t vector_part = norm(dr);
	vector_t force = vmul(vector_part, scalar_part);
	return force;
}

vector_t thrust_force(active_object_t obj)
{
	return obj.thrust;
}

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

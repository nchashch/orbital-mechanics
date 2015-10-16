#include <glm.hpp>
#include "constants.h"
#include "atmosphere.h"
#include "object.h"

glm::vec3 gravity_force(active_object_t obj)
{
	glm::vec3 dr = gs_r - obj.r;
	float distance = glm::length(dr);
	float scalar_part = mu_earth * obj.m / (distance*distance);
	glm::vec3 vector_part = glm::normalize(dr);
	glm::vec3 force = vector_part * scalar_part;
	return force;
}

glm::vec3 thrust_force(active_object_t obj)
{
	return obj.thrust;
}

/* Fd = 0.5 * Cd * A * p * v^2 */
/* Cd is the drag coefficient */
/* p is the air density */
/* v is the body's velocity */
/* A is area of the body, normal to the flow. */


glm::vec3 drag_force(active_object_t obj)
{
	float R = glm::length(obj.r);
	float v_length = glm::length(obj.v);
	float Cd = obj.Cd;
	float A = obj.A;
	float p = air_density(R);
	float scalar_part = -0.5 * Cd * A * p * v_length * v_length;

	glm::vec3 vector_part = glm::normalize(obj.v);
	glm::vec3 F = vector_part * scalar_part;
	return F;
}


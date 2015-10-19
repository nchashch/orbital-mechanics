#include <glm.hpp>
#include "constants.h"
#include "atmosphere.h"
#include "Object.h"

glm::vec3 gravity_force(const Object &obj)
{
	glm::vec3 dr = gs_r - obj.get_r();
	float distance = glm::length(dr);
	float scalar_part = mu_earth * obj.get_m() / (distance*distance);
	glm::vec3 vector_part = glm::normalize(dr);
	glm::vec3 force = vector_part * scalar_part;
	return force;
}

glm::vec3 thrust_force(const Object &obj)
{
	return obj.get_thrust();
}

/* Fd = 0.5 * Cd * A * p * v^2 */
/* Cd is the drag coefficient */
/* p is the air density */
/* v is the body's velocity */
/* A is area of the body, normal to the flow. */


glm::vec3 drag_force(const Object &obj)
{
	glm::vec3 r = obj.get_r();
	float R = glm::length(r);
	glm::vec3 v = obj.get_v();
	float Cd = obj.get_Cd();
	float A = obj.get_A();
	float p = air_density(R);
	float scalar_part = -0.5 * Cd * A * p * glm::dot(v,v);

	glm::vec3 vector_part = glm::normalize(v);
	glm::vec3 F = vector_part * scalar_part;
	return F;
}


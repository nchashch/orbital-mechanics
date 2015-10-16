#include <cmath>
#include "object.h"
#include "constants.h"

active_object_t integrate_step
		(active_object_t obj,
		glm::vec3 acc,
		float dt);

active_object_t integrate
		(active_object_t obj,
		glm::vec3 (*force)(active_object_t obj),
		float dt, float elapsed_time)
{
	float et_floored = floor(elapsed_time);
	int steps = et_floored;
	float last_step_dt = elapsed_time - et_floored;

	int i;
	for(i = 0; i < steps; i++)
	{
		glm::vec3 acc = force(obj) / obj.m;
		obj = integrate_step(obj, acc, dt);
	}	

	glm::vec3 acc = force(obj) / obj.m;
	obj = integrate_step(obj, acc, last_step_dt);

	return obj;
}

active_object_t integrate_step
		(active_object_t obj,
		glm::vec3 acc,
		float dt)
{
	glm::vec3 v0 = obj.v;
	glm::vec3 r0 = obj.r;

	glm::vec3 dv = acc * dt;
	glm::vec3 dr = v0 * dt;

	glm::vec3 v1 = v0 + dv;
	glm::vec3 r1 = r0 + dr;

	obj.v = v1;
	obj.r = r1;

	return obj;
}

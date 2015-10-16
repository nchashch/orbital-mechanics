#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm.hpp>

const int NAME_LENGTH = 255; /* Excluding '\0' */

typedef struct {
	const char* name;
	float ecc;
	float a;
	float inc;
	float LAN;
	float AP;
	float M0;
	float epoch;
	float m;
	float Cd;
	float A;
} inactive_object_t;

typedef struct {
	glm::vec3 r;
	glm::vec3 v;
	glm::vec3 thrust;
	float m;
	float Cd;
	float A;
} active_object_t;

inactive_object_t deactivate(active_object_t obj);
active_object_t activate(inactive_object_t obj);
active_object_t integrate
		(active_object_t obj,
		glm::vec3 (*force)(active_object_t obj),
		float dt, float elapsed_time);
#endif

/*

Inactive object

Eccentricity: ecc
Semimajor axis: a
Inclination: inc
Longitude of ascending node: LAN
Argument of periapsis: AP
Mean anomaly: M0
Epoch: epoch
Mass: m
Drag coefficient: Cd
Area normal to the velocity: A

Active object

Position: r
Velocity: v
Thrust: thrust
Mass: m
Drag coefficient: Cd
Area normal to the velocity: A

*/


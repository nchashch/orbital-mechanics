/*

Inactive object

Eccentricity: e
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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "vector.h"

const int NAME_LENGTH = 255; /* Excluding '\0' */

typedef struct {
	const char* name;
	double e;
	double a;
	double inc;
	double LAN;
	double AP;
	double M0;
	double epoch;
	double m;
	double Cd;
	double A;
} inactive_object_t;

typedef struct {
	vector_t r;
	vector_t v;
	vector_t thrust;
	double m;
	double Cd;
	double A;
} active_object_t;

inactive_object_t deactivate(active_object_t obj);
active_object_t activate(inactive_object_t obj);
active_object_t integrate
		(active_object_t obj,
		vector_t (*force)(active_object_t obj),
		double dt, double elapsed_time);
#endif

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm.hpp>
#include <string>
#include "Circle.h"

struct KeplerianElements {
	float e; /* eccentricity */
	float a; /* semi major axis */
	float inc; /* Inclination */
	float LAN; /* Longitude of Ascending Node */
	float AP; /* Argument of Periapsis */
	float M0; /* Mean anomaly at epoch */
	float epoch;
	float t;
};

class Object {
public:
	Object
		(std::string name,
		glm::vec3 r,
		glm::vec3 v,
		glm::vec3 thrust,
		float epoch, float m, float Cd, float A);
	Object(const Object &obj);
	Object operator= (const Object &obj);
	~Object();
	void tick(float dt);
	void set_thrust(glm::vec3 thrust);
	glm::vec3 get_r() const;
	glm::vec3 get_v() const;
	glm::vec3 get_thrust() const;
	float get_m() const;
	float get_Cd() const;
	float get_A() const;
	KeplerianElements get_KeplerianElements() const;
	void recompute_ke(float epoch);
	void deactivate(float epoch);
	void activate();
private:
	glm::vec3 force();
	std::string name;
	KeplerianElements ke;
	glm::vec3 r;
	glm::vec3 v;
	glm::vec3 thrust;
	float m;
	float Cd;
	float A;
	bool active;
};

void renderOrbit
		(const KeplerianElements &ke, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color);

void renderObject
		(const glm::vec3 &r, float scale, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color);
#endif

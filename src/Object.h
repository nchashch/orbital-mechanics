#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm.hpp>
#include <string>

struct OrbitalElements {
	float ecc;
	float a;
	float inc;
	float LAN;
	float AP;
	float M0;
	float epoch;
};

class Object {
public:
	Object
		(std::string name,
		glm::vec3 r,
		glm::vec3 v,
		glm::vec3 thrust,
		float dt, float m, float Cd, float A);
	Object(const Object &obj);
	Object operator= (const Object &obj);
	~Object();
	void tick(float elapsed_time);
	void set_thrust(glm::vec3 thrust);
	void set_dt(float dt);
	glm::vec3 get_r() const;
	glm::vec3 get_v() const;
	glm::vec3 get_thrust() const;
	float get_dt() const;
	float get_m() const;
	float get_Cd() const;
	float get_A() const;
private:
	void compute_oe(float epoch);
	float dt;

	glm::vec3 (*force)(const Object &obj);
	std::string name;
	OrbitalElements oe;
	glm::vec3 r;
	glm::vec3 v;
	glm::vec3 thrust;
	float m;
	float Cd;
	float A;
};
#endif

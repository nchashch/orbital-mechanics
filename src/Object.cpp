#include "Object.h"
#include "constants.h"
#include "forces.h"
#include <gtc/matrix_transform.hpp>
#include "Circle.h"
#include <iostream>

Object::Object
	(std::string name,
	glm::vec3 r,
	glm::vec3 v,
	glm::vec3 thrust,
	float epoch, float m, float Cd, float A) :
		name(name),
		r(r),
		v(v),
		thrust(thrust),
		m(m),
		Cd(Cd),
		A(A),
		active(true)
{
	recompute_ke(epoch);
}

Object::Object(const Object &obj)
{
	*this = obj;
}

Object Object::operator= (const Object &obj)
{
	this->name = obj.name;
	this->r = obj.r;
	this->v = obj.v;
	this->thrust = obj.thrust;
	this->m = obj.m;
	this->Cd = obj.Cd;
	this->A = obj.A;
	this->active = obj.active;
	return *this;
}

Object::~Object()

{
}

void Object::tick(float dt)
{
	float m = get_m();
	glm::vec3 acc = force() / m;

	glm::vec3 dv = acc * dt;
	glm::vec3 dr = get_v() * dt;

	v += dv;
	r += dr;
}

void Object::set_thrust(glm::vec3 thrust)
{
	this->thrust = thrust;
}

glm::vec3 Object::get_r() const
{
	return r;
}

glm::vec3 Object::get_v() const
{
	return v;
}

glm::vec3 Object::get_thrust() const
{
	return thrust;
}

float Object::get_m() const
{
	return m;
}

float Object::get_Cd() const
{
	return Cd;
}

float Object::get_A() const
{
	return A;
}

KeplerianElements Object::get_KeplerianElements() const
{
	return ke;
}

void Object::recompute_ke(float epoch)
{
	const float epsilon = 1.0e-9f;
	if(v.z < epsilon)
	{
		v.z = epsilon;
	}
	glm::vec3 h = glm::cross(r, v); /* Specific angular momentum. */
	glm::vec3 e = glm::cross(v, h)/mu_earth - glm::normalize(r);
	glm::vec3 n = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), h);

	float trueAnomaly;
	if(glm::dot(r,v) >= 0.0f)
	{
		trueAnomaly = acos(glm::dot(e,r)/(glm::length(e)*glm::length(r)));
	}
	else
	{
		trueAnomaly = 2*M_PI - acos(glm::dot(e,r)/(glm::length(e)*glm::length(r)));
	}

	ke.inc = acos(h.z/glm::length(h));
	ke.e = glm::length(e);
	float E = 2*atan(tan(trueAnomaly/2)/sqrt((1+ke.e)/(1-ke.e)));

	if(n.y >= 0)
	{
		ke.LAN = acos(n.x/glm::length(n));
	}
	else
	{
		ke.LAN = 2*M_PI - acos(n.x/glm::length(n));
	}

	if(e.z >= 0)
	{
		ke.AP = acos(glm::dot(n,e)/(glm::length(n)*glm::length(e)));
	}
	else
	{
		ke.AP = 2*M_PI - acos(glm::dot(n,e)/(glm::length(n)*glm::length(e)));
	}

	ke.M0 = E - ke.e * sin(E);
	ke.a = 1/(2/glm::length(r) - glm::dot(v,v)/mu_earth);
	ke.epoch = epoch;
}

void Object::deactivate(float epoch)
{
	recompute_ke(epoch);
	active = false;
}

void Object::activate()
{
}

glm::vec3 Object::force()
{
	return gravity_force(*this);
}

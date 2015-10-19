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

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP);

void renderOrbit
		(const KeplerianElements &ke, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color)
{
	glm::mat4 orbitTransform = circleToOrbit(ke.e, ke.a/R_earth, ke.inc, ke.LAN, ke.AP);
	glm::mat4 modelViewOrbit = camera * orbitTransform;
	circle.render(program, modelViewOrbit, projection, color, GL_LINE_LOOP);
}

void renderObject
		(const glm::vec3 &r, float scale, const Circle &circle,
		GLuint program, glm::mat4 camera, glm::mat4 projection, glm::vec4 color)
{
	glm::vec3 scale_vec(scale, scale, scale);
	glm::mat4 modelView =
			camera *
			glm::translate(glm::mat4(1.0f), r/R_earth) *
			glm::scale(glm::mat4(1.0f), scale_vec);
	circle.render(program, modelView, projection, color, GL_LINE_LOOP);
}

glm::mat4 circleToOrbit
		(float e, float a,
		float inc, float LAN, float AP)
{
	float b = a * sqrt(1 - e*e); /* Semi minor axis */
	float f = a * e; /* Distance from the center to the focus. */
	glm::vec3 scale(a, b, 1.0f); /* Scale the circle */
	glm::vec3 translation(-f, 0.0f, 0.0f); /* Move up by one focus distance. */

	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	glm::mat4 model =
		glm::rotate(glm::mat4(1.0f), LAN, z_axis) * 
		glm::rotate(glm::mat4(1.0f), inc, x_axis) *
		glm::rotate(glm::mat4(1.0f), AP, z_axis) *
		glm::translate(glm::mat4(1.0f), translation) *
		glm::scale(glm::mat4(1.0f), scale);
	return model;
}

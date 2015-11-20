#define GLM_SWIZZLE
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
		active(true),
		crashed(false)
{
	recompute_ke(epoch);
}

Object::Object
	(std::string name,
	KeplerianElements ke,
	float m, float Cd, float A) :
		name(name),
		ke(ke),
		m(m),
		Cd(Cd),
		A(A),
		active(true),
		crashed(false)
{
	recompute_sv();
}

Object::Object(const Object &obj)
{
	this->name = obj.name;
	this->ke = obj.ke;
	this->r = obj.r;
	this->v = obj.v;
	this->thrust = obj.thrust;
	this->m = obj.m;
	this->Cd = obj.Cd;
	this->A = obj.A;
	this->active = obj.active;
	this->crashed = obj.crashed;
}

Object Object::operator= (const Object &obj)
{
	this->name = obj.name;
	this->ke = obj.ke;
	this->r = obj.r;
	this->v = obj.v;
	this->thrust = obj.thrust;
	this->m = obj.m;
	this->Cd = obj.Cd;
	this->A = obj.A;
	this->active = obj.active;
	this->crashed = obj.crashed;
	return *this;
}

Object::~Object()
{
}

void Object::tick(float dt)
{
	if(!crashed)
		ke.t += dt;
	if(active && !crashed)
	{
		float m = get_m();
		const float fixed_dt = 1.0f;
		if(dt < fixed_dt)
		{
			glm::vec3 acc = force() / m;

			glm::vec3 dv = acc * dt;
			glm::vec3 dr = get_v() * dt;

			v += dv;
			r += dr;

		}
		else
		{
			recompute_sv();
		}

		if(glm::length(r) < R_earth)
		{
			crashed = true;
		}
	}
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

bool Object::isActive() const
{
	return active;
}

bool Object::isCrashed() const
{
	return crashed;
}

KeplerianElements Object::get_ke() const
{
	return ke;
}

void Object::recompute_ke(float epoch)
{
	glm::vec3 h = glm::cross(r, v);
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

	if(std::abs(ke.inc) < epsilon)
	{
		ke.LAN = 0.0f;
		ke.AP = 0.0f;
		if(std::abs(ke.e) < epsilon)
		{
			ke.LP = 0.0f;
		}
		else
		{
			glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
			if(e.z >= 0)
			{
				ke.LP = acos(glm::dot(x_axis, e)/ke.e);
			}
			else
			{
				ke.LP = 2*M_PI - acos(glm::dot(x_axis, e)/ke.e);
			}
		}
	}
	else
	{
		ke.LP = ke.AP + ke.LAN; 
	}

	ke.M0 = E - ke.e * sin(E);
	ke.a = 1/(2/glm::length(r) - glm::dot(v,v)/mu_earth);
	ke.epoch = epoch;
	ke.t = epoch;
}

float newtonRaphson(float M, float e, int iterations);
void Object::recompute_sv()
{
	float n = sqrt(mu_earth/pow(ke.a, 3));
	float dt = (ke.t - ke.epoch);
	float M = ke.M0 + dt * n;
	const int iterations = 10;
	float E = newtonRaphson(M, ke.e, iterations);
	float trueAnomaly = 2*atan2(sqrt(1+ke.e)*sin(E/2), sqrt(1-ke.e)*cos(E/2));
	float R = ke.a*(1-ke.e*cos(E));
	glm::vec4 r = R * glm::vec4(cos(trueAnomaly), sin(trueAnomaly), 0, 1);
	glm::vec4 v = ((float)sqrt(mu_earth*ke.a)/R) * glm::vec4(-sin(E), sqrt(1-pow(ke.e,2))*cos(E), 0, 0);
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	glm::mat4 ECI_Transform(1.0f);
	if(std::abs(ke.inc) < epsilon)
	{
		if(std::abs(ke.e) > epsilon)
		{
			ECI_Transform =
				glm::rotate(glm::mat4(1.0f), ke.LP, z_axis);
		}
	}
	else
	{
		ECI_Transform =
			glm::rotate(glm::mat4(1.0f), ke.LAN, z_axis) *
			glm::rotate(glm::mat4(1.0f), ke.inc, x_axis) *
			glm::rotate(glm::mat4(1.0f), ke.AP, z_axis);
	}
	r = ECI_Transform * r;
	v = ECI_Transform * v;
	this->r = r.xyz();
	this->v = v.xyz();
	/*
	std::cout << ke.e << " "
		  << ke.a << " "
		  << ke.inc << " "
		  << ke.LAN << " "
		  << ke.AP << " "
		  << ke.LP << " "
		  << ke.M0 << " "
		  << std::endl;
	*/
}

float newtonRaphson(float M, float e, int iterations)
{
	float E = M;
	for(int i = 0; i < iterations; i++)
	{
		E = E - (E - e*sin(E) - M)/(1 - e*cos(E));
	}
	return E;
}

void Object::deactivate()
{
	if(active)
	{
		recompute_ke(ke.t);
		active = false;
	}
}

void Object::activate()
{
	if(!active)
	{
		recompute_sv();
		active = true;
	}
}

glm::vec3 Object::force()
{
	return gravity_force(*this);
}

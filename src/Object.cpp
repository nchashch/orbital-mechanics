#include "Object.h"

Object::Object
	(std::string name,
	glm::vec3 r,
	glm::vec3 v,
	glm::vec3 thrust,
	float dt, float m, float Cd, float A) :
		name(name),
		r(r),
		v(v),
		thrust(thrust),
		dt(dt),
		m(m),
		Cd(Cd),
		A(A)
{
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
	this->dt = obj.dt;
	this->m = obj.m;
	this->Cd = obj.Cd;
	this->A = obj.A;
	return *this;
}

Object::~Object()

{
}

void Object::tick(float elapsed_time)
{
	float et_floored = floor(elapsed_time);
	float dt = this->get_dt();
	int steps = (int)(et_floored / dt);

	int i;
	for(i = 0; i < steps; i++)
	{
		float m = this->get_m();
		glm::vec3 acc = this->force(*this) / m;

		glm::vec3 dv = acc * dt;
		glm::vec3 dr = this->get_v() * dt;

		this->v += dv;
		this->r += dr;
	}	
}

void Object::set_thrust(glm::vec3 thrust)
{
	this->thrust = thrust;
}

void Object::set_dt(float dt)
{
	this->dt = dt;
}

glm::vec3 Object::get_r() const
{
	return this->r;
}

glm::vec3 Object::get_v() const
{
	return this->v;
}

glm::vec3 Object::get_thrust() const
{
	return this->thrust;
}

float Object::get_dt() const
{
	return this->dt;
}

float Object::get_m() const
{
	return this->m;
}

float Object::get_Cd() const
{
	return this->Cd;
}

float Object::get_A() const
{
	return this->A;
}

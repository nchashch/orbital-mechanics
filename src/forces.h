#ifndef _FORCES_H_
#define _FORCES_H_
#include "Object.h"
glm::vec3 gravity_force(const Object &obj);
glm::vec3 thrust_force(const Object &obj);
glm::vec3 drag_force(const Object &obj);
#endif

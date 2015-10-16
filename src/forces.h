#ifndef _FORCES_H_
#define _FORCES_H_
glm::vec3 gravity_force(active_object_t obj);
glm::vec3 thrust_force(active_object_t obj);
glm::vec3 drag_force(active_object_t obj);
#endif

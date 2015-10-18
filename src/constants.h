#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#include <glm.hpp>
const glm::vec3 gs_r = glm::vec3(0.0, 0.0, 0.0); /* Position of the earth */
const float M_earth = 5.9742e24; /* Mass of the earth */
const float R_earth = 3.671e6; /* Mean radius of the earth */
const float G = 6.67408e-11; /* Gravitational constant */
const float mu_earth = M_earth * G; /* Standard gravitational parameter. */
const float M_air = 0.0289644; /* Air molar mass */
const float g0 = mu_earth/(R_earth * R_earth); /* Gravitational acceleration */
const float R_gas = 8.31432; /* Universal gas constant for air */

#endif

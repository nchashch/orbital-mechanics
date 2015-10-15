#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#include "vector.h"
const vector_t gs_r = {0.0, 0.0, 0.0, 1.0}; /* Position of the earth */
const double M_earth = 5.9742e24; /* Mass of the earth */
const double R_earth = 3.671e6; /* Mean radius of the earth */
const double G = 6.67408e-11; /* Gravitational constant */
const double mu_earth = M_earth * G; /* Standard gravitational parameter. */
const double M_air = 0.0289644; /* Air molar mass */
const double g0 = mu_earth/(R_earth * R_earth); /* Gravitational acceleration */
const double R_star = 8.31432; /* Universal gas constant for air */

#endif

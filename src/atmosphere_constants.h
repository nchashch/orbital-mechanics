#ifndef _ATMOSPHERE_CONSTANTS_H_
#define _ATMOSPHERE_CONSTANTS_H_
/* International Standard Atmosphere data */

/*
0 Troposphere
1 Tropopause
2 Startosphere
3 Startosphere
4 Startopause
5 Mesosphere
6 Mesosphere
7 Mesopause
*/

const int NUMBER_OF_LAYERS = 8;

/* Base Atmospheric Density kg/m3 */
const float pb[] = {
1.29837e+0,
3.63918e-1,
8.80348e-2,
1.32250e-2,
1.42753e-3,
8.61604e-4,
6.42109e-5,
6.95786e-6 };

/* Base Temperature K */
const float Tb[] = {
+19.0 + 273.15,
-56.5 + 273.15,
-56.5 + 273.15,
-44.5 + 273.15,
-2.5 + 273.15,
-2.5 + 273.15,
-58.5 + 273.15,
-86.28 + 273.15 };

/* Base Lapse Rate K/m */
const float Lb[] = {
-6.5e-3,   
+0.0e-3,   
+1.0e-3,   
+2.8e-3,   
+0.0e-3,   
-2.8e-3,   
-2.0e-3,   
+0.0e-3 }; 

/* Base Height m (geopotential) */
const float hb[] = {
-6.10e2, 
+1.1e4,
+2.0e4,
+3.2e4,
+4.7e4,
-5.1e4,
-7.1e4,
+8.4852e4 };
#endif

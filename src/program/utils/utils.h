#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdlib.h>
#include "../vec3/vec3.h"

void initRandomTable(void);
double randomDouble(unsigned int *seed);
double randomDoubleMinMax(double pMin, double pMax, unsigned int *seed);
Vec3 randomInUnitSphere(unsigned int *seed);
Vec3 randomInHemisphere(Vec3 pNormal, unsigned int *seed);
double clamp(double pValue, double pMin, double pMax);
Vec3 randomUnitVector(unsigned int *seed);
Vec3 randomInHemisphere(Vec3 pNormal, unsigned int *seed);
#endif
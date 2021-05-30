#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../hittable/hittable.h"
#include "../ray/ray.h"
#include "../../utils/array.h"
#include "../material/material.h"

typedef struct
{
    Vec3 center;
    double radious;
    double materialFuzz;
    Color materialColor;
    MaterialScatterFunctionPointer materialScatterFunc;
} Sphere;

bool sphereHit(Sphere this, Ray ray, double tMin, double tMax, HitRecord *hitRecord);
bool spheresHit(Array *this, Ray ray, double tMin, double tMax, HitRecord *hitRecord);

#endif
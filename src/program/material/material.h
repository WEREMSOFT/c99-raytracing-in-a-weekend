#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../vec3/vec3.h"
#include "../ray/ray.h"
#include "../hittable/hittable.h"

bool lambertianMaterialScatter(Color albedo, Ray pRay, HitRecord *pHitRecord, Color *pAtenuation, Ray *pScattered, unsigned int *seed);
bool metalMaterialScatter(Color albedo, Ray pRay, HitRecord *hitRecord, Color *atenuation, Ray *pScattered, unsigned int *seed, double fuzz);

#endif
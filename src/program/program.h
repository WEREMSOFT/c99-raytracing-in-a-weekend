#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include "ppmImage/ppmImage.h"
#include "camera/camera.h"
#include "sphere/sphere.h"
#include "../utils/array.h"

typedef struct
{
    PPMImage image;
    Camera camera;
    int spheresCount;
    Array *spheres;
    int maxRecursionDepth;
    int samplesPerPixel;
    Array *imageBuffer;
} Program;

Program programCreate(void);
void programMainLoop(Program *);
void programDestroy(Program);

#endif
#ifndef _PPM_IMAGE_H__
#define _PPM_IMAGE_H__

#include <stdio.h>

typedef struct
{
    float aspectRatio;
    unsigned int width, height;
    FILE *fp;
} PPMImage;

PPMImage ppmImageCreateFromAspectRatio(unsigned int width, float aspectRatio, char *fileName);
PPMImage ppmImageCreate(unsigned int with, unsigned int height, char *filename);
void ppmImageDestroy(PPMImage);

#endif

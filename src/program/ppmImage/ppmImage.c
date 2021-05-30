#include "ppmImage.h"
#include <stdlib.h>
#include "../vec3/vec3.h"

PPMImage ppmImageCreateFromAspectRatio(unsigned int width, float aspectRatio, char *fileName)
{
    int height = width / aspectRatio;
    return ppmImageCreate(width, height, fileName);
}

PPMImage ppmImageCreate(unsigned int width, unsigned int height, char *filename)
{

    PPMImage this = {0};

    this.fp = fopen(filename, "w+");

    this.width = width;
    this.height = height;

    this.aspectRatio = (float)this.width / (float)this.height;

    if (this.fp == NULL)
    {
        fprintf(stderr, "Error creating the file %s\n", filename);
        exit(-1);
    }

    fprintf(this.fp, "P3\n%d %d\n255\n", this.width, this.height);

    return this;
}

void ppmImageDestroy(PPMImage this)
{
    fclose(this.fp);
}
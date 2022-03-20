#include "program.h"
#include "ppmImage/ppmImage.h"
#include "ray/ray.h"
#include "utils/utils.h"
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#define NUM_THREADS 16

Program programCreate()
{
    initRandomTable();
    Program this = {0};
    this.image = ppmImageCreateFromAspectRatio(3000, 16.0 / 9.0, "test.ppm");
    this.camera = cameraCreate(this.image);
    this.spheresCount = 2;
    this.spheres = arrayCreate(10, sizeof(Sphere));
    this.maxRecursionDepth = 50;
    this.samplesPerPixel = 100;

    Color materialColorGround = (Color){.5, .5, 0};
    Color materialColorCenter = (Color){.7, .3, .3};
    Color materialColorLeft = (Color){.8, .6, .2};
    Color materialColorRight = (Color){.8, .8, .8};

    arrayInsertElement(&this.spheres, &(Sphere){.radious = 0.5, .materialFuzz = 0, .center = (Vec3){0, 0, -1}, .materialColor = materialColorCenter, .materialScatterFunc = lambertianMaterialScatter});
    arrayInsertElement(&this.spheres, &(Sphere){.radious = 0.5, .materialFuzz = 0.1, .center = (Vec3){-1, 0, -1}, .materialColor = materialColorRight, .materialScatterFunc = metalMaterialScatter});
    arrayInsertElement(&this.spheres, &(Sphere){.radious = 0.5, .materialFuzz = 0.3, .center = (Vec3){1, 0, -1}, .materialColor = materialColorLeft, .materialScatterFunc = metalMaterialScatter});
    arrayInsertElement(&this.spheres, &(Sphere){.radious = 100, .materialFuzz = 0, .center = (Vec3){0, -100.5, -1}, .materialColor = materialColorGround, .materialScatterFunc = lambertianMaterialScatter});

    return this;
}

void colorWrite(Array **array, Color color, int pSamplesPerPixel)
{
    double scale = 1.0 / pSamplesPerPixel;
    Color rectifiedColor = (Color){
        sqrt(color.x * scale),
        sqrt(color.y * scale),
        sqrt(color.z * scale)};

    arrayInsertElement(array, &rectifiedColor);
}

static Color rayColor(Ray this, Array *pSpheres, int pMaxDepth, unsigned int *seed)
{

    if (pMaxDepth <= 0)
        return (Vec3){0};

    HitRecord hitRecord = {0};
    if (spheresHit(pSpheres, this, 0.001, LONG_MAX, &hitRecord))
    {
        Ray scattered = {0};
        Color attenuation = {0};

        if (hitRecord.materialScatterFunc(hitRecord.materialColor, this, &hitRecord, &attenuation, &scattered, seed, hitRecord.materialFuzz))
        {
            return vec3MultElem(attenuation, rayColor(scattered, pSpheres, pMaxDepth - 1, seed));
        }

        return (Color){0, 0, 0};
    }

    Vec3 unitDirection = vec3Unit(this.direction);
    hitRecord.t = 0.5 * (unitDirection.y + 1.0);
    return vec3Add(
        vec3Mult(vec3Create(1, 1, 1), (1.0 - hitRecord.t)),
        vec3Mult(vec3Create(.5, .7, 1.), hitRecord.t));
}

typedef struct
{
    int scanlineStart;
    int scanlinesEnd;
    int arraySize;
    Program program;
    Array *array;
} ThreadInformation;

void *processImageSlice(void *args)
{
    ThreadInformation *parameters = args;

    unsigned int seed;

    for (int j = parameters->scanlinesEnd; j >= parameters->scanlineStart; j--)
    {
        for (int i = 0; i < parameters->program.image.width; i++)
        {
            Color pixelColor = {0};

            for (int k = 0; k < parameters->program.samplesPerPixel; k++)
            {
                double u = (double)(i + randomDouble(&seed)) / (parameters->program.image.width - 1);
                double v = (double)(j + randomDouble(&seed)) / (parameters->program.image.height - 1);
                Vec3 temp = vec3Add(vec3Add(parameters->program.camera.lowerLeft, vec3Mult(parameters->program.camera.horizontal, u)), vec3Mult(parameters->program.camera.vertical, v));
                Vec3 direction = vec3Substract(temp, parameters->program.camera.origin);
                Ray ray = rayCreate(parameters->program.camera.origin, direction);
                pixelColor = vec3Add(pixelColor, rayColor(ray, parameters->program.spheres, parameters->program.maxRecursionDepth, &seed));
            }

            colorWrite(&parameters->array, pixelColor, parameters->program.samplesPerPixel);
        }
    }
    return NULL;
}

void programMainLoop(Program *this)
{
    pthread_t threads[NUM_THREADS] = {0};
    ThreadInformation threadParameters[NUM_THREADS] = {0};

    int sliceHeight = this->image.height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadParameters[i].scanlineStart = i * sliceHeight;
        threadParameters[i].scanlinesEnd = threadParameters[i].scanlineStart + sliceHeight;
        threadParameters[i].program = *this;
        threadParameters[i].array = arrayCreate(sliceHeight * this->image.width, sizeof(Color));
        int resultCode = pthread_create(&threads[i], NULL, processImageSlice, &threadParameters[i]);
        assert(!resultCode && "Error creating thread");
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = NUM_THREADS - 1; i >= 0; i--)
    {
        for (int j = 0; j < threadParameters[i].array->header.length; j++)
        {
            char line[200] = {0};
            Color *color = (Color *)&threadParameters[i].array->data[j * sizeof(Color)];
            double colorMultiply = 255.0;
            snprintf(line, 200, "%d %d %d\n", (int)(clamp(color->x, 0.0, 0.999) * colorMultiply), (int)(clamp(color->y, 0.0, 0.999) * colorMultiply), (int)(clamp(color->z, 0.0, 0.999) * colorMultiply));
            fprintf(this->image.fp, "%s", line);
        }
    }
    for (int i = NUM_THREADS - 1; i >= 0; i--)
    {
        free(threadParameters[i].array);
    }
}

void programDestroy(Program this)
{
    ppmImageDestroy(this.image);
    free(this.spheres);
    free(this.imageBuffer);
}

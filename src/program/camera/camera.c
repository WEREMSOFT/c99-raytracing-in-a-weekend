#include "camera.h"

Camera cameraCreate(PPMImage image)
{
    Camera this = {0};
    this.aspectRatio = image.aspectRatio;
    this.viewPortHeight = 2.0;
    this.viewPortWidth = this.aspectRatio * this.viewPortHeight;
    this.focalLength = 1.0;
    this.horizontal = vec3Create(this.viewPortWidth, 0, 0);
    this.vertical = vec3Create(0, this.viewPortHeight, 0);
    {
        Vec3 temp = vec3Mult(this.horizontal, .5);
        Vec3 point1 = vec3Substract(this.origin, temp);
        temp = vec3Mult(this.vertical, .5);
        Vec3 point2 = vec3Substract(point1, temp);
        temp = vec3Create(0, 0, this.focalLength);
        this.lowerLeft = vec3Substract(point2, temp);
    }
    return this;
}
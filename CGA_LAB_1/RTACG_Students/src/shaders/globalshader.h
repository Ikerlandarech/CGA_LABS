#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"
#include "../core/hemisphericalsampler.h"


class GlobalShader : public Shader
{
public:
    GlobalShader();
    GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

private:
    double maxDist;
    Vector3D hitColor;
    Vector3D color;
    Vector3D at;
};

#endif // GLOBALSHADER_H
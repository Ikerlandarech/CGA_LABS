#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"


class DirectShader : public Shader
{
public:
    DirectShader();
    DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

    Vector3D computeColorBasic(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

private:
    double maxDist;
    Vector3D hitColor;
    Vector3D color;
};

#endif // DIRECTSHADER_H
//#include "depthshader.h"
#include "..\..\src\shaders\depthshader.h"
#include "..\..\src\core\utils.h"

DepthShader::DepthShader()
{
}

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
{
}

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection intersection;
    if (Utils::hasIntersection(r, objList))
        return hitColor;
    else
        return bgColor;
}

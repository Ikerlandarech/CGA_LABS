#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_ Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_)
{ }

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
    if (Utils::hasIntersection(r, objList))
        double ci = 1 - Utils::getClosestIntersection(r, objList, ) / maxDist;
        if (ci < 0) {
            ci = 0;
        }
    return Vector3D(ci,ci,ci);

    else
        return bgColor;
}

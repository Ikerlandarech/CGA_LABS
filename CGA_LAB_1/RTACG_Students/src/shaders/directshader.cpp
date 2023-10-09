#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
    color(Vector3D(1, 0, 0))
{ }

DirectShader::DirectShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection intersection;
    Vector3D outputLight = Vector3D(0, 0, 0);
    if (Utils::getClosestIntersection(r, objList, intersection)) {
        double visibility = 0;
        for (size_t s = 0; s < lsList.size(); s++)
        {
            Vector3D normal = intersection.normal;
            Vector3D wo = (r.o - intersection.itsPoint).normalized();
            Vector3D wi = (lsList.at(s).getPosition() - intersection.itsPoint).normalized();
            Vector3D lightIntensity = lsList.at(s).getIntensity(intersection.itsPoint);
            Vector3D reflectance = intersection.shape->getMaterial().getReflectance(normal, wo, wi);
            if (Utils::hasIntersection(r, objList))
            {
                visibility = 1;
            }
            outputLight = outputLight + (lightIntensity * reflectance * visibility);
        }
        return outputLight;
    }
    return bgColor;
}

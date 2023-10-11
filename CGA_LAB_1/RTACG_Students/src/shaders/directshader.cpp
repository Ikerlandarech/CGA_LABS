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
    Vector3D color = Vector3D(0, 0, 0);
    Vector3D n = intersection.normal;
    Vector3D wo = (r.o - intersection.itsPoint).normalized();
    if (intersection.shape->getMaterial().hasTransmission()) {
        Vector3D l = r.d;
        double nt = 1.1;
        double sin2alf = 1 - pow(dot(l, n), 2);
        double sqint = 1 - pow(nt, 2) * sin2alf;
        //Checking Internal Reflection:
        if ((sqint) >= 0)
        {
            Vector3D wt = (n.operator*(-sqrt(sqint) + nt*(dot(l, n))) + (l.operator*(nt))).normalized();
            Ray refractionRay = Ray(intersection.itsPoint, wt, r.depth + 1);
            color = computeColorBasic(refractionRay, objList, lsList);
        }
        else if ((sqint) < 0)
        {
            Vector3D wr = ((n.operator*(dot(n, r.d)) * 2.0) - r.d).normalized();
            Ray reflectionRay = Ray(intersection.itsPoint, wr, r.depth + 1);
            color = computeColorBasic(reflectionRay, objList, lsList);
        }
    }
    else if (intersection.shape->getMaterial().hasSpecular())
    {
        Vector3D wr = ((n.operator*(dot(n, r.d)) * 2.0) - r.d).normalized();
        Ray reflectionRay = Ray(intersection.itsPoint, wr, r.depth + 1);
        color = computeColorBasic(reflectionRay, objList, lsList);
    }
    return bgColor;
}

Vector3D DirectShader::computeColorBasic(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection intersection;
    Vector3D outputLight = Vector3D(0, 0, 0);
    if (Utils::getClosestIntersection(r, objList, intersection)) {
        bool visibility = false;

        for (size_t s = 0; s < lsList.size(); s++)
        {
            Vector3D normal = intersection.normal;
            Vector3D wo = (r.o - intersection.itsPoint).normalized();
            Vector3D wi = (lsList.at(s).getPosition() - intersection.itsPoint).normalized();
            Vector3D lightIntensity = lsList.at(s).getIntensity(intersection.itsPoint);
            Vector3D reflectance = intersection.shape->getMaterial().getReflectance(normal, wo, wi);
            Ray wiRay = Ray(intersection.itsPoint, wi, 0, Epsilon, (lsList.at(s).getPosition() - intersection.itsPoint).length());
            if (!Utils::hasIntersection(wiRay, objList))
            {
                visibility = 1;
                outputLight = outputLight + (lightIntensity * reflectance * visibility);
            }
        }
        return outputLight;
    }
}

#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
    color(Vector3D(0))
{ }

DirectShader::DirectShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection intersection;
    Vector3D color = Vector3D(0, 0, 0);
    if (Utils::getClosestIntersection(r, objList, intersection)) {
        bool visibility = false;
        Vector3D n = intersection.normal;
        Vector3D wo = -r.d;
        Vector3D l = wo;
        if (intersection.shape->getMaterial().hasTransmission()) {
            double nt = intersection.shape->getMaterial().getIndexOfRefraction();
            if (dot(n, l) < 0)
            {
                n = -n;
                nt = 1 / nt;
            }
            double sin2alf = 1 - pow(dot(l, n), 2);
            double sqint = 1 - pow(nt, 2) * sin2alf;
            //Checking Internal Reflection:
            if ((sqint) >= 0)
            {
                double cosa = sqrt(1 - sin2alf);
                double sina = sqrt(sin2alf);
                double sin2bet = pow((sina * nt),2);
                double cosb = sqrt(1 - sin2bet);
                
                Vector3D wt = Utils::computeTransmissionDirection(r, n, nt, cosa, cosb);
                Ray refractionRay = Ray(intersection.itsPoint, wt, r.depth + 1);
                color = computeColor(refractionRay, objList, lsList);

            }
            else if ((sqint) < 0)
            {
                Vector3D wr = Utils::computeReflectionDirection(r.d, n);
                Ray reflectionRay = Ray(intersection.itsPoint, wr, r.depth + 1);
                color = computeColor(reflectionRay, objList, lsList);

            }
        }
        else if (intersection.shape->getMaterial().hasSpecular())
        {

            Vector3D wr = Utils::computeReflectionDirection(r.d, n);
            Ray reflectionRay = Ray(intersection.itsPoint, wr, r.depth + 1);
            color = computeColor(reflectionRay, objList, lsList);

        }
        else if (intersection.shape->getMaterial().hasDiffuseOrGlossy())
        {
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
                    color = color + (lightIntensity * reflectance * visibility);
                }
            }
        }
        return color;
    }
    else
    {
        return bgColor;
    }
}
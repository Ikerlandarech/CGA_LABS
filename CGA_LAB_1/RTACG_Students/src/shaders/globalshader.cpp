#include "globalshader.h"
#include "../core/utils.h"
#include <math.h>
#define N_DIRECTIONS 200
#define N_BOUNCES 2

GlobalShader::GlobalShader() :
    color(Vector3D(0))
{ }

GlobalShader::GlobalShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_, Vector3D at_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_), at(at_)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Ray cameraRay = r;

    Intersection intersection;
    Vector3D color = Vector3D(0, 0, 0);
    Vector3D dirIllumination = Vector3D(0, 0, 0);
    Vector3D indirIllumination = Vector3D(0, 0, 0);
    if (Utils::getClosestIntersection(cameraRay, objList, intersection)) {
        bool visibility = false;
        Vector3D n = intersection.normal;
        Vector3D wo = -cameraRay.d;
        Vector3D l = wo;
        //TRANSMISSIVE//
        if (intersection.shape->getMaterial().hasTransmission()) {
            cameraRay.depth = 0;
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
                double sin2bet = pow((sina * nt), 2);
                double cosb = sqrt(1 - sin2bet);

                Vector3D wt = Utils::computeTransmissionDirection(cameraRay, n, nt, cosa, cosb);
                Ray refractionRay = Ray(intersection.itsPoint, wt, cameraRay.depth + 1);
                color = computeColor(refractionRay, objList, lsList);

            }
            else if ((sqint) < 0)
            {
                Vector3D wr = Utils::computeReflectionDirection(cameraRay.d, n);
                Ray reflectionRay = Ray(intersection.itsPoint, wr, cameraRay.depth + 1);
                color = computeColor(reflectionRay, objList, lsList);

            }
        }
        //MIRROR//
        else if (intersection.shape->getMaterial().hasSpecular())
        {
            cameraRay.depth = 0;
            Vector3D wr = Utils::computeReflectionDirection(cameraRay.d, n);
            Ray reflectionRay = Ray(intersection.itsPoint, wr, cameraRay.depth + 1);



            reflectionRay.depth = cameraRay.depth;
            


            color = computeColor(reflectionRay, objList, lsList);

        }
        //PHONG//
        else if (intersection.shape->getMaterial().hasDiffuseOrGlossy())
        {
            for (size_t s = 0; s < lsList.size(); s++)
            {
                Vector3D normal = intersection.normal;
                Vector3D wo = (cameraRay.o - intersection.itsPoint).normalized();
                Vector3D wi = (lsList.at(s).getPosition() - intersection.itsPoint).normalized();
                Vector3D lightIntensity = lsList.at(s).getIntensity(intersection.itsPoint);
                Vector3D reflectance = intersection.shape->getMaterial().getReflectance(normal, wo, wi);
                Ray wiRay = Ray(intersection.itsPoint, wi, 0, Epsilon, (lsList.at(s).getPosition() - intersection.itsPoint).length());
                if (!Utils::hasIntersection(wiRay, objList))
                {
                    if (cameraRay.depth == 0)
                    {
                        Vector3D coeff = (1.0 / N_DIRECTIONS * 2.0 * M_PI);
                        for (size_t i = 0; i < N_DIRECTIONS; i++)
                        {
                            HemisphericalSampler hs = HemisphericalSampler();
                            Vector3D wj = hs.getSample(normal).normalized();
                            Ray rBounced = Ray(intersection.itsPoint, wj, cameraRay.depth + 1);
                            Vector3D li = computeColor(rBounced, objList, lsList);
                            Vector3D rp = intersection.shape->getMaterial().getReflectance(normal, wo, wj);
                            indirIllumination += li * rp;
                        }
                        indirIllumination = indirIllumination * coeff;
                    }
                    else if (cameraRay.depth > 0)
                    {
                        indirIllumination = intersection.shape->getMaterial().getDiffuseCoefficient() * at;
                    }
                    visibility = 1;
                    dirIllumination = dirIllumination + (lightIntensity * reflectance * visibility);
                    color = dirIllumination + indirIllumination;
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
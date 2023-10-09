#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
    color(Vector3D(1, 0, 0))
{ }

DirectShader::DirectShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_)
{ }

//Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
//{
//    Intersection intersection;
//    Intersection intersectionV;
//    Vector3D outputLight = Vector3D(0,0,0);
//    if (Utils::getClosestIntersection(r, objList, intersection)) {
//        bool visibility = false;
//        for (size_t obj = 0; obj < objList.size(); obj++)
//        {
//            for (size_t s = 0; s < lsList.size(); s++)
//            {
//                Vector3D normal = intersection.normal;
//                Vector3D wo = (r.o - intersection.itsPoint).normalized();
//                Vector3D wi = (lsList.at(s).getPosition() - intersection.itsPoint).normalized();
//                Vector3D lightIntensity = lsList.at(s).getIntensity(intersection.itsPoint);
//                Vector3D reflectance = objList.at(0)->getMaterial().getReflectance(normal, wo, wi);
//                objList.at(obj)->rayIntersect(r, intersectionV);
//                if ((intersection.itsPoint.x == intersectionV.itsPoint.x) && (intersection.itsPoint.y == intersectionV.itsPoint.y) && (intersection.itsPoint.z == intersectionV.itsPoint.z))
//                {
//                    visibility = true;
//                }
//                outputLight = outputLight + (lightIntensity * reflectance * visibility);
//            }
//        }
//        return outputLight;
//    }
//    return bgColor;
//}

//Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
//{
//    Intersection intersection;
//    Intersection intersectionV;
//    Vector3D outputLight = Vector3D(0, 0, 0);
//    if (Utils::getClosestIntersection(r, objList, intersection)) {
//        bool visibility = false;
//
//        for (size_t s = 0; s < lsList.size(); s++)
//        {
//            Vector3D normal = intersection.normal;
//            Vector3D wo = (r.o - intersection.itsPoint).normalized();
//            Vector3D wi = (lsList.at(s).getPosition() - intersection.itsPoint).normalized();
//            Vector3D lightIntensity = lsList.at(s).getIntensity(intersection.itsPoint);
//            Vector3D reflectance = intersection.shape->getMaterial().getReflectance(normal, wo, wi);
//            objList.at(0)->rayIntersect(r, intersectionV);
//            if ((intersection.itsPoint.x == intersectionV.itsPoint.x) && (intersection.itsPoint.y == intersectionV.itsPoint.y) && (intersection.itsPoint.z == intersectionV.itsPoint.z))
//            {
//                visibility = true;
//            }
//            outputLight = outputLight + (lightIntensity * reflectance * visibility);
//        }
//        return outputLight;
//    }
//    return bgColor;
//}

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const {
    Intersection* its = new Intersection();
    if (Utils::getClosestIntersection(r, objList, *its)) {

        Vector3D Lo = Vector3D(0, 0, 0);
        for (int i = 0; i < lsList.size(); i++) {

            Vector3D Li = lsList.at(i).getIntensity(its->itsPoint);
            Vector3D wi = (lsList.at(i).getPosition() - its->itsPoint).normalized();
            double maxT = (lsList.at(i).getPosition() - its->itsPoint).length();

            Ray wiRay = Ray(its->itsPoint, wi, 0, Epsilon, maxT);
            Vector3D rp = its->shape->getMaterial().getReflectance(its->normal, (r.o - its->itsPoint).normalized(), wi);

            if (!Utils::hasIntersection(wiRay, objList)) {
                Lo += Li * rp;
            }

        }
        return Lo;
    }
    else
        return bgColor;
}
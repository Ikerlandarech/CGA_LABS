#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() :
    color(Vector3D(1, 0, 0))
{ }

NormalShader::NormalShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_), maxDist(maxDist_)
{ }

//Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
//{
//    double ci{};
//    double hitDist{};
//    Intersection intersection;
//
//    if (Utils::getClosestIntersection(r, objList, intersection))
//        hitDist = sqrt(pow(intersection.itsPoint.x - r.o.x,2) - pow(intersection.itsPoint.y - r.o.y,2));
//        ci = 1.0 - (hitDist / maxDist);
//        if (ci < 0.0) {
//            ci = 0.0;
//        }
//        return Vector3D(0, ci, 0);
//
//    return bgColor;
//
//}


Vector3D NormalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection* intersection = new Intersection();
    Vector3D color;
    Vector3D normal;
    Vector3D normone = Vector3D(1.0, 1.0, 1.0);
    if (Utils::getClosestIntersection(r, objList, *intersection)) {
        normal.x = intersection->normal.x;
        normal.y = intersection->normal.y;
        normal.z = intersection->normal.z;

        color = (normal + normone) / 2;
        return color;
    }
    return bgColor;
}
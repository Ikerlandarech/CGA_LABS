#include "utils.h"

Utils::Utils()
{ }

double Utils::degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

Vector3D Utils::multiplyPerCanal(const Vector3D &v1, const Vector3D &v2)
{
    //return Vector3D(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
    return Vector3D(0.0);
}

//bool Utils::hasIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList) //or Shadow Ray
//{
//    //bool resultIntersection = false;
//    bool intersection = false;
//    //std::cout << "Need to implement the function Utils::hasIntersection() in the file utils.cpp" << std::endl;
//    for(size_t objIndex = 0; objIndex < objectsList.size(); objIndex ++)
//    {
//          // Get the current object
//          const Shape *obj = objectsList.at(objIndex);
//
//          intersection = obj->rayIntersectP(cameraRay);
//          if (intersection == true)
//          {
//            return intersection;
//          }
//    }
//    return intersection;
//}

bool Utils::hasIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList) //or Shadow Ray
{
    bool intersection = false;
    for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
    {
        const Shape* obj = objectsList.at(objIndex);
        if (intersection = obj->rayIntersectP(cameraRay))
        {
            return intersection;
        }
    }
    return intersection;
}


//Intersection Utils::getClosestIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList, Intersection& its) //or Closest Hit Ray
//{
//    bool rayInters = false;
//    for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
//    {
//        const Shape* obj = objectsList.at(objIndex);
//        Intersection intersection;
//        rayInters = obj->rayIntersect(cameraRay, its);
//        if (rayInters == true)
//        {
//            return its;
//        }
//    }
//    return its;
//}

//bool Utils::getClosestIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList, Intersection& its) //or Closest Hit Ray
//{
//    bool rayInters = false;
//    for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
//    {
//        const Shape* obj = objectsList.at(objIndex);
//        rayInters = obj->rayIntersect(cameraRay, its);
//        if (rayInters == true)
//        {
//            return rayInters;
//        }
//    }
//    return rayInters;
//}
 
 
//bool Utils::getClosestIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList, Intersection& its) //or Closest Hit Ray
//{
//    //std::cout << "Need to implement the function Utils::getClosestIntersection() in the file utils.cpp" << std::endl;
//
//    //
//    // Write your code bellow
//    const Shape* obj = objectsList.at(0);
//    obj->rayIntersect(cameraRay, its);
//    Intersection* currentIntersection = new Intersection();
//    Intersection& referenceToIntersection = *currentIntersection;
//
//    for (size_t objIndex = 1; objIndex < objectsList.size(); objIndex++)
//    {
//        // Get the current object
//        const Shape* obj = objectsList.at(objIndex);
//        obj->rayIntersect(cameraRay, referenceToIntersection);
//
//        if (std::sqrt(pow(referenceToIntersection.itsPoint.x - cameraRay.o.x, 2) + pow(referenceToIntersection.itsPoint.y - cameraRay.o.y, 2) + pow(referenceToIntersection.itsPoint.z - cameraRay.o.z, 2)) < std::sqrt(pow(its.itsPoint.x - cameraRay.o.x, 2) + pow(its.itsPoint.y - cameraRay.o.y, 2) + pow(its.itsPoint.z - cameraRay.o.z, 2)))
//        {
//            its.itsPoint.x = referenceToIntersection.itsPoint.x;
//            its.itsPoint.y = referenceToIntersection.itsPoint.y;
//            its.itsPoint.z = referenceToIntersection.itsPoint.z;
//
//        }
//    }
//
//    delete currentIntersection;
//    return false;
//}

bool Utils::getClosestIntersection(const Ray& cameraRay, const std::vector<Shape*>& objectsList, Intersection& its)
{
    const Shape* obj = objectsList.at(0);
    Intersection intersection;

    for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
    {
        const Shape* obj = objectsList.at(objIndex);
        
        if (obj->rayIntersect(cameraRay, intersection))
        {
            return true
        }
        if ((intersection.itsPoint - cameraRay.o).length() <(its.itsPoint - cameraRay.o).length())
        {
            its = intersection
        }
    }
    return false;
}

double interpolate(double val, double y0, double x0, double y1, double x1 )
{
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

double getRed(double value)
{
    if (value > 0.5)
        return interpolate( value, 0.0, 0.5, 1.0, 1.0 );
    else
        return 0.0;
}

double getGreen(double value)
{
    if (value < 0.25)
        return 0.0;
    else if (value < 0.5)
        return interpolate(value, 0.0, 0.25, 1, 0.5);
    else if (value < 0.75)
        return interpolate(value, 1, 0.5, 0, 0.75);
    else
        return 0;
}

double getBlue(double value)
{
    if (value < 0.5)
        return interpolate(value, 1.0, 0.0, 0.0, 0.5);
    else
        return 0;
}


Vector3D Utils::scalarToRGB(double scalar)
{
    return Vector3D( getRed(scalar),
                getGreen(scalar),
                getBlue(scalar) );
}

Vector3D Utils::computeReflectionDirection(const Vector3D &rayDirection, const Vector3D &normal)
{
    // Compute the perfect reflection direction
    return normal * 2 * dot(normal, -rayDirection) + rayDirection;
}

bool Utils::isTotalInternalReflection(const double &eta, const double &cosThetaI,
                                      double &cosThetaT_out)
{
    return false;
}

Vector3D Utils::computeTransmissionDirection(const Ray &r, const Vector3D &normal,
                                             const double &eta, const double &cosThetaI,
                                             const double &cosThetaT)
{
    return r.d * eta + normal * (eta*cosThetaI - cosThetaT);;
}

double Utils::computeReflectanceCoefficient(const double &eta, const double &cosThetaI,
                                            const double &cosThetaT)
{
    return 0.0;
}

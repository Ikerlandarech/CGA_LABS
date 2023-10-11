#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "material.h"


class Transmissive : public Material
{
public:
    Transmissive();
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
    double getIndexOfRefraction() const;
    Vector3D getDiffuseCoefficient() const;

private:
    double indRef;
};

#endif // TRANSMISSIVE_H
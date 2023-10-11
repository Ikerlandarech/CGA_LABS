#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "material.h"


class Transmissive : public Material
{
public:
    Transmissive();
    Transmissive(Vector3D kd_, Vector3D ks_, double alpha_);
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;

private:
    Vector3D kd;
    Vector3D ks;
    double alpha;
};

#endif // TRANSMISSIVE_H
#ifndef PHONG_H
#define PHONG_H

#include "material.h"


class Phong : public Material
{
public:
    Phong();
    Phong(Vector3D kd_, Vector3D ks_, double alpha_);
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
    Vector3D getDiffuseCoefficient() const;

private:
    Vector3D kd;
    Vector3D ks;
    double alpha;
};

#endif // PHONG_H

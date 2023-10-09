#include "phong.h"
#include "../core/utils.h"

Phong::Phong()
{
}

Phong::Phong(Vector3D kd_, Vector3D ks_, double alpha_) :
    kd(kd_), ks(ks_), alpha(alpha_)
{
}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
    double max = std::max(0.0, dot(n, wi));
    Vector3D reflectance = { 0, 0, 0 };
    Vector3D wr = ((n.operator*(dot(n, wi)) * 2.0) - wi).normalized();
    reflectance.x = kd.x * max * dot(wi, n) + ks.x * max *  pow(dot(wo, wr), alpha);
    reflectance.y = kd.y * max * dot(wi, n) + ks.y * max * pow(dot(wo, wr), alpha);
    reflectance.z = kd.z * max* dot(wi, n) + ks.z * max * pow(dot(wo, wr), alpha);

    return reflectance;
}

bool Phong::hasSpecular() const
{
    return false;
}

bool Phong::hasTransmission() const
{
    return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
    return false;
}

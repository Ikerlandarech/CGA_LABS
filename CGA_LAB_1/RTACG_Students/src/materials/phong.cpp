#include "phong.h"
#include "../core/utils.h"

Phong::Phong()
{
}

Phong::Phong(Vector3D kd_, Vector3D ks_, double alpha_) :
    kd(kd_), ks(ks_), alpha(alpha_)
{
}

//Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
//
//    Vector3D reflectance = Vector3D(0, 0, 0);
//    Vector3D wr = ((2 * (n.x * wi.x) * n.x - wi.x), (2 * (n.y * wi.y) * n.y - wi.y), (2 * (n.z * wi.z) * n.z - wi.z));
//    reflectance = ((kd.x * (wi.x * n.x) + ks.x * pow(wo.x * wr.x, alpha)), (kd.y * (wi.y * n.y) + ks.y * pow(wo.y * wr.y, alpha)), (kd.z * (wi.z * n.z) + ks.z * pow(wo.z * wr.z, alpha)));
//
//    return reflectance;
//}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
	Vector3D r = kd * (dot(wi, n) < 0 ? 0 : dot(wi, n));
	Vector3D calc_n = n;
	calc_n.x *= (2 * (dot(n, wi) < 0 ? 0 : dot(n, wi)));
	calc_n.y *= (2 * (dot(n, wi) < 0 ? 0 : dot(n, wi)));
	calc_n.z *= (2 * (dot(n, wi) < 0 ? 0 : dot(n, wi)));
	Vector3D wr = calc_n - wi;
	return r + (ks * pow((dot(wo, wr) < 0 ? 0 : dot(wo, wr)), alpha));
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

#include "ObjectBase.hpp"

#include <math.h>

#define EPSILON 1e-21f;

rt::ObjectBase::ObjectBase()
{

}

rt::ObjectBase::~ObjectBase()
{

}

bool rt::ObjectBase::TestIntersection(const Ray &castRay, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbVector<double> &localColor)
{
	return false; //placeholder
}

bool rt::ObjectBase::CloseEnough(const double f1, const double f2)
{
	return fabs(f1 - f2) < EPSILON;
}

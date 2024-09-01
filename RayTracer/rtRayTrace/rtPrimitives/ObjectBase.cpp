#include "ObjectBase.hpp"

#include <math.h>

#define EPSILON 1e-21f;

rt::ObjectBase::ObjectBase()
{

}

rt::ObjectBase::~ObjectBase()
{

}

bool rt::ObjectBase::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
	return false; //placeholder
}

void rt::ObjectBase::SetTransformMatrix(const rt::GTform &transformMatrix)
{
	transformMatrix_ = transformMatrix;
}

bool rt::ObjectBase::CloseEnough(const double f1, const double f2)
{
	return fabs(f1 - f2) < EPSILON;
}

bool rt::ObjectBase::AssignMaterial(const std::shared_ptr<rt::MaterialBase> &objectMaterial)
{
	pMaterial_ = objectMaterial;
	hasMaterial_ = true;

	return hasMaterial_;
}

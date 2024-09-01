#include "LightBase.hpp"

rt::LightBase::LightBase()
{

}

rt::LightBase::~LightBase()
{

}

bool rt::LightBase::ComputeIllumination(const qbVector3<double> &intPoint, const qbVector3<double> &localNormal, const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList, const std::shared_ptr<rt::ObjectBase> &currentObject, qbVector3<double> &color, double &intensity)
{
	return false; // placeholder
}


#include "LightBase.hpp"

rt::LightBase::LightBase()
{

}

rt::LightBase::~LightBase()
{

}

bool rt::LightBase::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal, const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList, const std::shared_ptr<rt::ObjectBase> &currentObject, qbVector<double> &color, double &intensity)
{
	return false; // placeholder
}


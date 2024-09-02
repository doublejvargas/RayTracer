#include "LightBase.hpp"

rt::LightBase::LightBase()
{

}

rt::LightBase::~LightBase()
{

}

bool rt::LightBase::ComputeIllumination(	const glm::dvec3 &intPoint,
											const glm::dvec3 &localNormal, 
											const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList, 
											const std::shared_ptr<rt::ObjectBase> &currentObject, glm::dvec3 &color, double &intensity
										)
{
	return false; // placeholder
}


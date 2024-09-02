#pragma once

#include "LightBase.hpp"

namespace rt
{
	class PointLight : public LightBase 
	{
	public:
		// Default constructor
		PointLight();

		// Override default destructor
		virtual ~PointLight() override;

		// Compute illumination
		virtual bool ComputeIllumination(	const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
											const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::shared_ptr<rt::ObjectBase> &currentObject,
											glm::dvec3 &color, double &intensity
										 ) override;
	};
}
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
		virtual bool ComputeIllumination
		(const qbVector3<double> &intPoint, const qbVector3<double> &localNormal,
			const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
			const std::shared_ptr<rt::ObjectBase> &currentObject,
			qbVector3<double> &color, double &intensity
		) override;
	};
}
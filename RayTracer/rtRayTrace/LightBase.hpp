#pragma once

#include "Ray.hpp"
#include "ObjectBase.hpp"

// tools
#include "toolbox/qbVector.h"

//lib
#include <memory>

namespace rt
{
	class LightBase
	{
	public:
		// Constructor / Destructor
		LightBase();
		virtual ~LightBase();

		// Compute illumination contribution
		virtual bool ComputeIllumination
		(	const qbVector<double> &intPoint, const qbVector<double> &localNormal,
			const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
			const std::shared_ptr<rt::ObjectBase> &currentObject,
			qbVector<double> &color, double &intensity
		);

	public:
		qbVector<double> m_Color	{ 3 };
		qbVector<double> m_Location	{ 3 };
		double m_Intensity			{ 3 };
											

	};
}
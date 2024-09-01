#pragma once

#include "rtRayTrace/Ray.hpp"
#include "rtRayTrace/rtPrimitives/ObjectBase.hpp"

// tools
#include "toolbox/qbVector3.hpp"

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
		
		//************************************
		// intPoint : point of intersection of light ray with object in world space
		// localNormal: surface normal at intPoint
		// objectList: reference to all objects in the scene, used to compute shadows 
		// currentObject: 
		// intensity: computed intensity at intPoint.
		//************************************
		virtual bool ComputeIllumination
		(	const qbVector3<double> &intPoint, const qbVector3<double> &localNormal,
			const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
			const std::shared_ptr<rt::ObjectBase> &currentObject,
			qbVector3<double> &color, double &intensity
		);

	public:
		qbVector3<double> m_Color	{ 3 };	// Color of light source
		qbVector3<double> m_Location	{ 3 };  // Location of light source
		double m_Intensity			{ 3 };	// Intensity [0, 1]
		
	};
}
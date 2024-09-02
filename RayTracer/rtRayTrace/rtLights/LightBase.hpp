#pragma once

#include "rtRayTrace/Ray.hpp"
#include "rtRayTrace/rtPrimitives/ObjectBase.hpp"

// tools
//#include "toolbox/qbVector3.hpp"
#include "toolbox/glm/glm.hpp"

//lib
#include <memory>
#include <vector>

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
		(	const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
			const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
			const std::shared_ptr<rt::ObjectBase> &currentObject,
			glm::dvec3 &color, double &intensity
		);

	public:
		glm::dvec3 m_Color		{ 0.0 };	// Color of light source
		glm::dvec3 m_Location	{ 0.0 };  // Location of light source
		double m_Intensity;	// Intensity [0, 1]
		
	};
}
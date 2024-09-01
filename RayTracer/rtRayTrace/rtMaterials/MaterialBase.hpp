#pragma once

#include "rtPrimitives/ObjectBase.hpp"
#include "rtLights/LightBase.hpp"
#include "toolbox/qbVector3.hpp"
#include "Ray.hpp"

// lib
#include <memory>

namespace rt
{
	class MaterialBase
	{
	public:
		MaterialBase();
		virtual ~MaterialBase();

		// Return the color of the material
		virtual qbVector3<double> ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
												const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
												const std::shared_ptr<rt::ObjectBase> &currentObject,
												const qbVector3<double> &intPoint, const qbVector3<double> &localNormal,
												const rt::Ray &cameraRay
											 );

		// Compute diffuse color
		static qbVector3<double> ComputeDiffuseColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
														const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
														const std::shared_ptr<rt::ObjectBase> &currentObject,
														const qbVector3<double> &intPoint, const qbVector3<double> &localNormal,
														const qbVector3<double> &baseColor
												   );

		// Compute the reflection color
		qbVector3<double> ComputeReflectionColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
													const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
													const std::shared_ptr<rt::ObjectBase> &currentObject,
													const qbVector3<double> &intPoint, const qbVector3<double> &localNormal,
													const rt::Ray &incidentRay
											   );

		// Cast a ray into the scene
		bool CastRay(	const rt::Ray &castRay, 
						const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
						const std::shared_ptr<rt::ObjectBase> &thisObject,
						std::shared_ptr<rt::ObjectBase> &closestObject,
						qbVector3<double> &closestIntPoint,
						qbVector3<double> &closestLocalNormal,
						qbVector3<double> &closestLocalColor
					);

	public:
		// Counter for the number of reflection rays
		// These are static variables because we want them to be initialized once, and for all MaterialBase objects to share this data.
		inline static int maxReflectionRays_;
		inline static int reflectionRayCount_;

	private:
	};
}
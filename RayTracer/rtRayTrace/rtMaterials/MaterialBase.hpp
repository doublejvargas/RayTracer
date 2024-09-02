#pragma once

#include "rtPrimitives/ObjectBase.hpp"
#include "rtLights/LightBase.hpp"
//#include "toolbox/qbVector3.hpp"
#include "Ray.hpp"

// tool
#include "toolbox/glm/glm.hpp"

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
		virtual glm::dvec3 ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
											const std::shared_ptr<rt::ObjectBase> &currentObject,
											const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
											const rt::Ray &cameraRay
									   );

		// Compute diffuse color
		static glm::dvec3 ComputeDiffuseColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
												const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
												const std::shared_ptr<rt::ObjectBase> &currentObject,
												const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
												const glm::dvec3 &baseColor
											);

		// Compute the reflection color
		glm::dvec3 ComputeReflectionColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
											const std::shared_ptr<rt::ObjectBase> &currentObject,
											const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
											const rt::Ray &incidentRay
										);

		// Cast a ray into the scene
		bool CastRay(	const rt::Ray &castRay, 
						const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
						const std::shared_ptr<rt::ObjectBase> &thisObject,
						std::shared_ptr<rt::ObjectBase> &closestObject,
						glm::dvec3 &closestIntPoint,
						glm::dvec3 &closestLocalNormal,
						glm::dvec3 &closestLocalColor
					);

	public:
		// Counter for the number of reflection rays
		// These are static variables because we want them to be initialized once, and for all MaterialBase objects to share this data.
		inline static int maxReflectionRays_;
		inline static int reflectionRayCount_;

	private:
	};
}
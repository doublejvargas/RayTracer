#pragma once

#include "MaterialBase.hpp"


namespace rt
{
	class SimpleMaterial : public MaterialBase
	{
	public:
		// Constructor / Destructor
		SimpleMaterial();
		virtual ~SimpleMaterial() override;

		// Return the color of the material
		virtual glm::dvec3 ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
											const std::shared_ptr<rt::ObjectBase> &currentObject,
											const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
											const rt::Ray &cameraRay
									   ) override;

		// Compute specular highlights
		glm::dvec3 ComputeSpecular(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
									const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
									const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
									const rt::Ray &cameraRay
								 );
	public:
		glm::dvec3 baseColor_{ 1.0, 0.0, 1.0 };
		double reflectivity_ = 0.0;
		double shininess_ = 0.0;
	};
}

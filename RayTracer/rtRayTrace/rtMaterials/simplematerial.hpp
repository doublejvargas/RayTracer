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
		virtual qbVector<double> ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
												const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
												const std::shared_ptr<rt::ObjectBase> &currentObject,
												const qbVector<double> &intPoint, const qbVector<double> &localNormal,
												const rt::Ray &cameraRay
											  ) override;

		// Compute specular highlights
		qbVector<double> ComputeSpecular(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
											const qbVector<double> &intPoint, const qbVector<double> &localNormal,
											const rt::Ray &cameraRay
										);
	public:
		qbVector<double> baseColor_{ std::vector<double>{1.0, 0.0, 1.0} };
		double reflectivity_ = 0.0;
		double shininess_ = 0.0;

	};
}

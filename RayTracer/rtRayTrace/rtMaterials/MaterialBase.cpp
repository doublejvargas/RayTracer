#include "MaterialBase.hpp"

rt::MaterialBase::MaterialBase()
{

}

rt::MaterialBase::~MaterialBase()
{

}

qbVector<double> rt::MaterialBase::ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
													const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
													const std::shared_ptr<rt::ObjectBase> &currentObject,
													const qbVector<double> &intPoint, const qbVector<double> &localNormal,
													const rt::Ray &cameraRay
												)
{
	// Define an initial material color
	qbVector<double> matColor	{ 3 };

	return matColor;
}

qbVector<double> rt::MaterialBase::ComputeDiffuseColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
														const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
														const std::shared_ptr<rt::ObjectBase> &currentObject,
														const qbVector<double> &intPoint, const qbVector<double> &localNormal,
														const qbVector<double> &baseColor
													  )
{
	// Compute the color due to diffuse illumination
	qbVector<double> diffuseColor	{ 3 };
	double intensity;
	qbVector<double> color	{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	bool validIllum = false;
	bool illumFound = false;
	for (auto currentLight : lightList)
	{
		validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);
		if (validIllum)
		{
			illumFound = true;
			red		+= color.GetElement(0) * intensity;
			green	+= color.GetElement(1) * intensity;
			blue	+= color.GetElement(2) * intensity;
		}
	}

	if (illumFound)
	{
		diffuseColor.SetElement(0, red	 * baseColor.GetElement(0));
		diffuseColor.SetElement(1, green * baseColor.GetElement(1));
		diffuseColor.SetElement(2, blue	 * baseColor.GetElement(2));
	}

	// Return the material color
	return diffuseColor;
}

bool rt::MaterialBase::CastRay(	const rt::Ray &castRay,
								const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
								const std::shared_ptr<rt::ObjectBase> &thisObject,
								std::shared_ptr<rt::ObjectBase> &closestObject,
								qbVector<double> &closestIntPoint,
								qbVector<double> &closestLocalNormal,
								qbVector<double> &closestLocalColor
							  )
{

}


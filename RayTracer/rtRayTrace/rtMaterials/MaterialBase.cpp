#include "MaterialBase.hpp"

rt::MaterialBase::MaterialBase()
{
	maxReflectionRays_ = 3;
	reflectionRayCount_ = 0;
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

qbVector<double> rt::MaterialBase::ComputeReflectionColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
															const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
															const std::shared_ptr<rt::ObjectBase> &currentObject,
															const qbVector<double> &intPoint, const qbVector<double> &localNormal,
															const rt::Ray &incidentRay
														)
{
	qbVector<double> reflectionColor	{ 3 };

	// Compute the reflection vector
	qbVector<double> d = incidentRay.lab;
	qbVector<double> reflectionVector = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);

	// Construct the reflection ray
	rt::Ray reflectionRay{ intPoint, intPoint + reflectionVector };

	/* Cast this ray into the scene and find the closest object that it intersects with */
	std::shared_ptr<rt::ObjectBase> closestObject;
	qbVector<double> closestIntPoint	{ 3 };
	qbVector<double> closestLocalNormal	{ 3 };
	qbVector<double> closestLocalColor	{ 3 };
	bool intersectionFound = CastRay(	reflectionRay, objectList, currentObject,
										closestObject, closestIntPoint, closestLocalNormal, closestLocalColor
									);
	/* Compute illumination for closest object assuming that there was a valid intersection */
	qbVector<double> matColor	{ 3 };
	if ((intersectionFound) and (reflectionRayCount_ < maxReflectionRays_))
	{
		// Increment the reflectionRayCount in order to avoid infinite recursion
		reflectionRayCount_++;

		// Check if a material has been assigned
		if (closestObject->hasMaterial_)
		{
			/* Use the material to compute the color *RECURSIVE CALL HERE*
			*	Recursive here because ComputeColor calls ComputeReflectionColor
			*/
			matColor = closestObject->pMaterial_->ComputeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, reflectionRay);
		}
		else
		{
			matColor = rt::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->baseColor_);
		}
	}
	else // No intersection 
	{
		// Leave matColor as it is
	}

	reflectionColor = matColor;
	return reflectionColor;
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
	// Test for intersections with all of the objects in the scene
	qbVector<double> intPoint		{ 3 };
	qbVector<double> localNormal	{ 3 };
	qbVector<double> localColor		{ 3 };

	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : objectList)
	{
		if (currentObject != thisObject)
		{
			bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

			// If we have a valid intersection
			if (validInt)
			{
				// Set the flag to show that we found an intersection
				intersectionFound = true;

				// Compute the distance between the source and the intersection point
				double dist = (intPoint - castRay.p1).norm();

				// Store a reference to this object if it is the closest
				if (dist < minDist)
				{
					minDist = dist;
					closestObject = currentObject;
					closestIntPoint = intPoint;
					closestLocalNormal = localNormal;
					closestLocalColor = localColor;
				}
			}
		}
	}

	return intersectionFound;
}
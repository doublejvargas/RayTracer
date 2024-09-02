#include "MaterialBase.hpp"

rt::MaterialBase::MaterialBase()
{
	maxReflectionRays_ = 3;
	reflectionRayCount_ = 0;
}

rt::MaterialBase::~MaterialBase()
{

}

glm::dvec3 rt::MaterialBase::ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
											const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
											const std::shared_ptr<rt::ObjectBase> &currentObject,
											const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
											const rt::Ray &cameraRay
										)
{
	// Define an initial material color
	glm::dvec3 matColor { 0.0 };

	return matColor;
}

glm::dvec3 rt::MaterialBase::ComputeDiffuseColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
													const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
													const std::shared_ptr<rt::ObjectBase> &currentObject,
													const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
													const glm::dvec3 &baseColor
												)
{
	// Compute the color due to diffuse illumination
	glm::dvec3 diffuseColor	{ 0.0 };
	glm::dvec3 color		{ 0.0 };
	double intensity = 0.0;
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
			red		+= color.r * intensity;
			green	+= color.g * intensity;
			blue	+= color.b * intensity;
		}
	}

	if (illumFound)
	{
		diffuseColor.r = red	* baseColor.r;
		diffuseColor.g = green	* baseColor.g;
		diffuseColor.b = blue	* baseColor.b;

	}

	// Return the material color
	return diffuseColor;
}

glm::dvec3 rt::MaterialBase::ComputeReflectionColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
														const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
														const std::shared_ptr<rt::ObjectBase> &currentObject,
														const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
														const rt::Ray &incidentRay
												   )
{
	glm::dvec3 reflectionColor	{ 0.0 };

	// Compute the reflection vector
	glm::dvec3 d = incidentRay.lab;
	glm::dvec3 reflectionVector = d - (2 * glm::dot(d, localNormal) * localNormal);

	// Construct the reflection ray
	rt::Ray reflectionRay{ intPoint, intPoint + reflectionVector };

	/* Cast this ray into the scene and find the closest object that it intersects with */
	std::shared_ptr<rt::ObjectBase> closestObject = nullptr;
	glm::dvec3 closestIntPoint		{ 0.0 };
	glm::dvec3 closestLocalNormal	{ 0.0 };
	glm::dvec3 closestLocalColor	{ 0.0 };
	bool intersectionFound = CastRay(	reflectionRay, objectList, currentObject,
										closestObject, closestIntPoint, closestLocalNormal, closestLocalColor
									);
	/* Compute illumination for closest object assuming that there was a valid intersection */
	glm::dvec3 matColor{ 0.0 };
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
								glm::dvec3 &closestIntPoint,
								glm::dvec3 &closestLocalNormal,
								glm::dvec3 &closestLocalColor
							  )
{
	// Test for intersections with all of the objects in the scene
	glm::dvec3 intPoint		{ 0.0 };
	glm::dvec3 localNormal	{ 0.0 };
	glm::dvec3 localColor	{ 0.0 };

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
				double dist = glm::length((intPoint - castRay.p1));

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
#include "simplematerial.hpp"

rt::SimpleMaterial::SimpleMaterial()
{

}

rt::SimpleMaterial::~SimpleMaterial()
{

}

glm::dvec3 rt::SimpleMaterial::ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
												const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
												const std::shared_ptr<rt::ObjectBase> &currentObject,
												const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
												const rt::Ray &cameraRay
										   )
{
	// Define the initial material colors
	glm::dvec3 matColor { 0.0 };
	glm::dvec3 refColor { 0.0 };
	glm::dvec3 difColor { 0.0 };
	glm::dvec3 spcColor { 0.0 };

	// Compute the diffuse component
	difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, baseColor_);

	// Compute the reflection component
	if (reflectivity_ > 0.0)
		refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

	// Combine reflection and diffuse components
	matColor = (reflectivity_ * refColor) + ((1 - reflectivity_) * difColor);

	// Compute the specular component
	if (shininess_ > 0.0)
		spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

	// Add the specular component to the final color
	matColor = matColor + spcColor;

	return matColor;
}

glm::dvec3 rt::SimpleMaterial::ComputeSpecular(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
														const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
														const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
														const rt::Ray &cameraRay
													)
{
	glm::dvec3 spcColor	{ 0.0 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	// Loop through all of the lights in the scene
	for (auto currentLight : lightList)
	{
		/* Check for intersections with all objects in the scene */ 
		
		
		double intensity = 0.0;
		// Construct a unit vector pointing from the intersection point to the light
		glm::dvec3 toLightDir{ glm::normalize((currentLight->m_Location - intPoint)) };
		// Compute a start point
		glm::dvec3 startPoint{ intPoint + (toLightDir * 0.001) };
		// Construct a ray from the point of intersection to the light
		rt::Ray toLightRay{ startPoint, startPoint + toLightDir };

		/* Loop through all objects in the scene to check if
		   any obstruct light from this source */
		glm::dvec3 poi			{ 0.0 };
		glm::dvec3 poiNormal	{ 0.0 };
		glm::dvec3 poiColor		{ 0.0 };
		bool validInt = false;
		for (auto sceneObject : objectList)
		{
			validInt = sceneObject->TestIntersection(toLightRay, poi, poiNormal, poiColor);
			if (validInt)
				break;
		}

		/* If no intersections were found, then proceed with
		    computing the specular component*/

		if (!validInt)
		{
			/* Eq for reflection vector:
			 *	^r = ^l - 2(^l·^n)*^n , where ^l is the vector from the light source to the surface and ^n is the normal vector of the surface
			 *	(NOTE that in our computations here, ^l is actually a vector pointing from the intersection point to the light and not vice versa)
			 *		So we are computing the projection of ^l into ^n, scaling it by -2 and giving it the direction of ^n, and finally subtracting this result
			 *      from ^l.
			 * Eq for specular intensity as a function of reflection vector and observer:
			 *	I_sv = I_l*(^r·^v)^a, where I_l is the intensity of the light from source, ^r is the reflection vector (see prev equation),
			 *							^v is the vector from the observer to the intersection point, and a is a scalar representing shininess
			 *								
			*/

			// Compute the reflection vector
			glm::dvec3 d = toLightRay.lab;

			// ^r = ^l - 2(^l·^n)*^n 
			glm::dvec3 r = glm::normalize(d - (2 * glm::dot(d, localNormal) * localNormal));
			// Compute the dot product
			glm::dvec3 v = glm::normalize(cameraRay.lab);
			double dotProduct = glm::dot(r, v);

			// Only proceed if the dot product is positive
			if (dotProduct > 0.0)
			{
				// I_sv = I_l*(^r·^v)^a, reflectivity is not reflected in the equation but its present here as a material property that does affect final intensity.
				intensity = reflectivity_ * std::pow(dotProduct, shininess_);
			}
		}

		red		+= currentLight->m_Color.r * intensity;
		green	+= currentLight->m_Color.g * intensity;
		blue	+= currentLight->m_Color.b * intensity;
	}

	spcColor.r = red;
	spcColor.g = green;
	spcColor.b = blue;

	return spcColor;
}
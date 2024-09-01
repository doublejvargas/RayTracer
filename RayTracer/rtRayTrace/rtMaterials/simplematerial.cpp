#include "simplematerial.hpp"

rt::SimpleMaterial::SimpleMaterial()
{

}

rt::SimpleMaterial::~SimpleMaterial()
{

}

qbVector<double> rt::SimpleMaterial::ComputeColor(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
													const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
													const std::shared_ptr<rt::ObjectBase> &currentObject,
													const qbVector<double> &intPoint, const qbVector<double> &localNormal,
													const rt::Ray &cameraRay
												 )
{
	// Define the initial material colors
	qbVector<double> matColor	{3};
	qbVector<double> refColor	{3};
	qbVector<double> difColor	{3};
	qbVector<double> spcColor	{3};

	// Compute the diffuse component
	difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, baseColor_);

	// Compute the reflection component
	if (reflectivity_ > 0.0)
		refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

	// Combine reflection and diffuse components
	matColor = (refColor * reflectivity_) + (difColor * (1 - reflectivity_));

	// Compute the specular component
	if (shininess_ > 0.0)
		spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

	// Add the specular component to the final color
	matColor = matColor + spcColor;

	return matColor;
}

qbVector<double> rt::SimpleMaterial::ComputeSpecular(	const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList,
														const std::vector<std::shared_ptr<rt::LightBase>> &lightList,
														const qbVector<double> &intPoint, const qbVector<double> &localNormal,
														const rt::Ray &cameraRay
													)
{
	qbVector<double> spcColor	{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	// Loop through all of the lights in the scene
	for (auto currentLight : lightList)
	{
		/* Check for intersections with all objects in the scene */ 
		
		
		double intensity = 0.0;
		// Construct a unit vector pointing from the intersection point to the light
		qbVector<double> toLightDir{ (currentLight->m_Location - intPoint).Normalized() };
		// Compute a start point
		qbVector<double> startPoint{ intPoint + (toLightDir * .001) };
		// Construct a ray from the point of intersection to the light
		rt::Ray toLightRay{ startPoint, startPoint + toLightDir };

		/* Loop through all objects in the scene to check if
		   any obstruct light from this source */
		qbVector<double> poi		{ 3 };
		qbVector<double> poiNormal	{ 3 };
		qbVector<double> poiColor	{ 3 };
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
			qbVector<double> d = toLightRay.lab;
				// ^r = ^l - 2(^l·^n)*^n 
			qbVector<double> r = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);
			r.Normalize();

			// Compute the dot product
			qbVector<double> v = cameraRay.lab;
			v.Normalize();
			double dotProduct = qbVector<double>::dot(r, v);

			// Only proceed if the dot product is positive
			if (dotProduct > 0.0)
			{
				// I_sv = I_l*(^r·^v)^a, reflectivity is not reflected in the equation but its present here as a material property that does affect final intensity.
				intensity = reflectivity_ * std::pow(dotProduct, shininess_);
			}
		}

		red += currentLight->m_Color.GetElement(0) * intensity;
		green += currentLight->m_Color.GetElement(1) * intensity;
		blue += currentLight->m_Color.GetElement(2) * intensity;
	}

	spcColor.SetElement(0, red);
	spcColor.SetElement(1, green);
	spcColor.SetElement(2, blue);

	return spcColor;
}
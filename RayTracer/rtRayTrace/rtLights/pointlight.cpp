#include "pointlight.hpp"

rt::PointLight::PointLight()
{
	m_Color = qbVector3<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	m_Intensity = 1.0;
}

rt::PointLight::~PointLight()
{

}

bool rt::PointLight::ComputeIllumination(const qbVector3<double> &intPoint, const qbVector3<double> &localNormal, const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList, const std::shared_ptr<rt::ObjectBase> &currentObject, qbVector3<double> &color, double &intensity)
{
	// Construct a unit vector pointing from the intersection point to the light
	qbVector3<double> toLightDir = (m_Location - intPoint).Normalized();

	// Compute a starting point
	qbVector3<double> startPoint{ intPoint };

	// Construct a ray from point of intersection to the light
	// Here startPoint is a point, and the second argument is the sum of startPoint (a point) and toLightDir (a unit vector), which 
	//  yields another point in the direction of the light, effectively creating p1 and p2 that define a line (or ray) from intersection (start point) to light source
	rt::Ray	lightRay{ startPoint, startPoint + toLightDir };

	/* Check for intersection with all of the objects
	   in the scene, except for the current one */
	qbVector3<double> poi;
	qbVector3<double> poiNormal;
	qbVector3<double> poiColor;
	bool validInt = false;
	for (auto sceneObject : objectList)
	{
		if (sceneObject != currentObject) // This function call assumes light intersection with current object, so no need to check for it
		{
			validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
		}
		
		/* If we have an intersection, then there is no point checking further,
			so we can break out of the loop, i.e, this object is blocking light from
			this light source (and casting a shadow) */
		if (validInt)
			break;
	}

	/* Only continue to compute illumination if the light ray didn't
		intersect with any objects in the scene, i.e., no objects are
		casting a shadow from this light source */
	if (!validInt)
	{
		// Compute the angle between the local normal and the light ray
	// Note that we are assuming that localNormal from the parameters guaranteed to be a unit vector,
	// and we normalize toLightDir to be a unit vector as well.
	// This simplifies computation as per the formula for dot product: 
	//		^a·^b = |a||b| cos(theta) --> cos(theta) = (^a·^b)/ |a||b|, where ^a and ^b would be lightDir and localNormal,
	//       meaning that the denominator factors out to 1.
		double angle = acos(qbVector3<double>::dot(localNormal, toLightDir));
		if (angle > 3.1415926 / 2.0) // pi/2 = 180, if angle > 180, surface is facing away from light source.
		{
			// No illumination
			color = m_Color;
			intensity = 0.0;
			return false;
		}
		else // We do have illumination
		{
			color = m_Color;
			// intensity of light is directly proportional to the angle between the lightDir vector
			//  (which points from intersection of ray to light source) and the localNormal (which is the normal
			//   to the plane defined by the surface at the intersection).
			intensity = m_Intensity * (1.0 - (angle / (3.1415926 / 2.0)));
			return true;
		}
	}
	else // valid intersection --> Shadow, so no illumination
	{
		color = m_Color;
		intensity = 0.0;
		return false;
	}
}

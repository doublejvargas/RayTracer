#include "pointlight.hpp"

rt::PointLight::PointLight()
{
	m_Color = qbVector<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	m_Intensity = 1.0;
}

rt::PointLight::~PointLight()
{

}

bool rt::PointLight::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal, const std::vector<std::shared_ptr<rt::ObjectBase>> &objectList, const std::shared_ptr<rt::ObjectBase> &currentObject, qbVector<double> &color, double &intensity)
{
	// Construct a vector pointing from the intersection point to the light
	qbVector<double> lightDir = (m_Location - intPoint).Normalized();

	// Compute a starting point
	qbVector<double> startPoint = intPoint;

	// Compute the angle between the local normal and the light ray
	// Note that we are assuming that localNormal from the parameters guaranteed to be a unit vector,
	// and we normalize lightDir to be a unit vector as well.
	// This simplifies computation as per the formula for dot product: 
	//		^a·^b = |a||b| cos(theta) --> cos(theta) = (^a·^b)/ |a||b|, where ^a and ^b would be lightDir and localNormal,
	//       meaning that the denominator factors out to 1.
	double angle = acos(qbVector<double>::dot(localNormal, lightDir));
	if (angle > 3.1415926 / 2.0) // pi/2 = 180, if angle > 180, surface is facing away from light source.
	{
		// No illumination
		color = m_Color;
		intensity = 0.0;
		return false;
	}
	else
	{
		color = m_Color;
		// intensity of light is direclty proportional to the angle between the lightDir vector
		//  (which points from intersection of ray to light source) and the localNormal (which is the normal
		//   to the plane defined by the surface at the intersection).
		intensity = m_Intensity * (1.0 - (angle / (3.1415926 / 2.0)));
		return true;
	}
}

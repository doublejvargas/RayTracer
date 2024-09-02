#pragma once

#include "ObjectBase.hpp"
#include "gtfm.hpp"

namespace rt
{
	class ObjPlane : public ObjectBase
	{
	public:
		// Default constructor
		ObjPlane();

		// Override the destructor
		virtual ~ObjPlane() override;

		// Override the function to test intersections
		virtual bool TestIntersection(	const Ray &castRay, 
										glm::dvec3 &intPoint, 
										glm::dvec3 &localNormal, 
										glm::dvec3 &localColor) override;
	private:

	};
}

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
										qbVector<double> &intPoint, 
										qbVector<double> &localNormal, 
										qbVector<double> &localColor) override;
	private:

	};
}

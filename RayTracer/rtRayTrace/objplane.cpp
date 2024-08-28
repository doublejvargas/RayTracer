#include "objplane.hpp"

rt::ObjPlane::ObjPlane()
{

}

rt::ObjPlane::~ObjPlane()
{

}

bool rt::ObjPlane::TestIntersection(	const Ray &castRay,
										qbVector<double> &intPoint,
										qbVector<double> &localNormal,
										qbVector<double> &localColor)
{
	// Copy the ray and apply the backwards transform
	rt::Ray bckRay = transformMatrix_.Apply(castRay, rt::BCKTFORM);

	// Copy the m_lab vector from bckRay and normalize it
	qbVector<double> k = bckRay.lab;
	k.Normalize();

	/* Check if there is an intersection, ie. if the castRay is not parallel to the plane
		NOTE: this means parallel because the plane is the XY plane (where Z is 0 always),
		so, any vector that is parallel to this plane, will have a Z-component of 0, ie, element at index 2 of a qbVector{3} will be zero */
	if (!CloseEnough(k.GetElement(2), 0.0))
	{
		// If above condition true: there is an intersection bc vector is not parallel to plane

		/* Eq for a plane:
		*	^x = ^po + ^po1*u + ^po2*v, where ^po, ^po1 and ^po2 are points (3-component vectors) that define the plane, and u and v are scalars that define points along the plane 
		*  Eq for a line:
		*	 ^l = ^a + ^kt, where ^a is a point in space and ^k is a unit vector in the direction of the line, and t is a scalar representing points along the line.
		*  Eq for intersection:
		*	 ^a + ^kt = ^po + ^po1*u + ^po2*v
		* 
		*  Since we define all of our objects to be in their own local coordinate system, we can fix our plane to lie in the XY plane and to be centered at the origin, making
		*		^po =  [0 0 0] (origin)
		*		^po1 = [1 0 0] (vector pointing in x dir)
		*		^po2 = [0 1 0] (vector pointing in y dir)
		* 
		*  Then, we rewrite the intersection equation to:
		*		^a =  |1| +  |0|
		*			 u|0| + v|1| - ^kt
		*			  |0|    |0|
		* 
		*	or: ax = u - kx * t
		*		ay = v -ky * t
		*		az = -kz * t
		* 
		*   therefore:
		*		t = az / -kz
		* 
		*	This is the value that we compute below:
		*/
		double t = bckRay.p1.GetElement(2) / -k.GetElement(2);

		// If t is negative, then the intersection point must be behind the camera
		//  and we can ignore it
		if (t > 0.0)
		{
			// Compute the values for u and v
			// u = ax + kx * t
			double u = bckRay.p1.GetElement(0) + (k.GetElement(0) * t);
			// v = ay + ky * t
			double v = bckRay.p1.GetElement(1) + (k.GetElement(1) * t);

			/* If magnitude of both u and v is less than or equal to one, 
			   then we must be in the plane */
			if ((abs(u) < 1.0) && (abs(v) < 1.0))
			{
				// Compute the point of intersection
				// poi = ^a + ^k * t
				qbVector<double> poi = bckRay.p1 + (k * t);

				// Transform the intersection point back into world coordinates
				intPoint = transformMatrix_.Apply(poi, rt::FWDTFORM);

				// Compute the local normal (shouldn't this be global (world space) normal?)
				// origin in local object space
				qbVector<double> localOrigin{ std::vector<double>{0.0, 0.0, 0.0} };
				// normal to XY plane in local object space, vector with Z component (why negative?, perhaps because it is pointing toward camera pinhole!)
				qbVector<double> normalVector{ std::vector<double>{0.0, 0.0, -1.0} };
				// Origin transformed to world space
				qbVector<double> globalOrigin = transformMatrix_.Apply(localOrigin, rt::FWDTFORM);
				// Transform local normal (global?) to world space and get origin pointing toward normal
				localNormal = transformMatrix_.Apply(normalVector, rt::FWDTFORM) - globalOrigin;
				localNormal.Normalize(); // unit vector

				// Return base color
				localColor = baseColor_;

				return true;
			}
			else // abs(u) or abs(v) >= 1.0 --> no intersection
			{
				return false;
			}
		}
		else // t < 0, intersection behind camera --> ignored, t == 0, no intersection
		{
			return false;
		}
	}
	// ray is parallel to plane --> no intersection
	return false;
}
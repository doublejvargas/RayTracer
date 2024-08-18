#include "objsphere.hpp"

rt::ObjSphere::ObjSphere()
{

}

rt::ObjSphere::~ObjSphere()
{

}


/*	Equation for Sphere:
*		x^2 + y^2 + z^2 = r^2
*	
*	Equation for ray that intersects sphere:
*	  Matrix form:
*		|x|		|p1x| +  |vx|
*		|y| = 	|p1y| + t|vy|
*		|z|		|p1z| +  |vz|
*	  Notation:
*		ŝ = p̂1 + tv̂, where p̂1 is point one of the ray, v̂ is the unit vector in the direction from p̂1 to p̂2, and t is a scalar which yields different points along the ray
*		this expands to (p1x + t*vx) + (p1y + t*vy) + (p1z + t*vz), corresponding to x, y, z components
*	
*	Set both equations equal to each other to find points of intersection
*		(p1x + t*vx)^2 + (p1y + t*vy)^2 + (p1z + t*vz)^2 = r^2
*	distribute exponents:
*		t^2*vx^2 + 2p1x*vx*t + p1x^2 + t^2*vy^2 + 2p1y*vy*t + p1y^2 + t^2*vz^2 + 2p1z*vz*t + p1z^2 = r^2
*	rearrange like terms and factor:
*		t^2(vx^2 + vy^2 + vz^2) + 2t(p1x*vx + p1y*vy + p1z*vz) + p1x^2 + p1y^2 + p1z^2 = r^2
*	rewrite in vector notation:
*		t^2||v̂||^2 + 2t(p̂1·v̂1) + ||p̂1||^2 = r^2
*	simplify using fact that norms are equal to square root of dot product of vector by itself i.e, ||x|| = sqrt(x · x)
*		t^2(v̂·v̂) + 2t(p̂1·v̂) + (p̂1·p̂1) - r^2 = 0
* 
*	Notice that this simplification is of the form
*		at^2 + bt + c = 0
* 
*	to solve for t, we use the quadratic equation
*	t = [-b +- sqrt(b^2 - 4ac)] / 2a
* 
*	if the determinant is > 0, intersections are real, i.e. they exist. If determinant is < 0, intersections are imaginary ie they don't exist.
* 
*	Note: since a = (v̂·v̂), which is the dot product of two unit vectors, it is always equal to 1.0.
*/
bool rt::ObjSphere::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
	// Compute the values of a, b and c
	qbVector<double> vHat = castRay.lab;
	vHat.Normalize();

	/* Note that 'a' is equal to the squared magnitude of the direction of the cast ray. As this will be a unit vector,
	   we can conclude that the value of 'a' will always be 1.
	*/
	// double a = 1.0;

	double b = 2.0 * qbVector<double>::dot(castRay.p1, vHat);

	double c = qbVector<double>::dot(castRay.p1, castRay.p1) - 1.0;

	// Test whether we actually have an intersection
	double discriminant_squared = (b * b) - 4.0 * c;

	if (discriminant_squared > 0.0)
	{
		double discriminant = sqrt(discriminant_squared);
		double t1 = (-b + discriminant) / 2.0;
		double t2 = (-b - discriminant) / 2.0;

		/* If either t1 or t2 are negative, then at least part of the object is behind
		   the camera and so we will ignore it*/
		if ((t1 < 0.0) || (t2 < 0.0))
		{
			return false;
		}
		else
		{
			// Determine which point of intersection was closest to the camera
			if (t1 < t2)
			{
				intPoint = castRay.p1 + (vHat * t1);
			}
			else
			{
				intPoint = castRay.p1 + (vHat * t2);
			}

			return true;
		}
	}
	else
	{
		return false;
	}
}
#pragma once

#include "toolbox/qbVector3.hpp"
#include "Ray.hpp"

namespace rt
{
	class Camera
	{
	public:
		// Default constructor
		Camera();

		// Setters (set camera parameters)
		void SetPosition(const qbVector3<double> &newPos);		// argument must be a 3d vector
		void SetLookAt(const qbVector3<double> &newLookAt);		// argument must be a 3d vector
		void SetUpVector(const qbVector3<double> &upVector);			// argument must be a 3d vector
		void SetLength(double newLength);
		void SetProjScreenWidth(double newSize);
		void SetAspect(double newAspect);

		// Getters (return camera parameters)
		inline qbVector3<double> const GetPosition() { return m_cameraPosition; }
		inline qbVector3<double> const GetLookAt() { return m_cameraLookAt; }
		inline qbVector3<double> const GetUpVector() { return m_cameraUp; }
		inline qbVector3<double> const GetU() { return m_projectionScreenU; }
		inline qbVector3<double> const GetV() { return m_projectionScreenV; }
		inline qbVector3<double> const GetScreenCenter() { return m_projectionScreenCenter; }
		inline double const GetLength()					{ return m_toScreenLenght; }
		inline double const GetProjScreenWidth()		{ return m_screenWidth; }
		inline double const GetAspect()					{ return m_cameraAspectRatio; }

		// Update the camera geometry
		void UpdateCameraGeometry();

		// Generates a ray from camera pinhole to projected, virtual screen at point proScreenX and proScreenY and stores to cameraRay parameter.
		bool GenerateRay(float proScreenX, float proScreenY, rt::Ray &cameraRay);

	private:
		// Just like in Ray.hpp, we make the assumption of 3-Dimensional space here by having vectors of 3 components.
		qbVector3<double> m_cameraPosition;
		qbVector3<double> m_cameraLookAt;	
		qbVector3<double> m_cameraUp;		
		
		// This is the distance from the camera pinhole to the projection screen.
		double m_toScreenLenght;
		// This is the width of the projected virtual screen.
		double m_screenWidth;
		double m_cameraAspectRatio;
		// This is the vector from the camera to the lookAt position
		qbVector3<double> m_alignmentVector;
		// Projection screen basis U vector in camera space
		qbVector3<double> m_projectionScreenU;
		// Projection screen basis V vector in camera space
		qbVector3<double> m_projectionScreenV;
		// Projection screen center position in camera space
		qbVector3<double> m_projectionScreenCenter;

	};
}
#pragma once

#include "toolbox/qbVector.h"
#include "Ray.hpp"

namespace rt
{
	class Camera
	{
	public:
		// Default constructor
		Camera();

		// Setters (set camera parameters)
		void SetPosition(const qbVector<double> &newPos);		// argument must be a 3d vector
		void SetLookAt(const qbVector<double> &newLookAt);		// argument must be a 3d vector
		void SetUpVector(const qbVector<double> &upVector);			// argument must be a 3d vector
		void SetLength(double newLength);
		void SetProjScreenWidth(double newSize);
		void SetAspect(double newAspect);

		// Getters (return camera parameters)
		inline qbVector<double> const GetPosition()		{ return m_cameraPosition; }
		inline qbVector<double> const GetLookAt()		{ return m_cameraLookAt; }
		inline qbVector<double> const GetUpVector()		{ return m_cameraUp; }
		inline qbVector<double> const GetU()			{ return m_projectionScreenU; }
		inline qbVector<double> const GetV()			{ return m_projectionScreenV; }
		inline qbVector<double> const GetScreenCenter()	{ return m_projectionScreenCenter; }
		inline double const GetLength()					{ return m_toScreenLenght; }
		inline double const GetProjScreenWidth()		{ return m_screenWidth; }
		inline double const GetAspect()					{ return m_cameraAspectRatio; }

		// Update the camera geometry
		void UpdateCameraGeometry();

		// Generates a ray from camera pinhole to projected, virtual screen at point proScreenX and proScreenY and stores to cameraRay parameter.
		bool GenerateRay(float proScreenX, float proScreenY, rt::Ray &cameraRay);

	private:
		// Just like in Ray.hpp, we make the assumption of 3-Dimensional space here by having vectors of 3 components.
		qbVector<double> m_cameraPosition	{3};
		qbVector<double> m_cameraLookAt		{3};
		qbVector<double> m_cameraUp			{3};
		
		// This is the distance from the camera pinhole to the projection screen.
		double m_toScreenLenght;
		// This is the width of the projected virtual screen.
		double m_screenWidth;
		double m_cameraAspectRatio;
		// This is the vector from the camera to the lookAt position
		qbVector<double> m_alignmentVector			{3};
		// Projection screen basis U vector in camera space
		qbVector<double> m_projectionScreenU		{3};
		// Projection screen basis V vector in camera space
		qbVector<double> m_projectionScreenV		{3};
		// Projection screen center position in camera space
		qbVector<double> m_projectionScreenCenter	{3};

	};
}
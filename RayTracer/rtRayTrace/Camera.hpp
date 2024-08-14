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
		void SetPosition	(const qbVector<double> &newPos);
		void SetLookAt		(const qbVector<double> &newLookAt);
		void SetUp			(const qbVector<double> &upVector);
		void SetLength		(double newLength);
		void SetHorSize		(double newSize);
		void SetAspect		(double newAspect);

		// Getters (return camera parameters)
		inline qbVector<double> const GetPosition()		{ return m_cameraPosition; }
		inline qbVector<double> const GetLookAt()		{ return m_cameraLookAt; }
		inline qbVector<double> const GetUp()			{ return m_cameraUp; }
		inline qbVector<double> const GetU()			{ return m_projectionScreenU; }
		inline qbVector<double> const GetV()			{ return m_projectionScreenV; }
		inline qbVector<double> const GetScreenCenter() { return m_projectionScreenCenter; }
		inline double			const GetLength()		{ return m_cameraLength; }
		inline double			const GetHorSize()		{ return m_cameraHorSize; }
		inline double			const GetAspect()		{ return m_cameraAspectRatio; }

		// Update the camera geometry
		void UpdateCameraGeometry();

		// Generate a ray
		Ray GenerateRay(float proScreenX, float proScreenY);

	private:
		qbVector<double> m_cameraPosition	{3};
		qbVector<double> m_cameraLookAt		{3};
		qbVector<double> m_cameraUp			{3};
		double m_cameraLength;
		double m_cameraHorSize;
		double m_cameraAspectRatio;

		qbVector<double> m_alignmentVector			{3};
		qbVector<double> m_projectionScreenU		{3};
		qbVector<double> m_projectionScreenV		{3};
		qbVector<double> m_projectionScreenCenter	{3};

	};
}
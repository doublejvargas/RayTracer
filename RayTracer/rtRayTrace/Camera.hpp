#pragma once

//#include "toolbox/qbVector3.hpp"
#include "Ray.hpp"

// tool
#include "toolbox/glm/glm.hpp"

namespace rt
{
	class Camera
	{
	public:
		// Default constructor
		Camera();

		// Setters (set camera parameters)
		void SetPosition(const glm::dvec3 &newPos);		// argument must be a 3d vector
		void SetLookAt(const glm::dvec3 &newLookAt);		// argument must be a 3d vector
		void SetUpVector(const glm::dvec3 &upVector);			// argument must be a 3d vector
		void SetLength(double newLength);
		void SetProjScreenWidth(double newSize);
		void SetAspect(double newAspect);

		// Getters (return camera parameters)
		inline glm::dvec3 const GetPosition() { return m_cameraPosition; }
		inline glm::dvec3 const GetLookAt() { return m_cameraLookAt; }
		inline glm::dvec3 const GetUpVector() { return m_cameraUp; }
		inline glm::dvec3 const GetU() { return m_projectionScreenU; }
		inline glm::dvec3 const GetV() { return m_projectionScreenV; }
		inline glm::dvec3 const GetScreenCenter() { return m_projectionScreenCenter; }
		inline double const GetLength()					{ return m_toScreenLenght; }
		inline double const GetProjScreenWidth()		{ return m_screenWidth; }
		inline double const GetAspect()					{ return m_cameraAspectRatio; }

		// Update the camera geometry
		void UpdateCameraGeometry();

		// Generates a ray from camera pinhole to projected, virtual screen at point proScreenX and proScreenY and stores to cameraRay parameter.
		bool GenerateRay(double proScreenX, double proScreenY, rt::Ray &cameraRay);

	private:
		// Just like in Ray.hpp, we make the assumption of 3-Dimensional space here by having vectors of 3 components.
		glm::dvec3 m_cameraPosition;
		glm::dvec3 m_cameraLookAt;	
		glm::dvec3 m_cameraUp;		
		
		// This is the distance from the camera pinhole to the projection screen.
		double m_toScreenLenght;
		// This is the width of the projected virtual screen.
		double m_screenWidth;
		double m_cameraAspectRatio;
		// This is the vector from the camera to the lookAt position
		glm::dvec3 m_alignmentVector;
		// Projection screen basis U vector in camera space
		glm::dvec3 m_projectionScreenU;
		// Projection screen basis V vector in camera space
		glm::dvec3 m_projectionScreenV;
		// Projection screen center position in camera space
		glm::dvec3 m_projectionScreenCenter;

	};
}
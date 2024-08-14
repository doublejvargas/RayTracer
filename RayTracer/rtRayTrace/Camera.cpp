#include "Camera.hpp"

rt::Camera::Camera()
{
	m_cameraPosition = qbVector<double>{ std::vector<double>{0.0, -10.0, 0.0} };
	m_cameraLookAt = qbVector<double>{ std::vector<double>{0.0, 0.0, 0.0} };
	m_cameraUp = qbVector<double>{ std::vector<double>{0.0, 0.0, 1.0} };
	m_cameraLength = 1.0;
	m_cameraHorSize = 1.0;
	m_cameraAspectRatio = 1.0;
}

void rt::Camera::SetPosition(const qbVector<double> &newPos)
{
	m_cameraPosition = newPos;
}

void rt::Camera::SetLookAt(const qbVector<double> &newLookAt)
{
	m_cameraLookAt = newLookAt;
}

void rt::Camera::SetUp(const qbVector<double> &upVector)
{
	m_cameraUp = upVector;
}

void rt::Camera::SetLength(double newLength)
{
	m_cameraLength = newLength;
}

void rt::Camera::SetHorSize(double newSize)
{
	m_cameraHorSize = newSize;
}

void rt::Camera::SetAspect(double newAspect)
{
	m_cameraAspectRatio = newAspect;
}

void rt::Camera::UpdateCameraGeometry()
{
	// First, compute the vector from the camera position to the lookAt position
	m_alignmentVector = m_cameraLookAt - m_cameraPosition;
	m_alignmentVector.Normalize();

	// Second, compute the U and V vectors
	m_projectionScreenU = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
	m_projectionScreenU.Normalize();
	m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);
	m_projectionScreenV.Normalize();

	// Thirdly, compute the position of the center point of the screen
	m_projectionScreenCenter = m_cameraPosition + (m_cameraLength * m_alignmentVector);

	// Modify the U and V vectors to match the size and aspect ratio
	m_projectionScreenU = m_projectionScreenU * m_cameraHorSize;
	m_projectionScreenV = m_projectionScreenV * (m_cameraHorSize / m_cameraAspectRatio);
}

rt::Ray rt::Camera::GenerateRay(float proScreenX, float proScreenY)
{
	// Compute the location of the screen point in world coordinates
	qbVector<double> screenWorldPart1 = m_projectionScreenCenter + (m_projectionScreenU * proScreenX);
	qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

	// Use this point along with camera position to compute the ray
	return rt::Ray(m_cameraPosition, screenWorldCoordinate);
}
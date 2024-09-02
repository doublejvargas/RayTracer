#include "Camera.hpp"

rt::Camera::Camera()
{
	m_cameraPosition = glm::dvec3{ 0.0, -10.0, 0.0 };
	m_cameraLookAt = glm::dvec3{ 0.0, 0.0, 0.0 };
	m_cameraUp = glm::dvec3{ 0.0, 0.0, 1.0 };
	m_toScreenLenght = 1.0;
	m_screenWidth = 1.0;
	m_cameraAspectRatio = 1.0;
}

void rt::Camera::SetPosition(const glm::dvec3 &newPos)
{
	m_cameraPosition = newPos;
}

void rt::Camera::SetLookAt(const glm::dvec3 &newLookAt)
{
	m_cameraLookAt = newLookAt;
}

void rt::Camera::SetUpVector(const glm::dvec3 &upVector)
{
	m_cameraUp = upVector;
}

void rt::Camera::SetLength(double newLength)
{
	m_toScreenLenght = newLength;
}

void rt::Camera::SetProjScreenWidth(double newSize)
{
	m_screenWidth = newSize;
}

void rt::Camera::SetAspect(double newAspect)
{
	m_cameraAspectRatio = newAspect;
}

void rt::Camera::UpdateCameraGeometry()
{
	// First, compute the vector from the camera position to the lookAt position
	// The alignment vector passes directly from the camera pinhole, through the virtual projected screen
	// onto the object the rays are being casted onto. Therefore, the intersection with the projected
	// virtual screen denotes the center of the screen.
	m_alignmentVector = glm::normalize(m_cameraLookAt - m_cameraPosition);

	// Second, compute the U and V vectors

	/* The U basis vector in the projected screen is perpendicular to the plane defined by the Camera space's up vector
	*	and the vector from camera to "lookAt" position (alignmentVector). Therefore, to compute basis U vector in camera space,
	*   we compute the cross product of the alignment vector and the camera space's up vector.
	*/
	m_projectionScreenU = glm::normalize(glm::cross(m_alignmentVector, m_cameraUp));

	/* The V basis vector in the projected screen is perpendicular to the plane defined by U vector in camera space
	*	and the vector from camera to "lookAt" position (alignmentVector). Therefore, to compute basis V vector in camera space, 
	*   we compute the cross product of the U basis vector and the alignment vector.s
	*/
	m_projectionScreenV = glm::normalize(glm::cross(m_projectionScreenU, m_alignmentVector));

	// Thirdly, compute the position of the center point of the virtual projected screen
	m_projectionScreenCenter = m_cameraPosition + (m_toScreenLenght * m_alignmentVector);

	// Modify the U and V vectors to match the size and aspect ratio
	m_projectionScreenU = m_projectionScreenU * m_screenWidth;
	m_projectionScreenV = m_projectionScreenV * (m_screenWidth / m_cameraAspectRatio);
}

bool rt::Camera::GenerateRay(double proScreenX, double proScreenY, rt::Ray &cameraRay)
{ 
	// Compute the location of the virtual projected screen point in world space (ie, camera world coordinates)
	glm::dvec3 screenWorldX = m_projectionScreenCenter + (m_projectionScreenU * proScreenX);
	glm::dvec3 screenWorldCoordinate = screenWorldX + (m_projectionScreenV * proScreenY);

	// Use this point along with camera position to compute the ray and store to parameter cameraRay
	cameraRay.p1 = m_cameraPosition;
	cameraRay.p2 = screenWorldCoordinate;
	cameraRay.lab = screenWorldCoordinate - m_cameraPosition;
	
	return true;
}
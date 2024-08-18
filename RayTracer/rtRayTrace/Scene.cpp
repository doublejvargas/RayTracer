#include "Scene.hpp"

rt::Scene::Scene()
{
	// Configure the camera 
	m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, 0.0}});
	m_Camera.SetLookAt	(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
	m_Camera.SetUpVector(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_Camera.SetProjScreenWidth(0.25);
	m_Camera.SetAspect(16.0 / 9.0);
	m_Camera.UpdateCameraGeometry();
}

bool rt::Scene::Render(rtImage &outputImage)
{
	// Get the dimensions of the output image
	int width = outputImage.GetWidth();
	int height = outputImage.GetHeight();

	// Loop over each pixel in our image
	rt::Ray cameraRay;
	qbVector<double> intPoint	(3);
	qbVector<double> localNormal(3);
	qbVector<double> localColor	(3);
	// used to normalize the pixels from our typical dimensions (eg 1280 x 720) to range between [0, 1].
	double xFactor = 1.0 / (static_cast<double>(width) / 2.0);	// [0, 2]
	double yFactor = 1.0 / (static_cast<double>(height) / 2.0); // [0, 2]
	double minDist = 1e6;
	double maxDist = 0.0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			// Normalize x and y coordinates
			double normX = (static_cast<double>(x) * xFactor) - 1.0; // x * xFactor is in range [0, 2], so subtracting one yields range [-1, 1]
			double normY = (static_cast<double>(y) * yFactor) - 1.0;//  [-1, 1]

			// Generate the ray for this pixel
			m_Camera.GenerateRay(static_cast<float>(normX), static_cast<float>(normY), cameraRay); //note this returns a boolean! we might want to use that

			// Test if we have a valid intersection
			bool validIntersection = m_Sphere.TestIntersection(cameraRay, intPoint, localNormal, localColor);

			if (validIntersection)
			{
				outputImage.SetPixel(x, y, 255.0, .0, .0);
			}
			else
			{
				outputImage.SetPixel(x, y, .0, .0, .0);
			}
		}
	}

	return true;
}
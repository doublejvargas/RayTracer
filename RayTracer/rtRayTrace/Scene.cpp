#include "Scene.hpp"
#include "rtMaterials/simplematerial.hpp"

// lib
#include <chrono>

rt::Scene::Scene()
{
	// Set up the scene
	SetupSceneObjects();
}

rt::Scene::~Scene()
{

}

bool rt::Scene::Render(rtImage &outputImage)
{
	// Record the start time
	auto startTime = std::chrono::steady_clock::now();

	// Get the dimensions of the output image
	int width = outputImage.GetWidth();
	int height = outputImage.GetHeight();

	// Loop over each pixel in our image
	for (int y = 0; y < height; y++)
	{
		// Display progress
		std::cout << "Processing line " << y << " of " << height << "." << "\r";
		std::cout.flush();

		for (int x = 0; x < width; x++)
		{
			qbVector3<double> pixelColor = RenderPixel(x, y, width, height);
			outputImage.SetPixel(x, y, pixelColor.GetElement(0), pixelColor.GetElement(1), pixelColor.GetElement(2));
		}
	}
	// Record the end time
	auto endTime = std::chrono::steady_clock::now();

	// Compute the time it took to render
	std::chrono::duration<double> renderTime = endTime - startTime;
	std::cout.flush();
	std::cout << "\n\nRendering time: " << renderTime.count() << "s" << std::endl;

	return true;
}

void rt::Scene::RenderTile(rt::DATA::tile *tile)
{
	// Loop over each pixel in the tile
	qbVector3<double> pixelColor;
	for (int y = 0; y < tile->height; y++)
	{
		for (int x = 0; x < tile->width; x++)
		{
			pixelColor = RenderPixel(tile->x + x, tile->y + y, width_, height_);
			tile->rgbData.at(Sub2Ind(x, y, tile->width, tile->height)) = pixelColor;
		}
	}

	tile->renderComplete = true;
}

bool rt::Scene::CastRay(	rt::Ray &castRay, 
							std::shared_ptr<rt::ObjectBase> &closestObject,
							qbVector3<double> &closestIntPoint,
							qbVector3<double> &closestLocalNormal, 
							qbVector3<double> &closestLocalColor
						)
{
	qbVector3<double> intPoint;
	qbVector3<double> localNormal;
	qbVector3<double> localColor;
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : objectList_)
	{
		bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

		//If we have a valid intersection
		if (validInt)
		{
			// Set the flag to indicate that we found an intersection
			intersectionFound = true;

			// Compute distance between the camera and the point of intersection
			double dist = (intPoint - castRay.p1).norm();

			/* If this object is closer to the camera than any one that we have
			   seen before, then store a reference to it */
			if (dist < minDist)
			{
				minDist = dist;
				closestObject = currentObject;
				closestIntPoint = intPoint;
				closestLocalNormal = localNormal;
				closestLocalColor = localColor;
			}
		}
	}

	return intersectionFound;
}

// To be overridden
void rt::Scene::SetupSceneObjects()
{

}

qbVector3<double> rt::Scene::RenderPixel(int x, int y, int width, int height)
{
	std::shared_ptr<rt::ObjectBase> closestObject;
	rt::Ray	cameraRay;
	double xFactor = 1.0 / (static_cast<double>(width) / 2.0);
	double yFactor = 1.0 / (static_cast<double>(height) / 2.0);
	double minDist = 1e6;
	double maxDist = 0.0;
	qbVector3<double> outputColor;

	// Normalize x and y coordinates
	double normX = (static_cast<double>(x) * xFactor) - 1.0; // x * xFactor is in range [0, 2], so subtracting one yields range [-1, 1]
	double normY = (static_cast<double>(y) * yFactor) - 1.0;//  [-1, 1]

	// Generate the ray for this pixel
	camera_.GenerateRay(static_cast<float>(normX), static_cast<float>(normY), cameraRay); //note this returns a boolean! we might want to use that

	// Test for intersections with all objects in the scene
	qbVector3<double> closestIntPoint;
	qbVector3<double> closestLocalNormal;
	qbVector3<double> closestLocalColor;
	bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

	/* Compute the illumination for the closest object, assuming that there
			   was a valid intersection */
	if (intersectionFound)
	{
		// Check if object has a material
		if (closestObject->hasMaterial_)
		{
			// Use material to compute color
			rt::MaterialBase::reflectionRayCount_ = 0;
			qbVector3<double> color = closestObject->pMaterial_->ComputeColor(objectList_, lightList_, closestObject, closestIntPoint, closestLocalNormal, cameraRay);

			outputColor = color;
		}
		else
		{
			// Use basic method to compute color
			qbVector3<double> matColor = rt::MaterialBase::ComputeDiffuseColor(	objectList_, lightList_,
																				closestObject, closestIntPoint,
																				closestLocalNormal, closestObject->baseColor_
																			  );
			outputColor = matColor;
		}
	}

	return outputColor;
}

int rt::Scene::Sub2Ind(int x, int y, int width, int height)
{
	if ((x < width) and (x >= 0) and (y < height) and (y >= 0))
	{
		return (y * width) + x;
	}
	else
	{
		return -1;
	}
} 

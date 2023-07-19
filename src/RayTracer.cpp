
#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "globals.hpp"
#include "RayTracerMathLibrary.hpp"


#include <SFML/Graphics.hpp>

#include <limits>
#include <iostream>

RayTracer::RayTracer() : 
    RayTracerCamera{new Camera()}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()}
     {}

RayTracer::RayTracer(const Camera& RTCamera) : 
    RayTracerCamera{new Camera(RTCamera)}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()}
     {}


RayTracer::RayTracer(const Camera& RTCamera, float InitScreenWidth, float InitScreenHeight) : 
    RayTracerCamera{new Camera(RTCamera)}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()},
    ScreenWidth{InitScreenWidth},
    ScreenHeight{InitScreenHeight}
     {}


RayTracer::~RayTracer() 
{
    // Free up all memory used by the raytracer
    delete RayTracerCamera;
    delete ScreenTexture;
    delete RaytracedImage;

    for (SceneObject* shape : SceneObjects) 
    {
        delete shape;
    }
}


bool RayTracer::AddObject(int ObjectIndex, const SceneObject& ObjToAdd)
{
    // Create a copy of the object passed in
    SceneObject* NewObj = ObjToAdd.Clone();

    // Add the reference to the SceneObjects
    if (NewObj)
    {
        SceneObjects.emplace_back(NewObj);
        return true;
    }

    return false;
}

bool RayTracer::RemoveObject(int ObjIdx)
{   
    if (ObjIdx >= 0 && ObjIdx < SceneObjects.size())
    {
        // Valid Idx attemp to remove
        if (SceneObject* ObjToRemove = SceneObjects.at(ObjIdx))
        {
            SceneObjects.erase(SceneObjects.begin() + ObjIdx);
            delete ObjToRemove;
            return true;
        }
    }

    return false;
}
 

bool RayTracer::RefreshImage(sf::Texture& OutTexture)
{
    // This example shows another common use of sf::Texture:
    // streaming real-time data, like video frames
    
    // Create an empty texture Todo: Screen Size
    if (!ScreenTexture->create(ScreenWidth, ScreenHeight))
    {
        return false; 
    }
    
    // Create an Image and raytrace the scene
    RaytracedImage->create(ScreenWidth, ScreenHeight, sf::Color::White);

    for (int x = -ScreenWidth/2; x < ScreenWidth/2; x++) 
    {
        for (int y = -ScreenHeight/2; y < ScreenHeight/2; y++)
        {            
            // Calculate the color of the pixel at this location
            sf::Vector3f Direction = CanvasToViewport(x,y);
            Ray CurrentRay = Ray(RayTracerCamera->GetCameraLocation(), Direction);
            CurrentRay.SetLength(1.0);
            sf::Color PixelColor = TraceRay(CurrentRay, 1, std::numeric_limits<float>::infinity());
            
            // Set the color of the pixel
            RaytracedImage->setPixel(x + ScreenWidth/2, ScreenHeight - (y + ScreenHeight/2) - 1, PixelColor);
        }
    }



    // Set the OutTexture to be the resulting image
    ScreenTexture->loadFromImage(*RaytracedImage, sf::IntRect(0, 0, ScreenWidth, ScreenHeight));
    OutTexture = sf::Texture(*ScreenTexture);

    return true;
}


sf::Vector3f RayTracer::CanvasToViewport(int x, int y)
{
    const Camera::ViewPort ViewPort = RayTracerCamera->GetCameraViewPort();

    const float VpW = ViewPort.ViewportWidth;
    const float VpH = ViewPort.ViewportHeight;
    const float d = ViewPort.ViewportD;
    return sf::Vector3f(x * VpW/ScreenWidth, y * VpH/ScreenHeight, d);
}


sf::Color RayTracer::TraceRay(const Ray& CurrentRay, float TMin, float TMax)
{
    float ClosestT = std::numeric_limits<float>::infinity();
    SceneObject* ClosestSceneObject = nullptr;

    // TODO: Change to be SceneObjects
    // Loop throught the scene spheres
    for (const auto CurObject : SceneObjects)
    {

        // For now, cast to sphere
        const Sphere* AsSphere = static_cast<Sphere*>(CurObject);

        // Returns the result of the quadratic formula from the ray sphere intersection
        sf::Vector2f TValues = RayTracerMathLibrary::IntersectRaySphere(CurrentRay, AsSphere);

        const float t1 = TValues.x;
        const float t2 = TValues.y;
        // TODO
        if (TMin <= t1 && t1 <= TMax && t1 < ClosestT )
        {
            ClosestT = t1;
            ClosestSceneObject = CurObject;
        }

        if (TMin <= t2 && t2 <= TMax && t2 < ClosestT)
        {
            ClosestT = t2;
            ClosestSceneObject = CurObject;
        }
    }

    if (!ClosestSceneObject)
    {
        return sf::Color::White;
    }

    return ClosestSceneObject->ObjectColor;

}
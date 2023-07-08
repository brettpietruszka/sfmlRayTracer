
#include "RayTracer.hpp"
#include "Camera.hpp"
#include "globals.hpp"

#include <SFML/Graphics.hpp>

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
    if (!ScreenTexture->create(640, 480))
    {
        return false; 
    }
    
    // Create an Image and raytrace the scene
    RaytracedImage->create(ScreenWidth, ScreenHeight, sf::Color::White);

    bool bIsGreen = true;
    for (int x = 0; x < ScreenWidth; x++) 
    {
        for (int y = 0; y < ScreenHeight; y++)
        {
            // set the color of the pixel
            // loop through scene objects 
            if (y % 40 == 0)
            {
                bIsGreen = !bIsGreen;
                //RaytracedImage->setPixel(x, y, sf::Color::Green);

            }

            // Set pixel color
            if (x > y)
            {
                RaytracedImage->setPixel(x, y, sf::Color::Green);
            }
            else 
            {
                RaytracedImage->setPixel(x, y, sf::Color::Red);
            }

        }
    }



    // Todo: fix this line
    ScreenTexture->loadFromImage(*RaytracedImage, sf::IntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    OutTexture = sf::Texture(*ScreenTexture);

    return true;
}
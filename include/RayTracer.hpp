#pragma once

#include "Camera.hpp"
#include "SceneObjects.hpp"

#include <SFML/Graphics.hpp>


class Ray;

class RayTracer 
{

    // Todo: keep track of wasd,e,q and mouse for manuevering scene
    // std::map for InputBindings; ???

private:
    sf::Image* RaytracedImage = nullptr;
    sf::Texture* ScreenTexture = nullptr;
    Camera* RayTracerCamera = nullptr;

    float ScreenWidth = 0.0f;
    float ScreenHeight = 0.0f;

    std::vector<SceneObject*> SceneObjects;
    std::vector<SceneLight*> SceneLights;

public:
    /* Default constructor initializes a camera at (0,0,0) with orientation of (0,0,1) */
    RayTracer();
    RayTracer(const Camera& RTCamera);
    RayTracer(const Camera& RTCamera, float InitScreenWidth, float InitScreenHeight);
    ~RayTracer();

    /* Add and remove SceneObjects/SceneLights to the scene */
    bool AddObject(int ObjectIndex, const SceneObject& ObjToAdd);
    bool RemoveObject(int ObjIdx);

    bool AddLight(int ObjectIndex, const SceneLight& ObjToAdd);
    bool RemoveLight(int ObjIdx);
 
    /* Ray traces the current scene into a texture the size of the screen */
    bool RefreshImage(sf::Texture& OutTexture);

    /* Converts Screen coordinates to locations on the viewport */
    sf::Vector3f CanvasToViewport(int x, int y);

    
    /* Check for keyboard presses and relocate/rotate the current camera 
        as needed (wasd moves camera, arrow keys rotate)*/
    bool HandleInput();

private:

    /* Traces a single Ray through the scene and determines the color, if nothing is found withing 
        the time from Tmin to TmMax, it will return the color of the scene background 
        TODO: add scene background. for now, it is white */
    sf::Color TraceRay(const Ray& CurrentRay, float TMin, float TMax, int RecursiveDepth);

    /* Performs a line trace through the scene with a specified range of t values
        and return the intersection of the lowest t-value. If no intersection is 
        found, the intersection struct returned with have a ClosestObject of nullptr */
    Intersection ClosestIntersection(const Ray& CurrentRay, float TMin, float TMax);

    /* Designed for multithreading of the screen portions to speed up rendering for intput */
    void RayTraceScreenPortion(int xmin, int xmax, int ymin, int ymax);

    /* Returns the intensity of the lighting at a specific 
        given point with a given normal */
    float ComputeLighting(const sf::Vector3f& Point, const sf::Vector3f& Normal, const sf::Vector3f& V, const int& Shininess);

    /* Prevent this from being used */
    RayTracer& operator=(const RayTracer&);
};
#pragma once

#include "Camera.hpp"

#include <SFML/Graphics.hpp>


class Ray;

class RayTracer {

public:

    // TODO: move these SceneObjects into their own files???
    struct SceneObject
    {
        virtual SceneObject* Clone() const = 0;

        sf::Color ObjectColor;

        SceneObject() : ObjectColor{sf::Color::Red} {}
        SceneObject(sf::Color InitColor) : ObjectColor{InitColor} {}
        virtual ~SceneObject() {}
    };

private:
    sf::Image* RaytracedImage;
    sf::Texture* ScreenTexture;
    Camera* RayTracerCamera;

    float ScreenWidth = 0.0f;
    float ScreenHeight = 0.0f;

    std::vector<SceneObject*> SceneObjects;

public:

    struct Sphere : public SceneObject {
        sf::Vector3f Center {0.0f, 0.0f, 0.0f};
        float Radius {0.0f};

        Sphere() = default;
        Sphere(sf::Vector3f InitCenter, float InitRadius, sf::Color InitColor) :
            SceneObject(InitColor), Center{InitCenter}, Radius{InitRadius} {}
        Sphere(const Sphere& Other) : 
             SceneObject(Other.ObjectColor), Center{Other.Center}, Radius{Other.Radius} {}

        ~Sphere() override {}

        SceneObject* Clone() const override {return new Sphere(*this);}

    };


    /* Default constructor initializes a camera at (0,0,0) with orientation of (0,0,1) */
    RayTracer();
    RayTracer(const Camera& RTCamera);
    RayTracer(const Camera& RTCamera, float InitScreenWidth, float InitScreenHeight);
    ~RayTracer();

    /* Add and remove SceneObjects to the scene */
    bool AddObject(int ObjectIndex, const SceneObject& ObjToAdd);
    bool RemoveObject(int ObjIdx);
 
    /* Ray traces the current scene into a texture the size of the screen */
    bool RefreshImage(sf::Texture& OutTexture);

    /* Converts Screen coordinates to locations on the viewport */
    sf::Vector3f CanvasToViewport(int x, int y);

    /* Traces a single Ray through the scene and determines the color, if nothing is found withing 
        the time from Tmin to TmMax, it will return the color of the scene background 
        TODO: add scene background. for now, it is white */
    sf::Color TraceRay(const Ray& CurrentRay, float TMin, float TMax);

    // Todo: Input

};
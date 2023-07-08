#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.hpp"


class RayTracer {

public:

    // TODO: move these into their own files 
    struct SceneObject
    {
        virtual SceneObject* Clone() const = 0;
        
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

    struct Sphere : SceneObject {
        sf::Vector3f Center {0.0f, 0.0f, 0.0f};
        float Radius {0.0f};

        Sphere(const Sphere& Other) : 
            Center{Other.Center}, Radius{Other.Radius} {}

        ~Sphere() override {}

        SceneObject* Clone() const override {return new Sphere(*this);}

    };


    /* Default constructor initializes a camera at (0,0,0) with orientation of (0,0,1) */
    RayTracer();
    RayTracer(const Camera& RTCamera);
    RayTracer(const Camera& RTCamera, float InitScreenWidth, float InitScreenHeight);
    ~RayTracer();

    bool AddObject(int ObjectIndex, const SceneObject& ObjToAdd);
    bool RemoveObject(int ObjIdx);
 
    bool RefreshImage(sf::Texture& OutTexture);

    // Todo: Input

};
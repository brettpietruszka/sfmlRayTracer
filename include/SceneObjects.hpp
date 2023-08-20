
#pragma once

#include "Camera.hpp"

#include <SFML/Graphics.hpp>


struct SceneObject
{
    sf::Vector3f ObjectColor {255.0f, 0.0f, 0.0f};
    int Shininess = -1; // -1 means matte (not shiny)
    float Reflectiveness = 0.0f; // 0 is not reflective TODO: Clamp [0.0f,1.0f]

    SceneObject() : ObjectColor{sf::Vector3f(255.0f, 0.0f, 0.0f)}, Shininess{-1}, Reflectiveness{0.0f} {}
    SceneObject(sf::Vector3f InitColor, int InitShininess, float InitReflectiveness) : 
        ObjectColor{InitColor}, Shininess{InitShininess}, Reflectiveness{InitReflectiveness} {}
    virtual ~SceneObject() {}

     virtual SceneObject* Clone() const = 0;
};

struct Sphere : public SceneObject 
{
    sf::Vector3f Center {0.0f, 0.0f, 0.0f};
    float Radius {0.0f};

    Sphere() = default;
    Sphere(sf::Vector3f InitCenter, float InitRadius, sf::Vector3f InitColor, int InitShininess = -1, float InitReflectiveness = 0.0f) :
        SceneObject(InitColor, InitShininess, InitReflectiveness), Center{InitCenter}, Radius{InitRadius} {}
    Sphere(const Sphere& Other) : 
            SceneObject(Other.ObjectColor, Other.Shininess, Other.Reflectiveness), Center{Other.Center}, Radius{Other.Radius} {}

    ~Sphere() override {}

    SceneObject* Clone() const override {return new Sphere(*this);}

};


// Lights
struct SceneLight
{
    // Type of light to limit complexity
    enum class Type {Ambient, Directional, Point};

    SceneLight() = default;
    SceneLight(Type InitLightType, float InitIntensity, sf::Vector3f InitLocationOrDirection = sf::Vector3f(0.0f, 0.0f, 0.0f)) :
         LightType{InitLightType}, Intensity{InitIntensity}, LocationOrDirection{InitLocationOrDirection} {}
    SceneLight(const SceneLight& Other) :
        LightType{Other.LightType}, Intensity{Other.Intensity}, LocationOrDirection{Other.LocationOrDirection} {}
        

    Type LightType = Type::Ambient;

    // How much light the light will add to scene
    float Intensity = 0.0f;

    // This will be the location of a point light 
    // or the direction of a directional light
    sf::Vector3f LocationOrDirection {0.0f, 0.0f, 0.0f};

    // Color ???

    SceneLight* Clone() const {return new SceneLight(*this);}
    
};


// Intersection information to pass back 
struct Intersection 
{
    Intersection(SceneObject* InitObject, float InitT) :
        ClosestObject{InitObject}, ClosestT{InitT} {}

    SceneObject* ClosestObject = nullptr;
    float ClosestT = 0.0f;
};


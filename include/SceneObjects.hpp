
#pragma once

#include "Camera.hpp"

#include <SFML/Graphics.hpp>


// TODO: move these SceneObjects into their own files???
struct SceneObject
{
    virtual SceneObject* Clone() const = 0;

    sf::Vector3f ObjectColor;

    SceneObject() : ObjectColor{sf::Vector3f(255.0f, 0.0f, 0.0f)} {}
    SceneObject(sf::Vector3f InitColor) : ObjectColor{InitColor} {}
    virtual ~SceneObject() {}
};

struct Sphere : public SceneObject {
    sf::Vector3f Center {0.0f, 0.0f, 0.0f};
    float Radius {0.0f};

    Sphere() = default;
    Sphere(sf::Vector3f InitCenter, float InitRadius, sf::Vector3f InitColor) :
        SceneObject(InitColor), Center{InitCenter}, Radius{InitRadius} {}
    Sphere(const Sphere& Other) : 
            SceneObject(Other.ObjectColor), Center{Other.Center}, Radius{Other.Radius} {}

    ~Sphere() override {}

    SceneObject* Clone() const override {return new Sphere(*this);}

private:
    SceneObject& operator=(const SceneObject& OtherObject);
};


// Lights
struct SceneLight
{
    // Type of light to limit complexity
    enum class Type {Ambient, Directional, Point};

    SceneLight() = default;
    SceneLight(Type InitLightType, float InitIntensity, sf::Vector3f InitLocationOrDirection) :
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
    
private:
    SceneLight& operator=(const SceneLight& OtherLight);
};




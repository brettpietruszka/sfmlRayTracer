

#include "Ray.hpp"
#include "RayTracerMathLibrary.hpp"
#include "DebugLog.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

Ray::Ray() :
    Origin{new sf::Vector3f()}, 
    Direction{new sf::Vector3f()} {}

Ray::Ray(sf::Vector3f InitOrigin, sf::Vector3f InitDirection) :
    Origin{new sf::Vector3f(InitOrigin)}, 
    Direction{new sf::Vector3f(InitDirection)} {}

Ray::Ray(const Ray& OtherRay) : 
    Origin{new sf::Vector3f(*OtherRay.Origin)},
    Direction{new sf::Vector3f(*OtherRay.Direction)} {}

Ray::~Ray()
{
    delete Origin;
    delete Direction;
}

sf::Vector3f Ray::GetPointDistanceFromOrigin(const float DistanceFromOrigin) const
{
    return *Origin + (*Direction) * (DistanceFromOrigin / Length());
}

void Ray::Translate(sf::Vector3f TransVec)
{
    // Moves the origin by transVec amount
    *Origin = *Origin + TransVec;
}

void Ray::Rotate(sf::Vector3f RotateVec) 
{
    if (!Direction)
    {
        LOG_DEBUG("Why is there no direction on this ray while traying to rotate");
        return;
    }    
    
    RayTracerMathLibrary::RotatePointAboutOrigin(*Direction, RotateVec);
}

float Ray::Length() const
{

    if (!Direction)
    {
        LOG_DEBUG("Why is there no direction on this ray while traying to get length");
        return 0.0f;
    }   

    // Calculate the length of the direction vector
    return RayTracerMathLibrary::GetVectorLength(*Direction);
}

void Ray::SetLength(float NewLength) {

    if (!Direction)
    {
        LOG_DEBUG("Why is there no direction on this ray while traying to set length");
        return;
    }   

    // Change the Length of the direction vector to be new_length
    RayTracerMathLibrary::SetVectorLength(*Direction, NewLength);
}

sf::Vector3f Ray::GetOrigin() const {
    return *Origin;
}

sf::Vector3f Ray::GetDirection() const {
    return *Direction;
}
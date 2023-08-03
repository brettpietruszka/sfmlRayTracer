
#include <SFML/Graphics.hpp>
#include "Ray.hpp"
#include "RayTracerMathLibrary.hpp"
#include <cmath>
#include <iostream>

Ray::Ray() :
    Origin{new sf::Vector3f()}, 
    Direction{new sf::Vector3f()} {}

Ray::Ray(sf::Vector3f InitOrigin, sf::Vector3f InitDirection) :
    Origin{new sf::Vector3f(InitOrigin)}, 
    Direction{new sf::Vector3f(InitDirection)} {}


Ray::~Ray()
{
    delete Origin;
    delete Direction;
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
        std::cerr << "Why is there no direction on this ray while traying to rotate" << std::endl;
        return;
    }    
    
    RayTracerMathLibrary::RotatePointAboutOrigin(*Direction, RotateVec);

    // TODO: must I change this?
}

float Ray::Length() {
    // Calculate the length of the direction vector
    return std::sqrt(std::pow(Direction->x,2) + std::pow(Direction->y,2) + std::pow(Direction->z,2));
}

void Ray::SetLength(float NewLength) {
    // Change the Length of the direction vector to be new_length
    RayTracerMathLibrary::SetVectorLength(*Direction, NewLength);
}

sf::Vector3f Ray::GetOrigin() const {
    return *Origin;
}

sf::Vector3f Ray::GetDirection() const {
    return *Direction;
}
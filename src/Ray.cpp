
#include <SFML/Graphics.hpp>
#include "Ray.hpp"
#include <cmath>

Ray::Ray() :
    Origin{new sf::Vector3f()}, 
    Direction{new sf::Vector3f()} {}

Ray::Ray(sf::Vector3f InitOrigin, sf::Vector3f InitDirection) :
    Origin{new sf::Vector3f(InitOrigin)}, 
    Direction{new sf::Vector3f(InitDirection)} {}


void Ray::Translate(sf::Vector3f TransVec) {
    // Moves the origin by transVec amount
    *Origin = *Origin + TransVec;
}

void Ray::Rotate(sf::Vector3f RotateVec) {
    // TODO: implement this 
}

float Ray::Length() {
    // Calculate the length of the direction vector
    return std::sqrt(std::pow(Direction->x,2) + std::pow(Direction->y,2) + std::pow(Direction->z,2));
}

void Ray::SetLength(float NewLength) {
    // Change the Length of the direction vector to be new_length
    if (std::abs(NewLength) < 0.0001) {
        return;
    } 
    *Direction = (*Direction / Length()) * NewLength;
}

sf::Vector3f Ray::GetOrigin() const {
    return *Origin;
}

sf::Vector3f Ray::GetDirection() const {
    return *Direction;
}
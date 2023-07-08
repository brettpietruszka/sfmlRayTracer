
#include <SFML/Graphics.hpp>
#include "Ray.hpp"
#include <cmath>

Ray::Ray(sf::Vector2f origin, sf::Vector2f direction) :
    origin{origin}, direction{direction} {}

sf::Vector2f Ray::intersects(Ray other) {

    /*
        Returns Point of intersection of two rays
        Only goes in the forward direction. 
        If the t value is negative, it returns a vector with NANs
        If the lines are parallel, it returns a vector with NANs
    */


    float x1 = this->origin.x;
    float y1 = this->origin.y;
    float a1 = this->direction.x;
    float b1 = this->direction.y;

    float x2 = other.getOrigin().x;
    float y2 = other.getOrigin().y;
    float a2 = other.getDirection().x;
    float b2 = other.getDirection().y;

    // Calculate Direction Dot Product to check if parallel
    float denom = a1 * b2 - a2 * b1;
    // Check if lines are parallel
    if (std::abs(denom) < 0.0001) {
        return sf::Vector2f(NAN,NAN);
    }

    float t1 = ((x2 - x1) * b2 - (y2 - y1) * a2) / denom;
    if (t1 < 0) {
        return sf::Vector2f(NAN,NAN);
    }
    float x = x1 + t1 * a1;
    float y = y1 + t1 * b1;

    return sf::Vector2f(x,y);

}

float Ray::distToPoint(sf::Vector2f point) {
    // Used 
    sf::Vector2f intersect = sf::Vector2f(0,0);
    if (this->direction.x == 0 && this->direction.y == 0) {
        // just return distance between points
        float ax = point.x - this->origin.x;
        float ay = point.y - this->origin.y;
        return std::sqrt((ax * ax) + (ay * ay));
    }  else if (this->direction.x == 0) {
        Ray perpRay = Ray(point, sf::Vector2f(-this->direction.y,0));
        intersect = this->intersects(perpRay);
    } else if (this->direction.y == 0) {
        Ray perpRay = Ray(point, sf::Vector2f(0, -this->direction.x));
        intersect = this->intersects(perpRay);
    } else {
        Ray perpRay = Ray(point, sf::Vector2f(-this->direction.y, this->direction.x));
        intersect = this->intersects(perpRay);
    }

    float ax = intersect.x - this->origin.x;
    float ay = intersect.y - this->origin.y;

    return std::sqrt((ax * ax) + (ay * ay));
    
    

}


void Ray::translate(sf::Vector2f transVec) {
    // Moves the origin by transVec amount
    this->origin = this->origin + transVec;
}

void Ray::rotate(float angle) {
    // Rotates the direction by angle in ???degrees
    sf::Transform rotationMatrix;
    rotationMatrix.rotate(angle);
    this->direction = rotationMatrix * this->direction;
}

float Ray::length() {
    // Calculate the length of the direction vector
    return std::sqrt(std::pow(this->direction.x,2) + std::pow(this->direction.y,2));
}

void Ray::setLength(float new_length) {
    // Change the Length of the direction vector to be new_length
    if (std::abs(new_length) < 0.0001) {
        return;
    } 
    this->direction = (this->direction / this->length()) * new_length;
}


void Ray::draw(sf::RenderTarget* renderable, sf::Transform transformMatrix) {
    sf::VertexArray line(sf::Lines,2);
    line[0].position =  sf::Vector2f(0,0); // this->origin;
    line[1].position =  this->direction; //this->origin + this->direction;
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;
    renderable->draw(line, transformMatrix);
}

// Getters and Setters
void Ray::setOrigin(sf::Vector2f origin) {
    this->origin = origin;
}
void Ray::setDirection(sf::Vector2f direction) {
    this->direction = direction;
}

sf::Vector2f Ray::getOrigin() {
    return this->origin;
}

sf::Vector2f Ray::getDirection() {
    return this->direction;
}
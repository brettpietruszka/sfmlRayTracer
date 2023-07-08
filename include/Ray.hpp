#ifndef RAY_HPP
#define RAY_HPP

#include <SFML/Graphics.hpp>

class Ray {
    /*
        This class represents a linear parametric equation
        can calculate intersections it is currently unused
        TODO: modify to suit this application
    */

    private:
        sf::Vector2f origin;
        sf::Vector2f direction;

    public:
        Ray() = default;
        Ray(sf::Vector2f origin, sf::Vector2f direction);

        sf::Vector2f intersects(Ray);
        float distToPoint(sf::Vector2f);

        void setOrigin(sf::Vector2f origin);
        void setDirection(sf::Vector2f direction);

        void translate(sf::Vector2f);
        void rotate(float);

        float length();
        void setLength(float new_length);

        void draw(sf::RenderTarget*,sf::Transform);

        sf::Vector2f getOrigin();
        sf::Vector2f getDirection();
};

#endif
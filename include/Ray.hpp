#ifndef RAY_HPP
#define RAY_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Ray {
    /*
        This class represents a linear parametric equation
        can calculate intersections it is currently unused
        TODO: modify to suit this application
    */

    private:
        sf::Vector3f* Origin = nullptr;
        sf::Vector3f* Direction = nullptr;

    public:
        Ray();
        Ray(sf::Vector3f InitOrigin, sf::Vector3f InitDirection);
        Ray(const Ray& OtherRay);
        ~Ray();

        sf::Vector3f GetPointDistanceFromOrigin(const float DistanceFromOrigin) const;

        void Translate(sf::Vector3f TransVec);
        void Rotate(sf::Vector3f RotateVec);

        float Length() const;
        void SetLength(float NewLength);

        sf::Vector3f GetOrigin() const;
        sf::Vector3f GetDirection() const;

};

#endif
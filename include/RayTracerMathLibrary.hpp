

#pragma once

#include "RayTracer.hpp"

#include <SFML/Graphics.hpp>


class Ray;

class RayTracerMathLibrary {
    /* Static Class helper functions for calculating intersections and 
        other valuable math techniques. TODO: Possibly just make a namespace library  */

public:

    // TODO: Implement?
    // template<typename T>
    // static sf::Vector3f IntersectRaySceneObject(const Ray* Ray, const T* SceneObject);

    /* Finds the intersection times for a point in 3d space of a ray and a sphere */
    static sf::Vector2f IntersectRaySphere(const Ray& Ray, const RayTracer::Sphere* Sphere);

    static sf::Vector3f RotatePointAboutOrigin(sf::Vector3f& InVec, const sf::Vector3f& RotateVec);

    static void SetVectorLength(sf::Vector3f& Vec, float NewLength);

    // inline helper functions
    inline static float GetVectorLength(const sf::Vector3f& Vec);

    inline static float DotProduct(const sf::Vector3f& Vec1, const sf::Vector3f& Vec2);

    inline static sf::Vector3f CrossProduct(const sf::Vector3f Vec1, const sf::Vector3f Vec2);

    inline static bool BasicallyZero(float x) {return std::abs(x) < 0.000001;} 

    inline static float DegreesToRadians(float Degs) {return Degs * M_PI / 180.0f;}

    inline static float RadiansToDegrees(float Rads) {return Rads * 180.0f / M_PI;}
};
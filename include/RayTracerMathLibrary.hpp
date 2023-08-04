

#pragma once

#include "RayTracer.hpp"

#include <SFML/Graphics.hpp>


class Ray;

class RayTracerMathLibrary 
{
    /* Static Class helper functions for calculating intersections and 
        other valuable math techniques. TODO: Possibly just make a namespace library  */

public:

    // TODO: Implement?
    // template<typename T>
    // static sf::Vector3f IntersectRaySceneObject(const Ray* Ray, const T* SceneObject);

    /* Finds the intersection times for a point in 3d space of a ray and a sphere */
    static sf::Vector2f IntersectRaySphere(const Ray& Ray, const RayTracer::Sphere* Sphere);

    static void RotatePointAboutOrigin(sf::Vector3f& InVec, const sf::Vector3f& RotateVec);

    static void SetVectorLength(sf::Vector3f& Vec, float NewLength);

    // inline helper functions
    inline static float GetVectorLength(const sf::Vector3f& Vec) {return std::sqrt(std::pow(Vec.x,2) + std::pow(Vec.y,2) + std::pow(Vec.z,2));}

    inline static float DotProduct(const sf::Vector3f& Vec1, const sf::Vector3f& Vec2) { return Vec1.x * Vec2.x + Vec1.y * Vec2.y + Vec1.z * Vec2.z;}

    inline static sf::Vector3f CrossProduct(const sf::Vector3f& A, const sf::Vector3f& B) {    return sf::Vector3f(A.z * B.y - A.y * B.z,   A.x * B.z - A.z * B.x,   A.x * B.y - A.y * B.x);}

    inline static bool BasicallyZero(float x) {return std::abs(x) < 0.000001;} 

    inline static float DegreesToRadians(float Degs) {return Degs * M_PI / 180.0f;}

    inline static float RadiansToDegrees(float Rads) {return Rads * 180.0f / M_PI;}
};
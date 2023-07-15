

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

    static float DotProduct(const sf::Vector3f& Vec1, const sf::Vector3f& Vec2);
};
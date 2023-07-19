
#include "RayTracerMathLibrary.hpp"
#include "Ray.hpp"
#include "RayTracer.hpp"

#include <SFML/Graphics.hpp>

#include <limits>
#include <cmath>


sf::Vector2f RayTracerMathLibrary::IntersectRaySphere(const Ray& Ray, const RayTracer::Sphere* Sphere)
{
    const float r = Sphere->Radius;
    const sf::Vector3f CO = Ray.GetOrigin() - Sphere->Center;
    const sf::Vector3f D = Ray.GetDirection();

    // Perform quadratic formula of ray sphere intersection 
    const float a = DotProduct(D, D);
    const float b = 2.0f * DotProduct(CO, D);
    const float c = DotProduct(CO, CO) - r * r;

    const float Discriminant = b * b  - 4 * a * c;

    if (Discriminant < 0)
    {
        return sf::Vector2f(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
    }

    const float t1 = (-b + std::sqrt(Discriminant)) / (2.0f * a);
    const float t2 = (-b - std::sqrt(Discriminant)) / (2.0f * a);

    return sf::Vector2f(t1,t2);
}


float RayTracerMathLibrary::DotProduct(const sf::Vector3f& Vec1, const sf::Vector3f& Vec2) {
    return Vec1.x * Vec2.x + Vec1.y * Vec2.y + Vec1.z * Vec2.z;
}

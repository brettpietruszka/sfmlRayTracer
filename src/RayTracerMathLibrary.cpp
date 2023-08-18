
#include "RayTracerMathLibrary.hpp"
#include "Ray.hpp"
#include "RayTracer.hpp"

#include <SFML/Graphics.hpp>

#include <limits>
#include <cmath>


sf::Vector2f RayTracerMathLibrary::IntersectRaySphere(const Ray& Ray, const Sphere* Sphere)
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

void RayTracerMathLibrary::RotatePointAboutOrigin(sf::Vector3f& InVec, const sf::Vector3f& RotateVec)
{
    // z y gives pitch pos pitch starts going upwards
    // x z gives yaw pos yaws cc
    // x y gives roll pos rolls cc
    const float p = DegreesToRadians(RotateVec.x); // pitch 
    const float y = DegreesToRadians(RotateVec.y); // yaw
    const float r = DegreesToRadians(RotateVec.z); // roll

    // Math calculated using linear algebra as defined with Rz = roll, Rx = Pitch, Ry = Yaw
    const float NewDirectionX = InVec.x * std::cos(y) * std::cos(r) 
                            - InVec.y * std::cos(y) * std::sin(r) 
                            - InVec.z * std::sin(y);
    const float NewDirectionY = InVec.x * (std::cos(p) * std::sin(r) - std::sin(p) * std::sin(y) * std::cos(r))
                            + InVec.y * (std::sin(p) * std::sin(y) * std::sin(r) + std::cos(p) * std::cos(r))
                            - InVec.z * std::sin(p) * std::cos(y);
    const float NewDirectionZ = InVec.x * (std::cos(p) * std::sin(y) * std::cos(r) + std::sin(p) * std::sin(r))
                            + InVec.y * (std::sin(p) * std::cos(r) * std::sin(r) - std::cos(p) * std::sin(y) * std::sin(r))
                            + InVec.z * std::cos(p) * std::cos(y);

    // Modify the direction
    InVec.x = NewDirectionX;
    InVec.y = NewDirectionY;
    InVec.z = NewDirectionZ;
}

void RayTracerMathLibrary::SetVectorLength(sf::Vector3f& Vec, float NewLength)
{
    if (RayTracerMathLibrary::BasicallyZero(NewLength)) {
        // TODO: log
        return;
    } 

    const float VectorLength = GetVectorLength(Vec);
    if (RayTracerMathLibrary::BasicallyZero(VectorLength))
    {
        // TODO: log
        return;
    }

    Vec = (Vec / VectorLength) * NewLength;
}


#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "globals.hpp"
#include "SceneObjects.hpp"
#include "RayTracerMathLibrary.hpp"
#include "DebugLog.hpp"

#include <SFML/Graphics.hpp>

#include <limits>
#include <iostream>
#include <thread>

RayTracer::RayTracer() : 
    RayTracerCamera{new Camera()}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()}
     {}

RayTracer::RayTracer(const Camera& RTCamera) : 
    RayTracerCamera{new Camera(RTCamera)}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()}
     {}


RayTracer::RayTracer(const Camera& RTCamera, float InitScreenWidth, float InitScreenHeight) : 
    RayTracerCamera{new Camera(RTCamera)}, 
    ScreenTexture{new sf::Texture()},
    RaytracedImage{new sf::Image()},
    ScreenWidth{InitScreenWidth},
    ScreenHeight{InitScreenHeight}
     {}


RayTracer::~RayTracer() 
{
    // Free up all memory used by the raytracer
    delete RayTracerCamera;
    delete ScreenTexture;
    delete RaytracedImage;

    for (SceneObject* Shape : SceneObjects) 
    {
        delete Shape;
    }

    for (SceneLight* Light : SceneLights) 
    {
        delete Light;
    }
}


bool RayTracer::AddObject(int ObjectIndex, const SceneObject& ObjToAdd)
{
    // Create a copy of the object passed in
    SceneObject* NewObj = ObjToAdd.Clone();

    // Add the reference to the SceneObjects
    if (NewObj)
    {
        SceneObjects.emplace_back(NewObj);
        return true;
    }

    return false;
}

bool RayTracer::RemoveObject(int ObjIdx)
{   
    // Find and remove the object (not sorted)
    if (ObjIdx >= 0 && ObjIdx < SceneObjects.size())
    {
        // Valid Idx attempt to remove
        if (SceneObject* ObjToRemove = SceneObjects.at(ObjIdx))
        {
            SceneObjects.erase(SceneObjects.begin() + ObjIdx);
            delete ObjToRemove;
            return true;
        }
    }

    return false;
}

bool RayTracer::AddLight(int ObjectIndex, const SceneLight& LightToAdd)
{
    // Create a copy of the object passed in
    SceneLight* NewLight = LightToAdd.Clone();

    // Add the reference to the SceneObjects
    if (NewLight)
    {
        SceneLights.emplace_back(NewLight);
        return true;
    }

    return false;
}


bool RayTracer::RemoveLight(int LightIdx)
{
    // Find and remove the light (not sorted)
    if (LightIdx >= 0 && LightIdx < SceneLights.size())
    {
        // Valid Idx attempt to remove
        if (SceneLight* LightToRemove = SceneLights.at(LightIdx))
        {
            SceneLights.erase(SceneLights.begin() + LightIdx);
            delete LightToRemove;
            return true;
        }
    }

    return false;
}
 

bool RayTracer::RefreshImage(sf::Texture& OutTexture)
{
    // This example shows another common use of sf::Texture:
    // streaming real-time data, like video frames
    
    // Create an empty texture Todo: Screen Size
    if (!ScreenTexture->create(ScreenWidth, ScreenHeight))
    {
        return false; 
    }
    
    // Create an Image and raytrace the scene
    RaytracedImage->create(ScreenWidth, ScreenHeight, sf::Color::White);

    // Divide the screen into 4 or more seperate portions.
    std::thread UpperLeft(&RayTracer::RayTraceScreenPortion, this, -ScreenWidth/2, 0, -ScreenHeight/2, 0);
    std::thread LowerLeft(&RayTracer::RayTraceScreenPortion, this, -ScreenWidth/2, 0, 0, ScreenHeight/2 - 1);
    std::thread UpperRight(&RayTracer::RayTraceScreenPortion, this, 0, ScreenWidth/2 - 1, -ScreenHeight/2, 0);
    std::thread LowerRight(&RayTracer::RayTraceScreenPortion, this, 0, ScreenWidth/2 - 1, 0, ScreenHeight/2 - 1);

    UpperLeft.join();
    LowerLeft.join();
    UpperRight.join();
    LowerRight.join();

    // Set the OutTexture to be the resulting image
    ScreenTexture->loadFromImage(*RaytracedImage, sf::IntRect(0, 0, ScreenWidth, ScreenHeight));
    OutTexture = sf::Texture(*ScreenTexture);

    return true;
}


sf::Vector3f RayTracer::CanvasToViewport(int x, int y)
{
    const Camera::ViewPort ViewPort = RayTracerCamera->GetCameraViewPort();

    const float VpW = ViewPort.ViewportWidth;
    const float VpH = ViewPort.ViewportHeight;
    const float d = ViewPort.ViewportD;

    /* TODO: figure out if the distortion cause by rotation is accurate or not
        Calculate the 3d location of this point in the world
        if we assume roll is level with the x-z plane becauase it is, we can use cross product to find 
        first vector perpendicular to orientation and (0,1,0) (straight up)
        then we cross product to get a normal to both of those. */

    sf::Vector3f Direction = RayTracerCamera->GetCameraOrientation();
    const sf::Vector3f HeightPerp {0.0f, 1.0f, 0.0f};
    sf::Vector3f WidthPerp = RayTracerMathLibrary::CrossProduct(Direction, HeightPerp);
    RayTracerMathLibrary::SetVectorLength(WidthPerp, 1.0);
    RayTracerMathLibrary::SetVectorLength(Direction, d);

    sf::Vector3f OutLocation = RayTracerCamera->GetCameraLocation() + Direction;
    
    OutLocation = OutLocation + WidthPerp * (float)x * VpW/ScreenWidth + HeightPerp * (float)y * VpH/ScreenHeight;

    return OutLocation;
}


sf::Color RayTracer::TraceRay(const Ray& CurrentRay, float TMin, float TMax, int RecursiveDepth)
{
    
    // Get the closest intersection and use it to compute the color
    Intersection Collision = ClosestIntersection(CurrentRay, TMin, TMax);
    SceneObject* ClosestSceneObject = Collision.ClosestObject;
    const float ClosestT = Collision.ClosestT;
    
    if (!ClosestSceneObject)
    {
        // Return the background color (white)
        return sf::Color::Black;
    }

    // TODO: make this work for other scene objects
    // For now we will just assume it is a sphere
    const Sphere* AsSphere = static_cast<Sphere*>(ClosestSceneObject);
    const sf::Vector3f SpherePoint = CurrentRay.GetPointDistanceFromOrigin(ClosestT);
    sf::Vector3f SphereNormal = SpherePoint - AsSphere->Center;
    SphereNormal /= RayTracerMathLibrary::GetVectorLength(SphereNormal);
    
    const sf::Vector3f ResultColor = ClosestSceneObject->ObjectColor * ComputeLighting(SpherePoint, SphereNormal, -CurrentRay.GetDirection(), ClosestSceneObject->Shininess);

    // Do reflections if the object is reflective and we haven't ran out of recursions
    const float Refl = ClosestSceneObject->Reflectiveness;
    if (Refl <= 0.0f || RecursiveDepth <= 0)
    {
        return sf::Color(ResultColor.x, ResultColor.y, ResultColor.z);
    }

    sf::Vector3f ReflectionRay = RayTracerMathLibrary::ReflectRay(-CurrentRay.GetDirection(), SphereNormal);
    sf::Color ReflectedColor = TraceRay(Ray(SpherePoint, ReflectionRay), 0.01, std::numeric_limits<float>::infinity(), RecursiveDepth - 1);
    sf::Vector3f FinalColor = ResultColor * (1 - Refl) + sf::Vector3f(ReflectedColor.r * Refl, ReflectedColor.g * Refl, ReflectedColor.b * Refl);

    return sf::Color(FinalColor.x, FinalColor.y, FinalColor.z);
}

 Intersection RayTracer::ClosestIntersection(const Ray& CurrentRay, float TMin, float TMax)
 {

    float ClosestT = std::numeric_limits<float>::infinity();
    SceneObject* ClosestSceneObject = nullptr;

    // TODO: Change to be SceneObjects
    // Loop throught the scene spheres
    for (const auto& CurObject : SceneObjects)
    {

        // For now, cast to sphere
        const Sphere* AsSphere = static_cast<Sphere*>(CurObject);

        // Returns the result of the quadratic formula from the ray sphere intersection
        sf::Vector2f TValues = RayTracerMathLibrary::IntersectRaySphere(CurrentRay, AsSphere);

        const float t1 = TValues.x;
        const float t2 = TValues.y;
        
        // Determine if this object is the closest and set values
        if (TMin <= t1 && t1 <= TMax && t1 < ClosestT )
        {
            ClosestT = t1;
            ClosestSceneObject = CurObject;
        }

        if (TMin <= t2 && t2 <= TMax && t2 < ClosestT)
        {
            ClosestT = t2;
            ClosestSceneObject = CurObject;
        }
    }

    return Intersection{ClosestSceneObject, ClosestT};
 }



float RayTracer::ComputeLighting(const sf::Vector3f& Point, const sf::Vector3f& Normal, const sf::Vector3f& V, const int& Shininess)
{
    /* Determing the intensity of the light at a particular location in the scene 
        by looping through the scene lights and adding their contributions */
    float Intensity = 0.0f;
    sf::Vector3f L;
    for (const auto& Light : SceneLights)
    {
        if (Light->LightType == SceneLight::Type::Ambient)
        {
            Intensity += Light->Intensity;
        }
        else 
        {
            // Determine the Light Ray/Direction
            float TMax = 0.0001f;
            if (Light->LightType == SceneLight::Type::Point)
            {
                L = Light->LocationOrDirection - Point;
            }
            else 
            {
                L = Light->LocationOrDirection;
                TMax = std::numeric_limits<float>::infinity();
            }

            // Check for Shadows, if shadow no diffuse or specular light
            Intersection Collision = ClosestIntersection(Ray(Point, L), 0.001, TMax);

            if (Collision.ClosestObject != nullptr)
            {
                continue;
            }

            // Diffuse Lighting
            const float NDotL = RayTracerMathLibrary::DotProduct(Normal, L);
            if (NDotL > 0.0f)
            {
                Intensity += Light->Intensity * NDotL 
                    / (RayTracerMathLibrary::GetVectorLength(Normal) * RayTracerMathLibrary::GetVectorLength(L));
            }

            // Specular lighting
            if (Shininess != -1)
            {
                sf::Vector3f R = RayTracerMathLibrary::ReflectRay(L, Normal);
                const float RDotV = RayTracerMathLibrary::DotProduct(R, V);
                if (RDotV > 0.0f) 
                {
                    Intensity += Light->Intensity * std::pow(RDotV / (RayTracerMathLibrary::GetVectorLength(R) * RayTracerMathLibrary::GetVectorLength(V)), Shininess);
                }
            }
        }

    }

    return Intensity > 1.0f ? 1.0f : Intensity;
}

bool RayTracer::HandleInput()
{

    if (!RayTracerCamera)
    {
        LOG_DEBUG("Handle input, RayTracerCamera null");
        return false;
    }


    bool bCameraChanged = false; 
    sf::Vector3f TranslateVec {};
    sf::Vector3f RotateVec {};

    constexpr float DeltaTranslate = 0.75f;
    constexpr float DeltaRotate = 5.0f;

    //  Move Camera 
    // z
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        LOG_DEBUG("w pressed");
        TranslateVec.z += DeltaTranslate; 
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        LOG_DEBUG("s pressed");
        TranslateVec.z -= DeltaTranslate;
        bCameraChanged = true;

    }

    // x
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        LOG_DEBUG("d pressed");
        TranslateVec.x += DeltaTranslate;
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        LOG_DEBUG("a pressed");
        TranslateVec.x -= DeltaTranslate;
        bCameraChanged = true;
    }

    // y
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
    {
        LOG_DEBUG("e pressed");
        TranslateVec.y += DeltaTranslate;
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        LOG_DEBUG("q pressed");
        TranslateVec.y -= DeltaTranslate;
        bCameraChanged = true;
    }



    // Rotate Camera
    // Pitch
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        LOG_DEBUG("up pressed");
        RotateVec.x -= DeltaRotate;
        bCameraChanged = true;

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        LOG_DEBUG("down pressed");
        RotateVec.x += DeltaRotate; 
        bCameraChanged = true;
    }

    // Yaw
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        LOG_DEBUG("left pressed");
        RotateVec.y += DeltaRotate; 
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        LOG_DEBUG("right pressed");
        RotateVec.y -= DeltaRotate; 
        bCameraChanged = true;
    }

    // Apply Camera Transformations
    RayTracerCamera->Rotate(RotateVec);
    RayTracerCamera->Translate(TranslateVec);

    return bCameraChanged;
}


void RayTracer::RayTraceScreenPortion(int xmin, int xmax, int ymin, int ymax)
{

    /* Loop a particular portion of the scene and set the color of the image 
        based of the result of the ray trace*/
    for (int x = xmin; x <= xmax; x++) 
    {
        for (int y = ymin; y <= ymax; y++)
        {        
            // Calculate the color of the pixel at this location
            // All of this is read only for shared data. no need for mutex locks
            sf::Vector3f Direction = CanvasToViewport(x,y);
            Ray CurrentRay = Ray(RayTracerCamera->GetCameraLocation(), Direction);
            CurrentRay.SetLength(1.0);
            sf::Color PixelColor = TraceRay(CurrentRay, 1.0f, std::numeric_limits<float>::infinity(), 3);
            
            // Set the color of the pixel
            // since we are only writing and never read from this
            // and to different portions of the screen
            // a mutex is technically not necessary
            RaytracedImage->setPixel(x + ScreenWidth/2, ScreenHeight - (y + ScreenHeight/2) - 1, PixelColor);
        }
    }
}


#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "globals.hpp"
#include "RayTracerMathLibrary.hpp"

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

    for (SceneObject* shape : SceneObjects) 
    {
        delete shape;
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
    if (ObjIdx >= 0 && ObjIdx < SceneObjects.size())
    {
        // Valid Idx attemp to remove
        if (SceneObject* ObjToRemove = SceneObjects.at(ObjIdx))
        {
            SceneObjects.erase(SceneObjects.begin() + ObjIdx);
            delete ObjToRemove;
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


    // Thread here from -ScreenWidth/2 to 0, 0 to ScreenWidth/2
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

    // TODO: calculate the 3d location of this point in the world
    // if we assume roll is level with the x-z plane becauase it is, we can use cross product to find 
    // first vector perpendicular to orientation and (0,1,0) (straight up)
    // then we cross product to get a normal to both of those

    // NEED TO TEST
    sf::Vector3f Direction = RayTracerCamera->GetCameraOrientation();
    const sf::Vector3f HeightPerp {0.0f, 1.0f, 0.0f};
    sf::Vector3f WidthPerp = RayTracerMathLibrary::CrossProduct(Direction, HeightPerp);
    RayTracerMathLibrary::SetVectorLength(WidthPerp, 1.0);
    RayTracerMathLibrary::SetVectorLength(Direction, d);

    sf::Vector3f OutLocation = RayTracerCamera->GetCameraLocation() + Direction;
    
    OutLocation = OutLocation + WidthPerp * (float)x * VpW/ScreenWidth + HeightPerp * (float)y * VpH/ScreenHeight;

    return OutLocation;
}


sf::Color RayTracer::TraceRay(const Ray& CurrentRay, float TMin, float TMax)
{
    float ClosestT = std::numeric_limits<float>::infinity();
    SceneObject* ClosestSceneObject = nullptr;

    // TODO: Change to be SceneObjects
    // Loop throught the scene spheres
    for (const auto CurObject : SceneObjects)
    {

        // For now, cast to sphere
        const Sphere* AsSphere = static_cast<Sphere*>(CurObject);

        // Returns the result of the quadratic formula from the ray sphere intersection
        sf::Vector2f TValues = RayTracerMathLibrary::IntersectRaySphere(CurrentRay, AsSphere);

        const float t1 = TValues.x;
        const float t2 = TValues.y;
        // TODO
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

    if (!ClosestSceneObject)
    {
        return sf::Color::White;
    }

    return ClosestSceneObject->ObjectColor;

}

bool RayTracer::HandleInput()
{

    if (!RayTracerCamera)
    {
        std::cerr << "Handle input, RayTracerCamera null" << std::endl;
    }


    bool bCameraChanged = false; 
    sf::Vector3f TranslateVec {};
    sf::Vector3f RotateVec {};

    constexpr float DeltaTranslate = 0.05f;
    constexpr float DeltaRotate = 1.0f;

    //  Move Camera 
    // z
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        std::cerr << "w pressed" << std::endl;
        TranslateVec.z += DeltaTranslate; 
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        std::cerr << "s pressed" << std::endl;
        TranslateVec.z -= DeltaTranslate;
        bCameraChanged = true;

    }

    // x
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        std::cerr << "d pressed" << std::endl;
        TranslateVec.x += DeltaTranslate;
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        std::cerr << "a pressed" << std::endl;
        TranslateVec.x -= DeltaTranslate;
        bCameraChanged = true;
    }

    // y
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
    {
        std::cerr << "e pressed" << std::endl;
        TranslateVec.y += DeltaTranslate;
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        std::cerr << "q pressed" << std::endl;
        TranslateVec.y -= DeltaTranslate;
        bCameraChanged = true;
    }



    // Rotate Camera
    // Pitch
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        std::cerr << "up pressed" << std::endl;
        RotateVec.x -= DeltaRotate;
        bCameraChanged = true;

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        std::cerr << "down pressed" << std::endl;
        RotateVec.x += DeltaRotate; 
        bCameraChanged = true;
    }

    // Yaw
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        std::cerr << "left pressed" << std::endl;
        RotateVec.y += DeltaRotate; 
        bCameraChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        std::cerr << "right pressed" << std::endl;
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

    // TODO: mutex locks
    for (int x = xmin; x <= xmax; x++) 
    {
        for (int y = ymin; y <= ymax; y++)
        {            
            // Calculate the color of the pixel at this location
            // All of this is read only for shared data. no need for mutex locks
            sf::Vector3f Direction = CanvasToViewport(x,y);
            Ray CurrentRay = Ray(RayTracerCamera->GetCameraLocation(), Direction);
            CurrentRay.SetLength(1.0);
            sf::Color PixelColor = TraceRay(CurrentRay, 1, std::numeric_limits<float>::infinity());
            
            // Set the color of the pixel
            // since we are only writing and never read from this
            // and to different portions of the screen
            // a mutex is technically not necessary
            RaytracedImage->setPixel(x + ScreenWidth/2, ScreenHeight - (y + ScreenHeight/2) - 1, PixelColor);
        }
    }
}

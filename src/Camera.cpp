#include "Camera.hpp"
#include "Ray.hpp"


Camera::Camera() :
    CameraRay{new Ray(sf::Vector3f(), sf::Vector3f(0.0f, 0.0f, 1.0f))},
    CameraViewPort{new ViewPort{0.0f, 0.0f, 1.0f}} {}

Camera::Camera(const ViewPort& VP) :
    CameraRay{new Ray(sf::Vector3f(), sf::Vector3f(0.0, 0.0f, VP.ViewportD))},
    CameraViewPort{new ViewPort{VP.ViewportWidth, VP.ViewportHeight, VP.ViewportD}} {}

Camera::Camera(const Camera& CameraToCopy) :
    CameraRay{new Ray(CameraToCopy.GetCameraRay())},
    CameraViewPort{new ViewPort(CameraToCopy.GetCameraViewPort())} {}


Camera::~Camera()
{
    delete CameraRay;
    delete CameraViewPort;
}


void Camera::Translate(sf::Vector3f TranslateVector) 
{
    CameraRay->Translate(TranslateVector);
}

void Camera::Rotate(sf::Vector3f RotateVector) 
{
    // Rotate the camera ray and viewport
    CameraRay->Rotate(RotateVector);
}


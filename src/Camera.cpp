#include "Camera.hpp"


Camera::Camera() :
    CameraLocation{new sf::Vector3f()}, 
    CameraOrientation{new sf::Vector3f()},
    CameraViewPort{new ViewPort{0.0f, 0.0f, 0.0f}} {}

Camera::Camera(const ViewPort& VP) :
    CameraLocation{new sf::Vector3f()}, 
    CameraOrientation{new sf::Vector3f()},
    CameraViewPort{new ViewPort{VP.ViewportWidth, VP.ViewportHeight, VP.ViewportD}} {}

Camera::Camera(const Camera& CameraToCopy) :
    CameraLocation{new sf::Vector3f(CameraToCopy.getCameraLocation())}, 
    CameraOrientation{new sf::Vector3f(CameraToCopy.getCameraOrientation())},
    CameraViewPort{new ViewPort(CameraToCopy.getCameraViewPort())} {}


Camera::~Camera()
{
    delete CameraLocation;
    delete CameraOrientation;
    delete CameraViewPort;
}


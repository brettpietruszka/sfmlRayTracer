
#pragma once

#include "Ray.hpp"

#include <SFML/Graphics.hpp>
#include <map>

class Camera {

    /* This is kind of a data class right now. Eventually will provide 
        functionality for moving the camera and utilizing the ray */

public:
    struct ViewPort {
        float ViewportWidth {1.0f};
        float ViewportHeight {1.0f};
        float ViewportD {1.0f};

        ViewPort():
            ViewportWidth{1.0f}, ViewportHeight{1.0f}, ViewportD{1.0f}  {}

        ViewPort(float Width, float Height, float D) :
            ViewportWidth{Width}, ViewportHeight{Height}, ViewportD{D} {}

        ViewPort(const ViewPort& Other) :
            ViewportWidth{Other.ViewportWidth}, 
            ViewportHeight{Other.ViewportHeight}, 
            ViewportD{Other.ViewportD} {}

    private:
        ViewPort& operator=(const ViewPort&);
    };

private:
    Ray* CameraRay = nullptr;

    ViewPort* CameraViewPort = nullptr;

public:
    /* Constructors: */
    Camera();
    Camera(const ViewPort& CameraViewport);
    Camera(const Camera& CameraToCopy);
    ~Camera();

    // sCamera Input functions
    void Translate(sf::Vector3f TranslateVector);
    void Rotate(sf::Vector3f RotateVector);

    // Getters
    sf::Vector3f GetCameraLocation() const {return CameraRay->GetOrigin();}
    sf::Vector3f GetCameraOrientation() const {return CameraRay->GetDirection();}
    Ray GetCameraRay() const {return *CameraRay;}
    ViewPort GetCameraViewPort() const {return ViewPort(*CameraViewPort);}

private:
    Camera& operator=(const Camera&);
};
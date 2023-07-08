
#pragma once

#include <SFML/Graphics.hpp>
#include <map>

class Camera {

    /* This is kind of a data class right now. Eventually will provide 
        functionality for moving the camera and utilizing the ray */

public:
    struct ViewPort {
        float ViewportWidth {0.0f};
        float ViewportHeight {0.0f};
        float ViewportD {1.0f};

        ViewPort() {}

        ViewPort(float Width, float Height, float D) :
            ViewportWidth{Width}, ViewportHeight{Height}, ViewportD{D} {}

        ViewPort(const ViewPort& Other) :
            ViewportWidth{Other.ViewportWidth}, 
            ViewportHeight{Other.ViewportHeight}, 
            ViewportD{Other.ViewportD} {}
    };

private:
    sf::Vector3f* CameraLocation;
    sf::Vector3f* CameraOrientation;

    ViewPort* CameraViewPort;

    // Todo: keep track of wasd,e,q and mouse for manuevering scene
    //std::map<float, int> InputBindings;

public:
    /* Constructors: */
    Camera();
    Camera(const ViewPort& CameraViewport);
    Camera(const Camera& CameraToCopy);
    ~Camera();

    // TODO Camera 
    //void HandleInput(const char InputButton);

    // Getters
    sf::Vector3f getCameraLocation() const {return sf::Vector3f(*CameraLocation);}
    sf::Vector3f getCameraOrientation() const {return sf::Vector3f(*CameraOrientation);}
    ViewPort getCameraViewPort() const {return ViewPort(*CameraViewPort);}
};
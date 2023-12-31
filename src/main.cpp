
#include "main.hpp"


int main() {

    // SFML data
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML RAYTRACER!!!");
    sf::Vector2i mouse_pos;
    sf::String UserInput;
    sf::Clock GameClock;

    // RayTracer data
    sf::Sprite DisplaySprite = sf::Sprite();

    Camera::ViewPort MyViewPort{1.6f, 1.0f, 1.0f}; // Should have default values
    Camera MyCamera {MyViewPort}; 
    RayTracer MyRaytracer {MyCamera, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Add our shapes
    Sphere RedSphere = Sphere(sf::Vector3f(0.0f, -1.0f, 3.0f), 1, sf::Vector3f(255,0,0), 500, 0.2);
    MyRaytracer.AddObject(0, RedSphere);

    Sphere BlueSphere = Sphere(sf::Vector3f(2.0f, 0.0f, 4.0f), 1, sf::Vector3f(0,0,255), 500, 0.3);
    MyRaytracer.AddObject(0, BlueSphere);

    Sphere GreenSphere = Sphere(sf::Vector3f(-2.0f, 0.0f, 4.0f), 1, sf::Vector3f(0,255,0), 10, 0.4);
    MyRaytracer.AddObject(0, GreenSphere);

    Sphere YellowSphere = Sphere(sf::Vector3f(0.0f, -5001.0f, 0.0f), 5000, sf::Vector3f(255,255,0), 1000, 0.5);
    MyRaytracer.AddObject(0, YellowSphere);

    // Add 3 Lights
    SceneLight AmbientLight = SceneLight(SceneLight::Type::Ambient, 0.2);
    MyRaytracer.AddLight(0, AmbientLight);

    SceneLight PointLight = SceneLight(SceneLight::Type::Point, 0.6, sf::Vector3f(2.0f, 1.0f, 0.0f));
    MyRaytracer.AddLight(0, PointLight);

    SceneLight DirectionalLight = SceneLight(SceneLight::Type::Directional, 0.2, sf::Vector3f(1.0f, 4.0f, 4.0f));
    MyRaytracer.AddLight(0, DirectionalLight);

    // Start the "game" loop
    sf::Texture RenderedScene = sf::Texture();
    float PrevTime = GameClock.getElapsedTime().asMilliseconds();
    int PrevSeconds = 0;
    int FPSCounter = 0;

    // Raytrace the scene
    if (MyRaytracer.RefreshImage(RenderedScene)) 
    {
        DisplaySprite.setTexture(RenderedScene);
        window.draw(DisplaySprite);
    }

    // Keep the window open
    while (window.isOpen()) 
    {

        // Wipe the Screen
        window.clear(sf::Color(0, 255, 0, 255));

        // increment fps counter. this breaks if fps is less than 1
        const int NewGameSeconds = (int)GameClock.getElapsedTime().asSeconds();
        if (NewGameSeconds > PrevSeconds)
        {
            // refresh fps counter
            LOG_DEBUG("FPS = %d", FPSCounter);
            FPSCounter = 0;
            PrevSeconds = NewGameSeconds;
        }
        else 
        {
            FPSCounter++;
        }

        // Check For Generic Window Events 
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            switch (event.type) 
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    // todo: resize the display sprite to be of the new screen size
                    // let raytracer know the screen size changed
                default:
                    break;
            }
        }


        // Gets the current click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            char MouseOutput[30] = "mouse output";
            std::sprintf(MouseOutput, "%d, %d", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            LOG_DEBUG_CHARARRAY(MouseOutput);
        } 
       
        // Handle Input
        if (MyRaytracer.HandleInput())
        {
            // Raytrace the scene
            if (MyRaytracer.RefreshImage(RenderedScene)) 
            {
                // Draw the new scene
                DisplaySprite.setTexture(RenderedScene);
                
            }
        }

        window.draw(DisplaySprite);
        window.display();
        
    }

    return 0;
}
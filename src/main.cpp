#include <iostream>

#include "main.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

int main() {

    sf::CircleShape shape(1.0f);
    shape.setFillColor(sf::Color::Black);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML RAYTRACER!!!");
    sf::Vector2i mouse_pos;
    sf::String userInput;
    sf::Clock sim_clock;

    sf::Sprite DisplaySprite = sf::Sprite();

    Camera::ViewPort MyViewPort; // Should have default values
    Camera MyCamera {MyViewPort}; 
    RayTracer MyRaytracer {MyCamera, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Add our 3 shapes
    RayTracer::Sphere RedSphere = RayTracer::Sphere(sf::Vector3f(0.0f, -1.0f, 3.0f), 1, sf::Color(255,0,0));
    MyRaytracer.AddObject(0, RedSphere);

    RayTracer::Sphere BlueSphere = RayTracer::Sphere(sf::Vector3f(2.0f, 0.0f, -4.0f), 1, sf::Color(0,0,255));
    MyRaytracer.AddObject(0, BlueSphere);

    RayTracer::Sphere GreenSphere = RayTracer::Sphere(sf::Vector3f(-2.0f, 0.0f, 4.0f), 1, sf::Color(0,255,0));
    MyRaytracer.AddObject(0, GreenSphere);

    sf::Texture RenderedScene = sf::Texture();

    float prev_time = sim_clock.getElapsedTime().asMilliseconds();
    while (window.isOpen()) {
        int test_time = sim_clock.getElapsedTime().asMilliseconds();
        if (test_time - prev_time >= MAX_FPS_TIME) {

            // Reset Delay Variabletitle
            prev_time = sim_clock.getElapsedTime().asMilliseconds();

            // Wipe the Screen
            window.clear(sf::Color(248, 248, 248,255));

            // Check For Generic Window Events
            sf::Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::TextEntered:
                        if (event.text.unicode == 8) {
                            userInput = userInput.substring(0,userInput.getSize() - 1);
                        }else {
                            userInput += event.text.unicode;
                        }
                        break;
                    case sf::Event::Resized:
                        // todo: resize the display sprite to be of the new screen size
                    default:
                    break;
                }
            }
            //std::cerr << userInput.toAnsiString() << "\n";

            // Gets the current click
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                mouse_pos = sf::Mouse::getPosition(window);
            } else {
                mouse_pos = sf::Vector2i(-500,-500); // Off the screen
            }

            // Raytrace the scene
            if (MyRaytracer.RefreshImage(RenderedScene)) 
            {
                DisplaySprite.setTexture(RenderedScene);
                window.draw(DisplaySprite);
            }

            window.display();
        }
        
    }

    return 0;
}
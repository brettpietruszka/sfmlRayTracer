#include <iostream>

#include "main.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

int main() {

    sf::CircleShape shape(1.0f);
    shape.setFillColor(sf::Color::Black);

    // SFML data
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML RAYTRACER!!!");
    sf::Vector2i mouse_pos;
    sf::String UserInput;
    sf::Clock GameClock;

    // Game Font
    sf::Font GameFont;
    // TODO: make sure this is relative
    if (!GameFont.loadFromFile("/Users/brettpietruszka/Documents/code/sfmlRayTracer/resource/Arialn.ttf"))
    {
        std::cerr << "FAILED TO LOAD FONT" << std::endl;
    }
    

    sf::Text FPSCounterText;
    FPSCounterText.setPosition(SCREEN_WIDTH * 0.95, SCREEN_HEIGHT * 0.05);
    FPSCounterText.setString(sf::String("0"));
    FPSCounterText.setFillColor(sf::Color::Black);
    FPSCounterText.setFont(GameFont);

    // RayTracer data
    sf::Sprite DisplaySprite = sf::Sprite();

    Camera::ViewPort MyViewPort{1.0f, 1.0f, 1.0f}; // Should have default values
    Camera MyCamera {MyViewPort}; 
    RayTracer MyRaytracer {MyCamera, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Add our 3 shapes
    RayTracer::Sphere RedSphere = RayTracer::Sphere(sf::Vector3f(0.0f, -1.0f, 3.0f), 1, sf::Color(255,0,0));
    MyRaytracer.AddObject(0, RedSphere);

    RayTracer::Sphere BlueSphere = RayTracer::Sphere(sf::Vector3f(2.0f, 0.0f, 4.0f), 1, sf::Color(0,0,255));
    MyRaytracer.AddObject(0, BlueSphere);

    RayTracer::Sphere GreenSphere = RayTracer::Sphere(sf::Vector3f(-2.0f, 0.0f, 4.0f), 1, sf::Color(0,255,0));
    MyRaytracer.AddObject(0, GreenSphere);

    // Start the "game" loop
    sf::Texture RenderedScene = sf::Texture();
    float PrevTime = GameClock.getElapsedTime().asMilliseconds();
    int PrevSeconds = 0;
    int FPSCounter = 0;
    while (window.isOpen()) 
    {
        // Wipe the Screen
        window.clear(sf::Color(0, 255, 0, 255));

        // increment fps counter. this breaks if fps is less than 1
        const int NewGameSeconds = (int)GameClock.getElapsedTime().asSeconds();
        if (NewGameSeconds > PrevSeconds)
        {
            // update fps text
            char* NewStringText = new char;
            std::sprintf(NewStringText, "%d", FPSCounter);
            FPSCounterText.setString(sf::String(NewStringText));
            std::cerr << NewStringText << std::endl;
            delete NewStringText;


            // refresh fps counter
            FPSCounter = 0;
            PrevSeconds = NewGameSeconds;
        }
        else 
        {
            FPSCounter++;
            std::cerr << "Incrementing fps count" << std::endl;
        }

        // Check For Generic Window Events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::TextEntered:
                    if (event.text.unicode == 8) {
                        UserInput = UserInput.substring(0,UserInput.getSize() - 1);
                    }else {
                        UserInput += event.text.unicode;
                    }
                    break;
                case sf::Event::Resized:
                    // todo: resize the display sprite to be of the new screen size
                default:
                break;
            }
        }
        //std::cerr << UserInput.toAnsiString() << "\n";

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

        window.draw(FPSCounterText);
        window.display();
        
    }

    return 0;
}
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button {
    private:
        sf::Sprite unclicked;
        sf::Sprite clicked;
        sf::Sprite* active;

        sf::Texture unclicked_texture; // Texture needs to remain in scope, otherwise sprite turns white
        sf::Texture clicked_texture;

        sf::Text title; // Not Used Currently. Dont even know if this works. 
        sf::Time endTime;
        bool state; // If button is currently pressed, then true. Otherwise false.
        bool activeStatus; // If button is turned on or off

    public:
        Button() = default; // So that compiler warning doesn't show, need to set this as default
        Button (std::string unclicked, std::string clicked, sf::Vector2f*, sf::Vector2f loc);
        bool checkClick (sf::Vector2i);
        void draw(sf::RenderTarget*);
        bool setState(bool);
        void setEndTime(sf::Int32);
        void setText(sf::String);
        void setSize(sf::Vector2f);
        void activate();
        void deactivate();

        bool getState();
        bool isActive();
        sf::Int32 getEndTime();
};



#endif
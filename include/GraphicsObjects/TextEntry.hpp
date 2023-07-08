

#ifndef TEXT_ENTRY_HPP
#define TEXT_ENTRY_HPP

#include <SFML/Graphics.hpp>

class TextEntry {
    private:
    
        sf::Text userText;

        sf::RectangleShape fieldBackground;

        bool active;

    public:
        TextEntry() = default; // So that compiler warning doesn't show, need to set this as default
        TextEntry(sf::Vector2f,sf::Vector2f,sf::String);

        bool checkClick (sf::Vector2i);
        void draw(sf::RenderTarget*);

        void setText(sf::String);
        sf::String getText();
        void setSize(sf::Vector2f);
        void setTextColor();
        void setBackgroundColor();

        float eval();

        void activate();
        void deactivate();

        bool isActive();
};



#endif
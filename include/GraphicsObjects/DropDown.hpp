
#ifndef DROPDOWN_HPP
#define DROPDOWN_HPP

#include "Button.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

class DropDown {
    
 
    public:
        //DropDown (std::vector<std::string> *algorithms);
        DropDown(std::string directory, sf::Vector2f location, sf::Vector2f size);
        ~DropDown();
        sf::Vector2f* dropDownButtonSize;
        sf::Vector2f* pos;

        std::vector<Button*> items;
        bool dropDownOpen;
        std::vector<std::string> mapsNames;

        void drawOne(sf::RenderTarget*);
        void drawAll(sf::RenderTarget*);
        void addOption(std::string);
        Button* getButton(int b);
        std::string getMapName(int c);
        int getDropDownLength();
        bool isOpen();
        
        //void setSize(sf::Vector2f);

};



#endif


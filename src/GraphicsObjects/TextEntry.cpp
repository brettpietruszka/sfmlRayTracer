


// if (event.type == sf::Event::TextEntered)
// {
//     playerInput +=event.text.unicode;
//     playerText.setString(playerInput);
// }

// ...

// window.draw(playerText);


#include "GraphicsObjects/TextEntry.hpp"
#include "globals.hpp"
#include <cmath>
#include <iostream>


TextEntry::TextEntry(sf::Vector2f loc, sf::Vector2f size, sf::String default_string) {
    this->fieldBackground = sf::RectangleShape();

    this->fieldBackground.setSize(size);
    this->fieldBackground.setPosition(loc.x,loc.y);
    this->fieldBackground.setFillColor(sf::Color(255,255,255));

    //this->userText = sf::Text("test", arial_font);
    this->userText.setString(default_string);
    this->userText.setPosition(loc.x,loc.y);
    this->userText.setFillColor(sf::Color::Black);

    this->active = false;
}


bool TextEntry::checkClick(sf::Vector2i mousePos) {
	sf::Vector2f pos((float)mousePos.x, (float)mousePos.y);
	return this->fieldBackground.getGlobalBounds().contains(pos);

	// Basically just does this
	/*
    if (mousePos.x > this->active->getPosition().x && mousePos.x < (this->active->getPosition().x + this->active->getSize().x)) {
        if(mousePos.y > this->active->getPosition().y && mousePos.y < (this->active->getPosition().y + this->active->getSize().y)) {
            setState(!current);
        }
    }*/
}

void TextEntry::draw(sf::RenderTarget* renderable)
{
	renderable->draw(this->fieldBackground);
	renderable->draw(this->userText);
}

void TextEntry::setSize(sf::Vector2f size)
{
	sf::Vector2f orgUnclickedSize = this->fieldBackground.getSize();

	float u_x_ratio = size.x / static_cast<float>(orgUnclickedSize.x);
	float u_y_ratio = size.y / static_cast<float>(orgUnclickedSize.y);

	this->fieldBackground.setScale(u_x_ratio, u_y_ratio);

}

void TextEntry::setText(sf::String text) {
	this->userText.setString(text);
}

sf::String TextEntry::getText() {
    return this->userText.getString();
}

float TextEntry::eval() { 
    // Tries to interpret the value in the text entry as a number
    // if it cannot, it returns NAN
    try {
        return std::stoi(this->userText.getString().toAnsiString());
    } catch(std::exception& e) {
        return std::sqrt(-1);
    }
}

void TextEntry::activate()
{
	// Button can be turned on
	this->active = true;
    this->fieldBackground.setFillColor(sf::Color(200,200,200));
    //UserInput = "";
    //this->userText.setString(UserInput);
}

void TextEntry::deactivate()
{
	// Text Entry Field not currently clicked
	this->active = false;
    this->fieldBackground.setFillColor(sf::Color(255,255,255));
}




bool TextEntry::isActive()
{
	return this->active;
}
#include "GraphicsObjects/Button.hpp"
#include "globals.hpp"

Button::Button(std::string unclicked_file_name, std::string clicked_file_name, sf::Vector2f* size, sf::Vector2f top_left)
{

	if (!this->unclicked_texture.loadFromFile(unclicked_file_name))
	{
		return;
	}

	if (!this->clicked_texture.loadFromFile(clicked_file_name))
	{
		return;
	}

	this->unclicked.setTexture(unclicked_texture);
	this->clicked.setTexture(clicked_texture);

	this->unclicked.setPosition(top_left.x, top_left.y);
	this->clicked.setPosition(top_left.x, top_left.y);

	this->active = &this->unclicked;
	this->state = false;
	this->activeStatus = true;
 
    this->title.setString("");
    this->title.setPosition(top_left.x+10, top_left.y+10);
    this->title.setFillColor(sf::Color::Black);

	if (size)
		this->setSize(*size);
}

bool Button::checkClick(sf::Vector2i mousePos)
{
	if (!this->activeStatus)
	{
		return false;
	}

	sf::Vector2f pos((float)mousePos.x, (float)mousePos.y);
	return this->active->getGlobalBounds().contains(pos);

	// Basically just does this
	/*
    if (mousePos.x > this->active->getPosition().x && mousePos.x < (this->active->getPosition().x + this->active->getSize().x)) {
        if(mousePos.y > this->active->getPosition().y && mousePos.y < (this->active->getPosition().y + this->active->getSize().y)) {
            setState(!current);
        }
    }*/
}

void Button::draw(sf::RenderTarget* renderable)
{
	renderable->draw(*(this->active));
	renderable->draw(this->title);
}

bool Button::setState(bool s)
{
	// Returns true if sucessful, false if otherwise
	if (!this->activeStatus) {
		return false;
	}

	this->state = s;
	if (this->state) {
		this->active = &this->clicked;
	}
	else {
		this->active = &this->unclicked;
	}

	return true;
}

void Button::setSize(sf::Vector2f size)
{
	sf::Vector2u orgUnclickedSize = this->unclicked_texture.getSize();
	sf::Vector2u orgClickedSize = this->clicked_texture.getSize();

	float u_x_ratio = size.x / static_cast<float>(orgUnclickedSize.x);
	float u_y_ratio = size.y / static_cast<float>(orgUnclickedSize.y);
	float c_x_ratio = size.x / static_cast<float>(orgClickedSize.x);
	float c_y_ratio = size.y / static_cast<float>(orgClickedSize.y);

	unclicked.setScale(u_x_ratio, u_y_ratio);
	clicked.setScale(c_x_ratio, c_y_ratio);
}

void Button::activate()
{
	// Button can be turned on
	this->activeStatus = true;
}

void Button::deactivate()
{
	// Button can be turned off
	this->activeStatus = false;
}

void Button::setText(sf::String new_text)
{
	// Didn't use the text. Will probably get rid of this and the text instance variable.
	this->title.setString(new_text);
}

void Button::setEndTime(sf::Int32 eTime)
{
	this->endTime = sf::milliseconds(eTime);
}

sf::Int32 Button::getEndTime()
{
	return this->endTime.asMilliseconds();
}

bool Button::getState()
{
	return this->state;
}

bool Button::isActive()
{
	return this->activeStatus;
}
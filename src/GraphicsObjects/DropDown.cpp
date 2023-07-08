
#include "GraphicsObjects/DropDown.hpp"
#include "GraphicsObjects/Button.hpp"
#include "globals.hpp"
#include <vector> 
#include <string>
#include <iostream>
#include <filesystem>
#include <dirent.h>

DropDown::DropDown(std::string directory, sf::Vector2f location, sf::Vector2f size) {
    this->dropDownButtonSize = new sf::Vector2f(size.x,size.y);
    this->pos = new sf::Vector2f(location.x,location.y);
    this->dropDownOpen = false;
    
    //Read every file in the maps directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            //ignore the blank entries
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                std::string filename = directory + ent->d_name;
                this->mapsNames.push_back(filename);
            }
        }
        closedir (dir);
    } else {
        //Cannot open directory
        perror ("");

    }
    //create a button for every drop down
    for (int i = 0; i < this->mapsNames.size(); i++){
        items.emplace_back(new Button("./resource/blankButton-unclicked.png", "./resource/blankButton-clicked.png", this->dropDownButtonSize, *this->pos + sf::Vector2f(0 ,75*i)));
        items.at(i)->setText(this->mapsNames.at(i));

    }
}

//draw closed drop down
void DropDown::drawOne(sf::RenderTarget* renderable){
	items[0]->draw(renderable);
}

//draw open drop down
void DropDown::drawAll(sf::RenderTarget* renderable){
	for (Button* item : this->items) {
        item->draw(renderable);
    }
}

//add an option for drop down
//not used
void DropDown::addOption(std::string name){
    items.emplace_back(new Button("./resource/blankButton-unclicked.png", "./resource/blankButton-clicked.png", this->dropDownButtonSize, *this->pos)); //sf::Vector2f(1125 ,30 + 75)
    items.at(-1)->setText(name);
}

//Get a button
Button* DropDown::getButton(int b){
	return items[b];
}

//get a map name
std::string DropDown::getMapName(int c){
	return this->mapsNames[c];
}

//Get the number of ites in the drop down. 
int DropDown::getDropDownLength(){
	return items.size();
}

//boolean getter if the drop down is open
bool DropDown::isOpen(){
	return dropDownOpen;
}

DropDown::~DropDown() {
    for (Button* item : this->items) {
        delete item;
    }

    delete this->dropDownButtonSize;
    delete this->pos;
}
    

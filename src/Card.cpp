#include "Card.hpp"
#include <iostream>
#include <string>

Card::Card(std::string color, std::string effect){
    _color = color;
    _effect = effect;
}

bool Card::operator < (const Card& card) const{
    if (get_color() < card.get_color()) return true;
    if (get_color() > card.get_color()) return false;
    else {
        if (get_effect() < card.get_effect()) return true;
        else return false;
    }
}

bool Card::is_compatible(const Card& card) const{
    if (get_color() == "black") return true;
    if (get_color() == card.get_color()) return true;
    if (get_effect() == card.get_effect()) return true;

    return false;
}


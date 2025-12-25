#include "CardMaker.hpp"

#include "Card.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void CardMaker::save() {}
// TODO: safe default setup
std::vector<Card> CardMaker::load(std::string path) {
    std::vector<Card> cards;
    std::fstream file;
    file.open("../saves/" + path + "\"", std::ios::in);
    if (file.is_open()) {
        std::string color, effect;
        int quantity;

        while (file >> color >> effect >> quantity) {
            Card card{color, effect};

            for (int i = 0; i < quantity; i++) {
                cards.push_back(card);
            }
        }
    }
    return cards;
}
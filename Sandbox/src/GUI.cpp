#include "GUI.hpp"

#include <iostream>

void GUI::show_card(const Card& card)  {
    std::string color = card.get_color();
    std::string effect = card.get_effect();
    std::cout << txt_colors[color] << effect << txt_colors["None"];
}

void GUI::show_player_hand(Player& player)  {
    std::vector<Card> hand = player.get_hand();
    for (Card card : hand) {
        show_card(card);
        std::cout << " ";
    }
    std::cout << std::endl;
}

void GUI::top_deck_card(Deck& deck) {
    Card top_card = deck.top();
    std::cout << "Top: ";
    show_card(top_card);
    std::cout << std::endl;
}

void GUI::show_player(Player& player)  {
    std::string name = player.get_name();
    std::cout << "#========================================#" << std::endl;
    std::cout << name << " turn: " << std::endl;
}

void GUI::new_players()  {
    std::cout << "Choose number of player and players names" << std::endl;
}
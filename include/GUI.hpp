#ifndef GUI_HPP
#define GUI_HPP

#include "Deck.hpp"
#include "Player.hpp"

#include <string>
#include <unordered_map>

class GUI {
  private:
    std::unordered_map<std::string, std::string> txt_colors{
        {"Blue", "\033[94m"},
        {"Green", "\033[92m"},
        {"Red", "\033[91m"},
        {"Yellow", "\033[93m"},
        {"Black", "\033[90m"},
        {"None", "\033[0m"},
    };

  public:
    void show_card(const Card& card) ;
    void show_player_hand(Player& player) ;
    void top_deck_card(Deck& deck) ;
    void show_player(Player& player) ;
    void new_players() ;
};

#endif
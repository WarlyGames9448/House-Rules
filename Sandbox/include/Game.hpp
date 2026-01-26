#ifndef GAME_HPP
#define GAME_HPP

#include "CardMaker.hpp"
#include "Deck.hpp"
#include "GUI.hpp"
#include "Player.hpp"

#include <vector>

class Game {
  private:
    std::vector<Player> _players;
    Deck deck;
    Deck discard_deck;
    CardMaker card_maker;
    GUI gui;
    int chosen_player_index = 0;

    void assing_players();
    void make_deck();
    void setup_players(int initial_hand = 7);

    void add_player(Player& player);

    std::vector<Player>& get_players() { return _players; }

  public:
    bool play_turn();
    void skip(int skips = 0);
    void start();

    int k_players() const { return _players.size(); }
};

#endif
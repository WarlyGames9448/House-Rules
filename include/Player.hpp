#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Card.hpp"

#include <string>
#include <vector>

class Player {
  private:
    std::string _name;
    std::vector<Card> _hand;

  public:
    Player(std::string name) : _name(std::move(name)) {}

    void add_card(const Card& card);
    void remove_card(int index);
    void sort_hand();

    std::string get_name() const { return _name; }
    std::vector<Card> get_hand() const { return _hand; }
    int get_hand_size() const {return get_hand().size();}
};

#endif

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Card.hpp"

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
};

#endif

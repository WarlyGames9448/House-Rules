#ifndef DECK_HPP
#define DECK_HPP

#include <vector>

#include "Card.hpp"

class Deck {
private:
    std::vector<Card> _cards;
public:
    void shuffle();

    void add_card(const Card& card){ _cards.push_back(card);}
    void remove_top(){ _cards.pop_back(); }
    Card top() const { return _cards.back(); }

};

#endif
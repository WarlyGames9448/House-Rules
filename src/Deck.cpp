#include <Card.hpp>
#include <Deck.hpp>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(_cards.begin(), _cards.end(), g);
}

void Deck::add_card(const std::vector<Card> cards) {
    for (Card card : cards) {
        add_card(card);
    }
}

Card Deck::top() const {
    if (_cards.size() == 0) {
        throw std::out_of_range("Empty deck.");
    }
    else {
        return _cards.back();
    }
}
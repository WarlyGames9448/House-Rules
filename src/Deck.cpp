#include <vector>
#include <random>
#include <algorithm>

#include <Deck.hpp>
#include <Card.hpp>


void Deck::shuffle(){
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(_cards.begin(), _cards.end(), g);
}
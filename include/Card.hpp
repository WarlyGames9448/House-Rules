#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Game;

class Card {
  private:
    std::string _color;
    std::string _effect;

  public:
    Card(std::string color, std::string effect);
    bool operator<(const Card& card) const;
    bool is_compatible(const Card& card) const;
    void play();

    std::string get_color() const { return _color; }
    std::string get_effect() const { return _effect; }
};

#endif

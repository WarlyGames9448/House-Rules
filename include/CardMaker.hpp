#ifndef CARDMAKER_HPP
#define CARDMAKER_HPP

#include <Card.hpp>
#include <string>
#include <vector>

class CardMaker {
  private:
  public:
    void save(); // TODO
    std::vector<Card> load(std::string path = "default.txt");
};

#endif

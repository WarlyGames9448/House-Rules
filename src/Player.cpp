#include "Player.hpp"

#include <string>
#include <vector>
#include <algorithm>

void Player::add_card(const Card& card){
    _hand.push_back(card);
}
void Player::remove_card(int index){
    _hand.erase(_hand.begin() + index);
}
void Player::sort_hand(){
    sort(_hand.begin(), _hand.end());
}

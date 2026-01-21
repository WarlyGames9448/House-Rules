#include "Game.hpp"

#include "Card.hpp"
#include "CardMaker.hpp"
#include "GUI.hpp"
#include "Player.hpp"

#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_K_PLAYERS = 4;

void Game::add_player(Player& player) {
    _players.push_back(player);
}

void Game::assing_players() {

    gui.new_players();
    int n_players;
    std::cin >> n_players;

    if (n_players == 0) {
        for (int i = 1; i <= DEFAULT_K_PLAYERS; i++) {
            Player player{"Player " + std::to_string(i)};
            add_player(player);
        }
    }

    for (int index = 0; index < n_players; index++) {
        std::string player_name;
        std::cin >> player_name;
        Player player{player_name};
        add_player(player);
    }
}

void Game::make_deck() {
    std::vector<Card> cards = card_maker.load();
    deck.add_card(cards);
    deck.shuffle();
    discard_deck.add_card(deck.top());
    deck.remove_top();
}

void Game::setup_players(int initial_hand) {
    for (Player& player : _players) {
        for (int index = 0; index < initial_hand; index++) {
            player.add_card(deck.top());
            deck.remove_top();
        }
    }
}

void Game::skip(int skips) {
    chosen_player_index = (chosen_player_index + 1 + skips) % k_players();
}

bool Game::play_turn() {
    Player &chosen_player = get_players()[chosen_player_index];
    gui.show_player(chosen_player);
    gui.top_deck_card(discard_deck);
    gui.show_player_hand(chosen_player);

    int index;
    while (true) {
        std::cin >> index;
        if ((1 <= index) && (index <= chosen_player.get_hand_size())) {
            Card chosen_card = chosen_player.get_hand()[index - 1];

            if (chosen_card.is_compatible(discard_deck.top())) {
                chosen_card.play();
                chosen_player.remove_card(index);
                discard_deck.add_card(chosen_card);
                break;
            }
        }
        if (index == 0) {
            break;
        }
    }
    return true;
}

void Game::start() {
    assing_players();
    make_deck();
    setup_players(7);

    while (play_turn()) {
        skip();
    }
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>

using namespace std;

unordered_map<string, string> txt_colors{
    {"Blue", "\033[94m"},
    {"Green", "\033[92m"},
    {"Red", "\033[91m"},
    {"Yellow", "\033[93m"},
    {"Black", "\033[90m"},
};
/*

void display(Player& player, Deck& deck){
    cout << endl << "#--------------------#" << endl;
    Card top_card =  deck.top();
    cout << "Top card: " << txt_colors[top_card.color] << top_card.effect << "\033[0m" << endl;

    cout << player.name << " Hand:" << endl;

    // player's hand
    for (Card card : player.hand){
        cout << txt_colors[card.color] << card.effect << " \033[0m";
    }
    cout << endl;
}

Player start(){
    vector<Player> players { {"Wesley", {} },{"Zé", {} } };
    //vector<Player> players {{"wesley", {}}};
    Deck deck = load_cards();
    Deck discard_deck;
    discard_deck.add_card(deck.top(), 1);
    deck.remove_top();

    //initial setup
    for (Player& player : players){
        player.draw_cards(deck, 7);
    }

    while(true){
        for (Player& player: players){
            int chosen = 0;
            bool drawn = false;

            while(true) {
                player.sort_hand();
                display(player, discard_deck);

                //chose witch card will be played
                while(!(cin >> chosen)){
                    cout << "Invalid input." << endl;
                    display(player, discard_deck);
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                //0 for draw or skip
                if (chosen == 0){
                    if(drawn == false) player.draw_cards(deck, 1);
                    else break;
                }
                //check if card chosen is compatible
                else if (player.hand[chosen - 1].is_compatible(discard_deck.top())){
                    player.play_card(chosen - 1, discard_deck);
                    if (size(player.hand) <= 0) return player;
                    break;
                }
            }

        }
    }
}

*/

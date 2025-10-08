#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>

using namespace std;

vector<string> colors = {"Blue", "Green", "Red", "Yellow"};
vector<string> effects = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "block", "reverse", "+2"};
vector<string> black_effects = {"wild", "+4"};

unordered_map<string, string> txt_colors{
    {"Blue", "\033[94m"},
    {"Green", "\033[92m"},
    {"Red", "\033[91m"},
    {"Yelllow", "\033[93m"},
    {"Black", "\033[90m"},
};

class Card{
public:
    string color;
    string effect;
};

class Player
{
    public:
    string name;
    vector<Card> hand;

    void add_card(vector<Card> cards){
        for(Card card: cards){
            hand.push_back(card);
        }
    }
};

class Deck
{
    vector<Card> cards;
public:

    void add_card(Card card, int quantity){
        for(;quantity>0; quantity--) {cards.push_back(card);}
    }

    //return the last played card.
    Card top() {return cards.back();}

    void shuffle_cards(){
        random_device rd;
        mt19937 g(rd());

        shuffle(cards.begin(), cards.end(), g);
    }

    Player draw_cards(Player player, int quantity){
        vector<Card> cards_drawn{cards.end() - quantity, cards.end()};
        player.add_card(cards_drawn);

        return player;
    }
};

void save_cards(){
    fstream file;
    file.open("../saves/cards_setup.txt", ios::out);
    if (file.is_open()) {
        for (string color : colors){
            for (string effect: effects){
                file << color << " " << effect << endl;
            }
        }

        for (string effect : black_effects){
            file << "Black " << effect << endl;
        }
        file.close();
    }
}

Deck load_cards(){
    Deck deck;
    fstream file;
    file.open("../saves/cards_setup.txt", ios::in);
    if (file.is_open()) {
        string line;
        while(getline(file, line)){
            istringstream iss(line);
            string color, effect;
            iss >> color >> effect;
            Card card{color, effect};
            deck.add_card(card, 4);
        }
        file.close();
    }

    deck.shuffle_cards();

    return deck;
}

//Distribute

void display(Player player, Deck deck){
    Card top_card =  deck.top();
    cout << "Top card: " << txt_colors[top_card.color] << top_card.effect << "\033[0m" << endl;

    cout << player.name << " Hand:" << endl;

    // player's hand
    for (Card card : player.hand){
        cout << txt_colors[card.color] << card.effect << " \033[0m";
    }
    cout << endl;
}

void start(){
    Player player {"Wesley", {}};
    Deck deck = load_cards();
    player = deck.draw_cards(player, 7);
    display(player, deck);
}

int main(){
    start();
    return 0;
}

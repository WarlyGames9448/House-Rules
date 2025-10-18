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
    {"Yellow", "\033[93m"},
    {"Black", "\033[90m"},
};

class Card{
public:
    string color;
    string effect;

    bool operator < (Card card){
        if (color < card.color) return true;
        if (color > card.color) return false;
        else {
            if (effect < card.effect) return true;
            else return false;
        }
    }

    bool is_compatible(Card card){
        if (color == "black") return true;
        if (color == card.color) return true;
        if (effect == card.effect) return true;

        return false;
    }
};

class Deck
{
    vector<Card> cards;
public:

    void add_card(Card card, int quantity = 1){
        for(;quantity>0; quantity--) {cards.push_back(card);}
    }

    void remove_top(){
        cards.pop_back();
    }

    //return the last played card.
    Card top() {return cards.back();}

    void shuffle_cards(){
        random_device rd;
        mt19937 g(rd());

        shuffle(cards.begin(), cards.end(), g);
    }

    vector<Card> get_cards(const int quantity){
        vector<Card> cards_get {cards.end() - quantity, cards.end()};
        cards = {cards.begin(), cards.end() - quantity};
        return cards_get;
    }
};

class Player
{
    public:
    string name;
    vector<Card> hand;

    //add a list of cards in players.hand
    void add_card(vector<Card>& cards){
        for(Card card: cards){
            hand.push_back(card);
        }
    }

    void play_card(int index, Deck& deck){
        deck.add_card(hand[index]);
        hand.erase(hand.begin() + index);
    }

    void sort_hand(){
        sort(hand.begin(), hand.end());
    }

    void draw_cards(Deck& deck, const int quantity){
        vector<Card> cards = deck.get_cards(quantity);
        add_card(cards);
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


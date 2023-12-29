#include <iostream>
#include <ctime>
#include <random>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"

using namespace std;

int main()
{
    srand(time(NULL));

    Game game = Game(46);

    // cout << "Choose Player" << endl;
    // int player_number;
    // cin >> player_number;
    // cout << "Welcome Player " << player_number << endl;

    int player_number = 0;

    while (true)
    {
        string input = "";

        cin >> input;

        if (input == "quit")
        {
            break;
        }
        else if (input == "set")
        {
            cout << "Set size " << game.set_size() << endl;
        }
        else if (input == "pile")
        {
            game.display_discard_pile();
        }
        else if (input == "hand")
        {
            cout << "Display hand:" << endl;
            game.display_player_hand(player_number);
        }
        else if (input == "sort")
        {
            cout << "Sorted hand." << endl;
            game.sort_player_hand(player_number);
        }
        else if (input == "draw")
        {
            cout << "Player " << player_number << " draws a tile." << endl;
            game.player_draw(player_number);
        }
        else if (input == "discard")
        {
            cout << "Player " << player_number << " discards a tile." << endl;
            game.player_discard(player_number);
        }
        else
        {
            cout << "Unknown input " << input << endl;
        }

        // cout << input << endl;
    }
};

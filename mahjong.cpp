/*
Mahjong in C++
*/

#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Player.hpp"

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
    game.set_human(player_number);

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
            cout << "Set size " << game.get_set_size() << endl;
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
            game.player_draw(player_number, true);
        }
        else if (input == "discard")
        {
            cout << "Player " << player_number << " discards a tile." << endl;
            game.player_discard(player_number);
        }
        else if (input == "turn")
        {
            game.player_draw(player_number, true);
            game.sort_player_hand(player_number);
            game.display_player_hand(player_number);
            game.player_discard(player_number);
        }
        else if (input == "full_turn")
        {
            std::vector<Player> players = game.get_players();
            for (size_t current_player = 0; current_player < players.size(); current_player++)
            {
                cout << "Player " << current_player << "'s turn:" << endl;
                bool broadcast = current_player == player_number;
                game.player_draw(current_player, broadcast);
                if (current_player == player_number)
                {
                    game.sort_player_hand(current_player);
                    game.display_player_hand(current_player);
                }
                else
                {
                    game.display_visible_player_hand(current_player);
                }
                game.player_discard(current_player);
            }
        }
        else if (input == "game")
        {
            unsigned int current_player;
            bool broadcast;
            while (game.is_running())
            {
                current_player = game.get_current_player();
                cout << "Player " << current_player << "'s turn:" << endl;
                broadcast = current_player == player_number;
                if (current_player == player_number && game.get_pile_size() > 0)
                {
                    game.display_discard_pile();
                }
                game.player_turn(current_player, broadcast);
                std::vector<std::string> player_actions = {};
                for (size_t i = 0; i < N_PLAYERS; i++)
                {
                    // std::cout << "Player " << i << " is checking pickup actions..." << std::endl;
                    std::string player_action = game.player_choose_pickup_action(i, current_player);
                    player_actions.push_back(player_action);
                    // std::cout << "Player " << i << " chooses " << player_action << std::endl;
                }
                std::tuple<int, std::string> pickup_tuple = game.prioritize_pickup_action(player_actions);
                if (get<1>(pickup_tuple) != "none")
                {
                    current_player = get<0>(pickup_tuple);
                    std::string action = get<1>(pickup_tuple);
                    broadcast = current_player == player_number;
                    std::cout << "Player " << current_player << " performs " << action << "." << std::endl;
                    game.player_pick_from_discard(current_player, action);
                    if (broadcast)
                    {
                        game.sort_player_hand(current_player);
                        game.display_player_hand(current_player);
                    }
                    game.player_discard(current_player);
                    game.set_current_player(get<0>(pickup_tuple) + 1);
                }

                current_player = (current_player + 1) % N_PLAYERS;
                game.set_current_player(current_player);

                std::cout << "" << std::endl;

                if (game.get_set_size() == 0)
                {
                    game.finish();
                }
            }
            // cout << game.is_running();
        }
        else
        {
            cout << "Unknown input " << input << endl;
        }

        // cout << input << endl;
    }
};

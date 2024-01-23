/*
Mahjong in C++
*/

#include <iostream>
#include <ctime>
#include <random>
#include <thread>
#include <vector>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Player.hpp"

using namespace std;

int main()
{
    srand(time(NULL));

    Mahjong::Game game = Mahjong::Game(46);

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
            std::vector<Mahjong::Player> players = game.get_players();
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
            if (game.get_set_size() == 0)
            {
                srand(time(NULL));

                Mahjong::Game game = Mahjong::Game(46);

                int player_number = 0;
                game.set_human(player_number);
            }

            unsigned int current_player;
            bool broadcast;

            // First turn
            current_player = game.get_current_player();
            cout << "Player " << current_player << "'s turn:" << endl;
            broadcast = (current_player == player_number);
            game.player_turn(current_player, broadcast);
            std::cout << "" << std::endl;

            while (game.is_running())
            {
                // Check if any pickup actions are performed.
                std::tuple<int, std::string> pickup_tuple = game.pickup_action(current_player);
                std::string action = get<1>(pickup_tuple);

                if (action != "none")
                {
                    current_player = get<0>(pickup_tuple);
                    broadcast = (current_player == player_number);
                    std::cout << "Player " << current_player << " performs " << action << "." << std::endl;
                    game.player_pick_from_discard(current_player, action);
                    if (broadcast)
                    {
                        // std::cout << "Yes broadcast" << std::endl;
                        game.sort_player_hand(current_player);
                        game.display_player_hand(current_player);
                        game.display_player_score(current_player, true);
                    }
                    else
                    {
                        // std::cout << "No broadcast" << std::endl;
                        game.display_visible_player_hand(current_player);
                        game.display_player_score(current_player, false);
                    }
                    game.player_has_winning_hand(current_player);
                    if (game.is_running())
                        game.player_discard(current_player);
                    game.set_current_player(get<0>(pickup_tuple));
                }
                else
                {
                    // std::cout << "No actions performed." << std::endl;
                    current_player = (current_player + 1) % N_PLAYERS;
                    game.set_current_player(current_player);
                    broadcast = (current_player == player_number);

                    if (current_player == player_number && game.get_pile_size() > 0)
                    {
                        game.display_discard_pile();
                    }

                    cout << "Player " << current_player << "'s turn:" << endl;
                    game.player_turn(current_player, broadcast);
                }

                std::this_thread::sleep_for(500ms);
                std::cout << "\n";

                if (game.get_set_size() == 0)
                {
                    std::cout << "Game finished due to running out of tiles." << std::endl;
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

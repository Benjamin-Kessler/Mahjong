/*
Mahjong in C++
*/

#include <iostream>
#include <ctime>
#include <random>
#include <thread>
#include <vector>

#include "include/Tile.hpp"
#include "include/Set.hpp"
#include "include/Game.hpp"
#include "include/Player.hpp"

using namespace std;

int main()
{
    srand(time(NULL));

    Mahjong::Game game = Mahjong::Game(46);

    while (true)
    {
        string input = "";

        cin >> input;

        if (input == "quit")
        {
            break;
        }
        else if (input == "game")
        {
            int player_number = 0;
            game.set_human(player_number);
            for (int i = 0; i < N_PLAYERS; i++)
            {
                if (i == player_number)
                    continue;
                game.set_player_policy(i, "tile_count");
            }

            if (game.get_set_size() == 0)
            {
                game.reset();
                game.set_human(player_number);
            }

            unsigned int current_player;
            bool broadcast;

            bool multiple_rounds = true;
            while (multiple_rounds)
            {

                // First turn
                current_player = game.get_current_player();
                cout << "Player " << current_player << "'s turn (" << game.get_players()[current_player].get_seat_wind().get_wind_as_string() << "):" << endl;
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
                        else
                        {
                            for (int i = 0; i < N_PLAYERS; i++)
                            {
                                cout << "Player " << i << " - ";
                                game.display_player_score(i, true);
                            }
                            cout << "\n";
                        }
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

                        cout << "Player " << current_player << "'s turn (" << game.get_players()[current_player].get_seat_wind().get_wind_as_string() << "):" << endl;
                        game.player_turn(current_player, broadcast);
                    }

                    std::this_thread::sleep_for(250ms);
                    std::cout << "\n";

                    if (game.get_set_size() == 0)
                    {
                        std::cout << "Game finished due to running out of tiles." << std::endl;
                        for (int i = 0; i < N_PLAYERS; i++)
                        {
                            cout << "Player " << i << " - ";
                            game.display_player_score(i, true);
                        }
                        game.finish();
                        cout << "\n";
                    }
                }

                cout << "Start next round (Y/n)?";
                string reply = "";
                cin >> reply;

                if (reply == "Y")
                {
                    game.next_round();
                    int player_number = 0;
                    game.set_human(player_number);
                    for (int i = 0; i < N_PLAYERS; i++)
                    {
                        if (i == player_number)
                            continue;
                        game.set_player_policy(i, "tile_count");
                    }
                }
                else
                {
                    multiple_rounds = false;
                }
            }
        }
        else if (input == "sim")
        {
            int player_number = -1;

            game.set_player_policy(0, "tile_count");

            if (game.get_set_size() == 0)
            {
                game.reset();
                game.set_player_policy(0, "tile_count");
            }

            unsigned int current_player;
            bool broadcast = false;

            // First turn
            current_player = game.get_current_player();
            cout << "Player " << current_player << "'s turn:" << endl;
            game.player_turn(current_player, broadcast);
            std::cout << "\n";
            cout << game.is_running() << "\n";

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

                    // std::cout << "No broadcast" << std::endl;
                    game.display_visible_player_hand(current_player);
                    game.display_player_score(current_player, broadcast, false);

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
                    cout << "Player " << current_player << "'s turn:" << endl;
                    game.player_turn(current_player, broadcast);
                }

                std::cout << "\n";

                if (game.get_set_size() == 0)
                {
                    std::cout << "Game finished due to running out of tiles.\n";
                    for (int i = 0; i < N_PLAYERS; i++)
                    {
                        cout << "Player " << i << " - ";
                        game.display_player_score(i, true);
                    }
                    game.finish();
                    cout << "\n";
                }
            }
        }
        else
        {
            cout << "Unknown input " << input << endl;
        }

        // cout << input << endl;
    }
};

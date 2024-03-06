#include <iostream>
#include <ctime>
#include <random>
#include <thread>
#include <vector>

#include "include/Tile.hpp"
#include "include/Set.hpp"
#include "include/Game.hpp"
#include "include/Player.hpp"

unsigned int N_GAMES = 5000;

using namespace std;

int main()
{

    srand(time(NULL));

    cout.setstate(ios_base::failbit);

    Mahjong::Game game = Mahjong::Game(46);

    vector<int> player_wins = {0, 0, 0, 0};
    vector<int> player_scores = {0, 0, 0, 0};

    for (unsigned int n = 0; n < N_GAMES; n++)
    {
        if (n % 100 == 0)
        {
            cout.clear();
            cout << "Starting game number " << n << endl;
            cout.setstate(ios_base::failbit);
        }

        game.reset();
        game.set_player_policy(0, "tile_count");

        unsigned int current_player = rand() % 4;
        game.set_current_player(current_player);

        game.player_turn(current_player, false);

        while (game.is_running())
        {
            tuple<int, string> pickup_tuple = game.pickup_action(current_player);
            string action = get<1>(pickup_tuple);

            if (action != "none")
            {
                current_player = get<0>(pickup_tuple);
                game.player_pick_from_discard(current_player, action);

                game.player_has_winning_hand(current_player);
                if (game.is_running())
                    game.player_discard(current_player);
                else
                {
                    player_wins[current_player] += 1;
                    for (int i = 0; i < N_PLAYERS; i++)
                    {
                        player_scores[i] += game.get_player_score(i, true, (i == current_player));
                    }
                }
                game.set_current_player(get<0>(pickup_tuple));
            }
            else
            {
                current_player = (current_player + 1) % N_PLAYERS;
                game.set_current_player(current_player);
                game.player_turn(current_player, false);
            }

            if (game.get_set_size() == 0)
            {
                for (int i = 0; i < N_PLAYERS; i++)
                {
                    player_scores[i] += game.get_player_score(i, true, false);
                }
                game.finish();
                cout << "\n";
            }
        }
    }

    for (int i = 0; i < N_PLAYERS; i++)
    {
        int n_wins = player_wins[i];
        float average_score = player_scores[i] / N_GAMES;

        cout.clear();
        cout << "Player " << i << ":\nNumber of wins: " << n_wins << "\nAverage score: " << average_score << endl;
    }
}
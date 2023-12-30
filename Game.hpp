#pragma once
#include <vector>
#include <iostream>

#include "Set.hpp"
#include "Player.hpp"
#include "Discard_pile.hpp"

unsigned int N_PLAYERS = 4;

class Game
{
private:
    int id;
    std::vector<Player> players;
    Set set;
    Discard_pile discard_pile;

public:
    Game(int id)
    {
        Game::id = id;
        std::cout << "Initiated Game with ID " << id << std::endl;

        Game::discard_pile = Discard_pile();

        Game::set = Set();
        Game::set.shuffle();
        std::cout
            << "Number of tiles in set: " << set.get_size() << std::endl;

        Game::players = {};
        for (size_t player_number = 0; player_number < N_PLAYERS; player_number++)
        {
            players.push_back(Player(player_number, Game::set));
            std::cout << "Initiated Player " << player_number << std::endl;
        }

        std::cout
            << "Number of tiles in set: " << set.get_size() << std::endl;
    }

    void update_set(Set set)
    {
        Game::set = set;
    }

    void display_player_hand(int player_number)
    {
        Player &player = Game::players[player_number];
        player.display_hand();
    }

    void display_visible_player_hand(int player_number)
    {
        Player &player = Game::players[player_number];
        player.display_visible_hand();
    }

    void sort_player_hand(int player_number)
    {
        Player &player = Game::players[player_number];
        player.sort_player_hand();
    }

    void player_draw(int player_number, bool broadcast)
    {
        Player &player = Game::players[player_number];
        player.draw_tile(Game::set, broadcast);
    }

    void player_discard(int player_number)
    {
        Player &player = Game::players[player_number];
        player.discard_tile(Game::discard_pile);
    }

    int set_size()
    {
        return Game::set.get_size();
    }

    void display_discard_pile()
    {
        Game::discard_pile.display_discard_pile();
    }

    void set_human(int player_number)
    {
        Player &player = Game::players[player_number];
        player.set_human();
    }

    std::vector<Player> get_players()
    {
        return Game::players;
    }
};

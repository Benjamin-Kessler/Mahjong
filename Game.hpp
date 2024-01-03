#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <tuple>

#include "Set.hpp"
#include "Player.hpp"
#include "Discard_pile.hpp"

unsigned int N_PLAYERS = 4;

class Game
{
private:
    int id;
    bool running;
    std::vector<Player> players;
    Set set;
    Discard_pile discard_pile;
    unsigned int current_player;

public:
    Game(int id_in) : id(id_in), running(true), current_player(0)
    {
        std::cout << "Initiated Game with ID " << id << std::endl;

        discard_pile = Discard_pile();

        set = Set();
        set.shuffle();
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

    void display_player_hand(unsigned int player_number)
    {
        Player &player = Game::players[player_number];
        player.display_hand();
    }

    void display_visible_player_hand(unsigned int player_number)
    {
        Player &player = Game::players[player_number];
        player.display_visible_hand();
    }

    void sort_player_hand(unsigned int player_number)
    {
        Player &player = Game::players[player_number];
        player.sort_player_hand();
    }

    void player_draw(unsigned int player_number, bool broadcast)
    {
        Player &player = Game::players[player_number];
        player.draw_tile(Game::set, broadcast);
    }

    void player_pick_from_discard(unsigned int player_number, std::string action)
    {
        Player &player = Game::players[player_number];
        Tile tile_to_pickup = Game::discard_pile.back();
        player.pick_tile_from_discard(Game::discard_pile);
        player.reveal_combination(tile_to_pickup, action);
    }

    void player_discard(unsigned int player_number)
    {
        Player &player = Game::players[player_number];
        player.discard_tile(Game::discard_pile);
    }

    std::string player_choose_pickup_action(unsigned int player_number, unsigned int current_player)
    {
        Player &player = Game::players[player_number];
        return player.choose_pickup_action(Game::discard_pile, current_player);
    }

    std::tuple<int, std::string> prioritize_pickup_action(std::vector<std::string> player_actions)
    {
        auto index = std::find(player_actions.begin(), player_actions.end(), "kong") - player_actions.begin();
        if (index < player_actions.size())
        {
            return std::make_tuple(static_cast<int>(index), "kong");
        }
        index = std::find(player_actions.begin(), player_actions.end(), "pong") - player_actions.begin();
        if (index < player_actions.size())
        {
            return std::make_tuple(static_cast<int>(index), "pong");
        }
        index = std::find(player_actions.begin(), player_actions.end(), "chow") - player_actions.begin();
        if (index < player_actions.size())
        {
            return std::make_tuple(static_cast<int>(index), "chow");
        }
        return std::make_tuple(-1, "none");
    }

    void player_turn(unsigned int player_number, bool broadcast)
    {
        player_draw(player_number, broadcast);
        sort_player_hand(player_number);
        if (broadcast)
        {
            display_player_hand(player_number);
        }
        else
        {
            display_visible_player_hand(player_number);
        }
        player_discard(player_number);
        // display_discard_pile();
    }

    int get_set_size() const
    {
        return Game::set.get_size();
    }

    unsigned int get_pile_size() const
    {
        return Game::discard_pile.get_size();
    }

    void display_discard_pile() const
    {
        Game::discard_pile.display_discard_pile();
    }

    void set_human(unsigned int player_number)
    {
        Player &player = Game::players[player_number];
        player.set_human();
    }

    std::vector<Player> get_players() const
    {
        return Game::players;
    }

    bool is_running() const
    {
        return running;
    }

    void finish()
    {
        running = false;
    }

    unsigned int get_current_player() const
    {
        return current_player;
    }

    void set_current_player(unsigned int new_current_player)
    {
        current_player = new_current_player;
    }
};

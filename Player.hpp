#pragma once

#include <algorithm>
#include <random>

#include "Hand.hpp"
#include "Set.hpp"
#include "Discard_pile.hpp"

float STARTING_MONEY = 100;

class Player
{
private:
    int player_number;
    bool is_human = false;
    float money;
    Hand hand;

public:
    Player(int number, Set &set) : player_number(number), money(STARTING_MONEY), hand()
    {
        hand.draw_hand(set);
    }

    void display_hand()
    {
        Player::hand.display_hand();
    }

    void display_visible_hand()
    {
        Player::hand.display_visible_hand();
    }

    void sort_player_hand()
    {
        Player::hand.sort();
    }

    void draw_tile(Set &set, bool broadcast)
    {
        Player::hand.draw_tile(set, broadcast);
    }

    void pick_tile_from_discard(Discard_pile &discard_pile)
    {
        Player::hand.pick_tile_from_discard(discard_pile);
    }

    void discard_tile(Discard_pile &discard_pile)
    {
        if (Player::is_human)
        {
            Player::hand.discard_tile(discard_pile);
        }
        else
        {
            Player::hand.discard_random_tile(discard_pile);
        }
    }

    std::string choose_pickup_action(Discard_pile &discard_pile, unsigned int current_player)
    {
        std::vector<std::string> available_actions = hand.check_available_actions(discard_pile, player_number, current_player);
        // std::cout << player_number << ", " << is_human << ", " << available_actions.size() << std::endl;
        if (is_human && available_actions.size() > 0)
        {
            std::
                    cout
                << "Available actions:" << std::endl;
            for (size_t i = 0; i < available_actions.size(); i++)
            {
                std::cout << i << ": " << available_actions[i] << std ::endl;
            }
            int chosen_action;
            std::cout << "Select action:" << std::endl;
            std::cin >> chosen_action;
            if (chosen_action == -1)
            {
                return "none";
            }
            assert(chosen_action < available_actions.size());
            return available_actions[chosen_action];
        }
        else if (available_actions.size() > 0)
        {
            unsigned seed = std::time(0);
            std::srand(seed);
            return available_actions[std::rand() % available_actions.size()];
        }
        return "none";
    }

    void set_human()
    {
        Player::is_human = true;
    }

    void reveal_combination(Tile tile, std::string action)
    {
        Player::hand.reveal_combination(tile, action);
    }
};
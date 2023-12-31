#pragma once

#include "Hand.hpp"
#include "Set.hpp"
#include "Discard_pile.hpp"

float STARTING_MONEY = 100;

class Player
{
private:
    int playerNumber;
    bool is_human = false;
    float money;
    Hand hand;

public:
    Player(int number, Set &set) : playerNumber(number), money(STARTING_MONEY), hand()
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

    void call_pong(Discard_pile &Discard_pile)
    {
        }

    void set_human()
    {
        Player::is_human = true;
    }
};
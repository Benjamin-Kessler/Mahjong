#pragma once

#include "Hand.hpp"
#include "Set.hpp"
#include "Discard_pile.hpp"

float STARTING_MONEY = 100;

class Player
{
private:
    int playerNumber;
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

    void sort_player_hand()
    {
        Player::hand.sort();
    }

    void draw_tile(Set &set)
    {
        Player::hand.draw_tile(set);
    }

    void discard_tile(Discard_pile &discard_pile)
    {
        Player::hand.discard_tile(discard_pile);
    }
};
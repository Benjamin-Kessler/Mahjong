#pragma once

#include <cmath>
#include <vector>

#include "Discard_pile.hpp"
#include "Hand.hpp"

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    class State
    {
    private:
        unsigned int player_number;
        std::vector<Mahjong::Hand> hands;
        Mahjong::Discard_pile discard_pile;

    public:
        State(){};

        State(unsigned int input_player_number, std::vector<Mahjong::Hand> input_hands, Mahjong::Discard_pile input_discard_pile)
            : player_number(input_player_number), hands(input_hands), discard_pile(input_discard_pile) {}

        int score_state()
        {
            int score = 0;
            for (int index = 0; index < hands.size(); index++)
            {
                auto scores = hands[index].get_max_score();
                int player_score = std::get<0>(scores) * pow(2, std::get<1>(scores));
                if (index == player_number)
                    score += player_score;
                else
                    score -= player_score;
            }
            return score;
        }
    };
} // namespace Mahjong

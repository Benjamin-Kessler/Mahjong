#pragma once

#include <cmath>
#include <vector>

#include "Discard_pile.hpp"
#include "Hand.hpp"
#include "Tile.hpp"

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

        unsigned int get_player_number() const
        {
            return player_number;
        }

        Mahjong::Hand get_player_hand(int input_player_number) const
        {
            return hands[input_player_number];
        }

        Mahjong::Discard_pile get_discard_pile() const
        {
            return discard_pile;
        }

        unsigned int get_n_tile_occurence(Mahjong::Tile tile)
        {
            unsigned int n_occurence = discard_pile.get_n_tile_occurence(tile);
            for (Mahjong::Hand hand : hands)
                n_occurence += hand.get_n_tile_occurence(tile);
            return n_occurence;
        }

        unsigned int get_n_used_tiles() const
        {
            unsigned int n_used_tiles = discard_pile.get_size();
            for (Mahjong::Hand hand : hands)
                n_used_tiles += hand.get_hand_size();
            return n_used_tiles;
        }

        unsigned int get_n_unused_tiles() const
        {
            return 136 - get_n_used_tiles();
        }

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

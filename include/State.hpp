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
    /**
     * @brief Class representing the state of a Mahjong game.
     */
    class State
    {
    private:
        unsigned int player_number;         ///< The index of the current player.
        std::vector<Mahjong::Hand> hands;   ///< The hands of all players.
        Mahjong::Discard_pile discard_pile; ///< The discard pile.

    public:
        /**
         * @brief Default constructor for State class.
         */
        State(){};

        /**
         * @brief Parameterized constructor for State class.
         *
         * @param input_player_number The index of the current player.
         * @param input_hands The hands of all players.
         * @param input_discard_pile The discard pile.
         */
        State(unsigned int input_player_number, std::vector<Mahjong::Hand> input_hands, Mahjong::Discard_pile input_discard_pile)
            : player_number(input_player_number), hands(input_hands), discard_pile(input_discard_pile) {}

        /**
         * @brief Retrieves the index of the current player.
         *
         * @return The index of the current player.
         */
        unsigned int get_player_number() const
        {
            return player_number;
        }

        /**
         * @brief Retrieves the hand of a specific player.
         *
         * @param input_player_number The index of the player whose hand is to be retrieved.
         * @return The hand of the specified player.
         */
        Mahjong::Hand get_player_hand(int input_player_number) const
        {
            return hands[input_player_number];
        }

        /**
         * @brief Retrieves the discard pile.
         *
         * @return The discard pile.
         */
        Mahjong::Discard_pile get_discard_pile() const
        {
            return discard_pile;
        }

        /**
         * @brief Counts the number of occurrences of a tile in the game state.
         *
         * @param tile The tile whose occurrences are to be counted.
         * @return The number of occurrences of the specified tile in the game state.
         */
        unsigned int get_n_tile_occurence(Mahjong::Tile tile)
        {
            unsigned int n_occurence = discard_pile.get_n_tile_occurence(tile);
            for (Mahjong::Hand hand : hands)
                n_occurence += hand.get_n_tile_occurence(tile);
            return n_occurence;
        }

        /**
         * @brief Calculates the total number of used tiles in the game state.
         *
         * @return The total number of used tiles in the game state.
         */
        unsigned int get_n_used_tiles() const
        {
            unsigned int n_used_tiles = discard_pile.get_size();
            for (Mahjong::Hand hand : hands)
                n_used_tiles += hand.get_hand_size();
            return n_used_tiles;
        }

        /**
         * @brief Calculates the total number of unused tiles in the game state.
         *
         * @return The total number of unused tiles in the game state.
         */
        unsigned int get_n_unused_tiles() const
        {
            return 136 - get_n_used_tiles();
        }

        /**
         * @brief Scores the game state.
         *
         * This method scores the game state based on the hands of all players.
         *
         * @return The score of the game state.
         */
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

/**
 * @file Player.hpp
 * @brief Defines the Player class representing a player in a Mahjong game.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <random>

#include "Hand.hpp"
#include "Set.hpp"
#include "Discard_pile.hpp"

/** @brief Initial money for each player. */
float STARTING_MONEY = 100;

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    /**
     * @class Player
     * @brief Represents a player in a Mahjong game.
     *
     * The Player class encapsulates the attributes and actions of a player in the game,
     * including their hand, money, player number, and methods for drawing, discarding tiles,
     * and choosing actions based on the game state.
     */
    class Player
    {
    private:
        unsigned int player_number; /**< The unique identifier for the player. */
        bool is_human = false;      /**< Flag indicating if the player is a human player. */
        float money;                /**< The amount of money the player has. */
        Mahjong::Hand hand;         /**< The player's hand of tiles. */

    public:
        /**
         * @brief Constructor for Player class.
         * @param number The unique player number.
         * @param set Reference to the tile set for drawing initial tiles.
         */
        Player(unsigned int number, Mahjong::Set &set) : player_number(number), money(STARTING_MONEY), hand()
        {
            hand.draw_hand(set);
        }

        /**
         * @brief Display the player's complete hand.
         */
        void display_hand() const
        {
            hand.display_hand();
        }

        /**
         * @brief Display only the visible (non-hidden) tiles in the player's hand.
         */
        void display_visible_hand() const
        {
            hand.display_visible_hand();
        }

        /**
         * @brief Sort the tiles in the player's hand.
         */
        void sort_player_hand()
        {
            hand.sort();
        }

        /**
         * @brief Draw a tile from the set.
         * @param set Reference to the tile set.
         * @param broadcast Flag indicating whether to broadcast the action.
         */
        void draw_tile(Mahjong::Set &set, bool broadcast)
        {
            hand.draw_tile(set, broadcast);
        }

        /**
         * @brief Pick a tile from the discard pile and add it to the player's hand.
         * @param discard_pile Reference to the discard pile.
         */
        void pick_tile_from_discard(Discard_pile &discard_pile)
        {
            hand.pick_tile_from_discard(discard_pile);
        }

        /**
         * @brief Discard a tile from the player's hand.
         * @param discard_pile Reference to the discard pile.
         */
        void discard_tile(Discard_pile &discard_pile)
        {
            if (is_human)
            {
                hand.discard_tile(discard_pile);
            }
            else
            {
                hand.discard_random_tile(discard_pile);
            }
        }

        /**
         * @brief Choose a pickup action based on the game state.
         * @param discard_pile Reference to the discard pile.
         * @param current_player The player who discarded the last tile.
         * @return The chosen action as a string.
         */
        std::string choose_pickup_action(Discard_pile &discard_pile, unsigned int current_player)
        {
            std::vector<std::string> available_actions = hand.check_available_actions(discard_pile, player_number, current_player);
            // std::cout << player_number << ", " << is_human << ", " << available_actions.size() << std::endl;
            if (is_human && available_actions.size() > 0)
            {
                std::cout << "Available actions:" << std::endl;
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
                available_actions.push_back("none");
                unsigned seed = std::time(0);
                std::srand(seed);
                return available_actions[std::rand() % available_actions.size()];
            }
            return "none";
        }

        /**
         * @brief Set the player as human.
         */
        void set_human()
        {
            is_human = true;
        }

        bool check_human()
        {
            return is_human;
        }

        /**
         * @brief Reveal a combination (set of tiles) based on a pickup action.
         * @param tile The tile that triggered the action.
         * @param action The pickup action (kong, pong, chow) to reveal.
         */
        void reveal_combination(Mahjong::Tile tile, std::string action)
        {
            hand.reveal_combination(tile, action, is_human);
        }

        /**
         * @brief Check if this player's hand is a winning Mahjong hand.
         *
         * This function checks if the specified player has a winning Mahjong hand by calling the
         * `is_winning_hand` method of the Hand class. If the player has a winning hand, a message
         * is displayed indicating the winning status, and the game is set to a non-running state.
         *
         */
        bool has_winning_hand() const
        {
            return hand.is_winning_hand();
        }

        std::tuple<int, int> get_player_score(bool full_hand = false) const
        {
            if (full_hand)
                return hand.get_max_score();
            else
                return hand.get_visible_score();
        }

        void display_player_score(bool full_hand = false, bool mahjong = false) const
        {
            std::tuple<int, int> score = get_player_score(full_hand);
            unsigned int unmodified_score = std::get<0>(score);
            unsigned int multiplier = std::get<1>(score);

            if (mahjong)
                unmodified_score += 20;

            std::cout << "Total score: " << unmodified_score * std::pow(2, multiplier);
            std::cout << " (" << unmodified_score << " doubled " << multiplier << " times)\n";
        }

        Mahjong::Hand get_visible_hand()
        {
            Mahjong::Hand visible_hand = Hand();
            for (Mahjong::Tile tile : hand.get_tiles())
            {
                if (!tile.is_hidden())
                    visible_hand.add_tile(tile);
            }
            return visible_hand;
        }

        Mahjong::Hand get_full_hand()
        {
            return hand;
        }
    };
} // namespace Mahjong

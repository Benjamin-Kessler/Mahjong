/**
 * @file Player.hpp
 * @brief Defines the Player class representing a player in a Mahjong game.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <map>
#include <random>

#include "Hand.hpp"
#include "Policy.hpp"
#include "Set.hpp"
#include "State.hpp"
#include "Discard_pile.hpp"
#include "Wind.hpp"

/** @brief Initial money for each player. */
float STARTING_MONEY = 100;

std::map<std::string, int> PICKUP_ACTION_TO_INT = {{"none", 0}, {"chow", 1}, {"pong", 2}, {"kong", 3}};
std::map<int, std::string> PICKUP_ACTION_TO_STRING = {{0, "none"}, {1, "chow"}, {2, "pong"}, {3, "kong"}};

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
        unsigned int player_number;                         /**< The unique identifier for the player. */
        bool is_human = false;                              /**< Flag indicating if the player is a human player. */
        Mahjong::Policy policy;                             /**< The AI policy dictating which actions to choose. */
        float money;                                        /**< The amount of money the player has. */
        Mahjong::Hand hand;                                 /**< The player's hand of tiles. */
        Mahjong::Wind seat_wind;                            /**< The player's current seat wind. */
        std::tuple<Mahjong::Tile, std::string> latest_tile; /**< The latest tile that was drawn and it's origin. */

    public:
        /**
         * @brief Constructor for Player class.
         * @param number The unique player number.
         * @param set Reference to the tile set for drawing initial tiles.
         */
        Player(unsigned int number, Mahjong::Set &set) : player_number(number), money(STARTING_MONEY), hand(), seat_wind(number)
        {
            hand.draw_hand(set);
            latest_tile = std::tuple(hand.get_tile_by_index(-1), "set");
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
            latest_tile = std::tuple(hand.get_tile_by_index(-1), "set");
        }

        /**
         * @brief Pick a tile from the discard pile and add it to the player's hand.
         * @param discard_pile Reference to the discard pile.
         */
        void pick_tile_from_discard(Discard_pile &discard_pile)
        {
            hand.pick_tile_from_discard(discard_pile);
            latest_tile = std::tuple(hand.get_tile_by_index(-1), "discard");
        }

        /**
         * @brief Discard a tile from the player's hand.
         * @param discard_pile Reference to the discard pile.
         */
        void discard_tile(Discard_pile &discard_pile, State game_state)
        {
            if (is_human)
            {
                hand.discard_tile(discard_pile);
            }
            else
            {
                // hand.discard_random_tile(discard_pile);
                std::vector<int> valid_discards = hand.get_valid_discards();
                int action = policy.select_action("Discard", valid_discards, game_state);
                // std::cout << policy.get_policy() << "\n";
                hand.discard_tile_by_index(discard_pile, action);
            }
        }

        /**
         * @brief Choose a pickup action based on the game state.
         * @param discard_pile Reference to the discard pile.
         * @param current_player The player who discarded the last tile.
         * @return The chosen action as a string.
         */
        std::string choose_pickup_action(Discard_pile &discard_pile, unsigned int current_player, Mahjong::State game_state)
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
                std::vector<int> available_actions_int = {};
                for (std::string action : available_actions)
                    available_actions_int.push_back(PICKUP_ACTION_TO_INT[action]);
                return PICKUP_ACTION_TO_STRING[policy.select_action("Pickup", available_actions_int, game_state)];
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

        /**
         * @brief Check if the player is human.
         * @return Boolean value indicating whether the player is human or not.
         */
        bool check_human()
        {
            return is_human;
        }

        /**
         * @brief Set the player's policy.
         * @param new_policy The new policy as string.
         */
        void set_policy(std::string new_policy)
        {
            policy.set_policy(new_policy);
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
         * @return Boolean indicating whether the player has a winning hand or not.
         */
        bool has_winning_hand() const
        {
            return hand.is_winning_hand();
        }

        /**
         * @brief Returns the Mahjong score for the player's hand.
         *
         * Returns the Mahjong score for the player's hand based on whether a full hand score is requested or not.
         *
         * @param round_wind The current round wind.
         * @param full_hand Flag indicating whether to calculate the score for the full hand or only the visible tiles. Default is false.
         * @param full_hand Flag indicating whether to calculate the score with potential mahjong bonuses or not. Default is false.
         * @return A tuple containing the Mahjong score and the corresponding multiplier for the given combination.
         */
        std::tuple<int, int> get_player_score(Mahjong::Wind round_wind, bool full_hand = false, bool mahjong = false) const
        {
            std::tuple<int, int> score;
            if (full_hand)
                score = hand.get_max_score(round_wind, seat_wind);
            else
                score = hand.get_visible_score(round_wind, seat_wind);

            if (mahjong)
            {
                // Bonus points for Mahjong
                std::get<0>(score) += 20;

                // Bonus points for completely hidden hand
                if (hand.get_hidden_hand().size() == hand.get_hand_size())
                {
                    std::get<0>(score) += 20;
                }

                // Bonus multiplier for special hands
                std::set<int> all_suits_set = hand.get_all_suits();
                std::vector<int> all_suits(all_suits_set.begin(), all_suits_set.end());

                if (all_suits.size() == 1)
                {
                    // Only one ground color.
                    if (all_suits[0] < 3)
                    {
                        std::get<1>(score) += 3;
                    }
                    // only dragons xor winds.
                    else
                    {
                        std::get<1>(score) += 4;
                    }
                }

                all_suits.erase(std::remove(all_suits.begin(), all_suits.end(), 3), all_suits.end());
                all_suits.erase(std::remove(all_suits.begin(), all_suits.end(), 4), all_suits.end());

                // Only one ground color and dragons/suits.
                if (all_suits.size() == 1)
                {
                    std::get<1>(score) += 2;
                }

                // Only ones or nines
                std::set<int> all_ranks_set = hand.get_all_ranks();
                std::vector<int> all_ranks(all_ranks_set.begin(), all_ranks_set.end());

                if (all_ranks.size() == 1)
                {
                    if ((all_ranks[0] == 0) || (all_ranks[0] == 8))
                    {
                        std::get<1>(score) += 4;
                    }
                }
            }

            return score;
        }

        /**
         * @brief Displays the player's Mahjong score.
         *
         * Displays the player's Mahjong score based on whether a full hand score is requested and whether Mahjong was declared.
         *
         * @param round_wind The current round wind.
         * @param full_hand Flag indicating whether to display the score for the full hand or only the visible tiles. Default is false.
         * @param mahjong Flag indicating whether Mahjong was declared. Default is false.
         */
        void display_player_score(Mahjong::Wind round_wind, bool full_hand = false, bool mahjong = false) const
        {
            std::tuple<int, int> score = get_player_score(round_wind, full_hand, mahjong);
            unsigned int unmodified_score = std::get<0>(score);
            unsigned int multiplier = std::get<1>(score);

            if (full_hand)
            {
                std::cout << "Total score: " << unmodified_score * std::pow(2, multiplier);
            }
            else
            {
                std::cout << "Known score: " << unmodified_score * std::pow(2, multiplier);
            }
            std::cout << " (" << unmodified_score << " doubled " << multiplier << " times)\n";
        }

        /**
         * @brief Returns the visible tiles in the player's hand.
         *
         * Constructs a new hand containing only the visible tiles from the player's hand.
         *
         * @return A Mahjong::Hand object representing the visible tiles in the player's hand.
         */
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

        /**
         * @brief Returns the full hand of the player.
         *
         * @return A Mahjong::Hand object representing the full hand of the player.
         */
        Mahjong::Hand get_full_hand()
        {
            return hand;
        }

        /**
         * @brief Get the seat wind of the player.
         *
         * @return The seat wind of the player.
         */
        Mahjong::Wind get_seat_wind() const
        {
            return seat_wind;
        }

        /**
         * @brief Rotate the seat wind of the player.
         */
        void rotate_seat_wind()
        {
            seat_wind.rotate_wind();
        }

        /**
         * @brief Get the player number.
         *
         *
         * @return The player number
         */
        unsigned int get_player_number()
        {
            return player_number;
        }
    };
} // namespace Mahjong

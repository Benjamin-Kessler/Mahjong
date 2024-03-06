#pragma once

#include <algorithm>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "State.hpp"

std::set<std::string> VALID_POLICIES = {"random", "human", "tile_count"};

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    /**
     * @brief Class representing a policy for decision-making in Mahjong game.
     */
    class Policy
    {
    private:
        std::string policy = "random"; ///< The current policy for decision-making.
        float random = 0.05;           ///< The randomness factor for decision-making.
        float chow_rate = 0.5;         ///< The rate for selecting Chow action.

    public:
        /**
         * @brief Default constructor for Policy class.
         */
        Policy(){};

        /**
         * @brief Sets the policy to "human".
         */
        void set_human()
        {
            policy = "human";
        }

        /**
         * @brief Sets the policy to the specified one if it's valid.
         *
         * @param new_policy The new policy to be set.
         */
        void set_policy(std::string new_policy)
        {
            if (VALID_POLICIES.find(new_policy) != VALID_POLICIES.end())
                policy = new_policy;
            else
                std::cout << "Invalid policy " << new_policy << "\n";
        }

        /**
         * @brief Returns the current policy.
         *
         * @return The current policy as a string.
         */
        std::string get_policy() const
        {
            return policy;
        }

        /**
         * @brief Sets the randomness factor for decision-making.
         *
         * @param new_random The new value for randomness factor.
         */
        void set_randomness(float new_random)
        {
            random = new_random;
        }

        /**
         * @brief Selects an action based on the policy, available actions, and game state.
         *
         * This method selects an action based on the current policy, available actions, and the game state.
         *
         * @param action_type The type of action to select.
         * @param available_actions A vector of available actions.
         * @param game_state The current game state.
         * @return The index of the selected action.
         */
        int select_action(std::string action_type, std::vector<int> available_actions, Mahjong::State game_state)
        {
            std::string decision_policy = policy;

            // Select a random action with predefined chance given by randomness
            int random_number = std::rand() % 100;

            if (random * 100 < random_number)
                decision_policy = "random";

            if (decision_policy == "random")
            {
                return available_actions[std::rand() % available_actions.size()];
            }

            if (decision_policy == "tile_count")
            {
                if (action_type == "Discard")
                {
                    unsigned int player_number = game_state.get_player_number();
                    Mahjong::Hand player_hand = game_state.get_player_hand(player_number);

                    int prefered_action;
                    int minimal_score = 5000;

                    for (int index : available_actions)
                    {
                        Mahjong::Tile tile = player_hand.get_tile_by_index(static_cast<unsigned int>(index));
                        int count_hand = player_hand.get_n_tile_occurence(tile);
                        int count_seen = game_state.get_n_tile_occurence(tile);
                        int suit = (tile.get_suit() < 3) ? 0 : 1;
                        int suit_count = player_hand.get_n_tiles_of_suit(tile.get_suit());

                        int score = 1000 * count_hand + 100 * (4 - count_seen) + 10 * suit + suit_count;

                        if (score < minimal_score)
                        {
                            prefered_action = index;
                            minimal_score = score;
                        }

                        if (score == minimal_score)
                            prefered_action = (std::rand() % 10 < 5) ? prefered_action : index;
                    }
                    // std::cout << "Minimal score " << minimal_score << " for " << player_hand.get_tile_by_index(prefered_action).get_tile_as_string() << "\n";
                    return prefered_action;
                }
                else if (action_type == "Pickup")
                {
                    int prefered_action = *std::max_element(available_actions.begin(), available_actions.end());
                    if (prefered_action == 1)
                        return ((std::rand() % 10) < (chow_rate * 10)) ? 0 : 1;
                    return prefered_action;
                }
            }

            return available_actions[std::rand() % available_actions.size()];
        }
    };
} // namespace Mahjong

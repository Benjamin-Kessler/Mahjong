#pragma once

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
    class Policy
    {
    private:
        std::string policy = "random";
        float random = 0.1;

    public:
        Policy(){};

        void set_human()
        {
            policy = "human";
        }

        void set_policy(std::string new_policy)
        {
            if (VALID_POLICIES.find(new_policy) != VALID_POLICIES.end())
                policy = new_policy;
        }

        void set_randomness(float new_random)
        {
            random = new_random;
        }

        template <typename Type>
        Type select_action(std::string action_type, std::vector<Type> available_actions, Mahjong::State game_state)
        {
            std::string decision_policy = policy;
            srand(time(0));

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
                    // for (typename std::vector<Type>::size_type index = 0; index < available_actions.size(); ++index)
                    // {
                    //     if (std::is_integral<Type>::value)
                    //     {
                    //         Mahjong::Tile tile = player_hand.get_tile_by_index(static_cast<unsigned int>(index));
                    //         int count_hand = player_hand.get_n_tile_occurence(tile);
                    //         int count_seen = game_state.get_n_tile_occurence(tile);
                    //     }
                    //     return available_actions[std::rand() % available_actions.size()];
                    // }
                    return available_actions[0];
                }
            }

            return available_actions[std::rand() % available_actions.size()];
        }
    };
} // namespace Mahjong

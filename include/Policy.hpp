#pragma once

#include <random>
#include <string>
#include <vector>

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

        void set_randomness(float new_random)
        {
            random = new_random;
        }

        template <typename Type>
        Type select_action(std::string action_type, std::vector<Type> available_actions)
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
            return available_actions[0];
        }
    };
} // namespace Mahjong

/**
 * @file Wind.hpp
 * @brief Defines the Wind class representing the seat winds in a Mahjong game.
 */

#pragma once
#include <string>

/** @brief String names for the ranks of the tiles with Wind suit. */
std::string WIND_STRING[4] = {"East", "South", "West", "North"};

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{

    /**
     * @class Wind
     * @brief Represents a (seat or round) wind in a Mahjong game.
     */
    class Wind
    {
    private:
        int wind; ///< The wind value.

    public:
        Wind(int wind_in) : wind(wind_in) {}

        /**
         * @brief Get the current wind value.
         * @return The rank of the tile.
         */
        int get_wind() const
        {
            return wind;
        }

        /**
         * @brief Get the string representation of the wind.
         * @return The string representation of the wind.
         */
        std::string get_wind_as_string() const
        {
            return WIND_STRING[wind];
        }

        /**
         * @brief Rotate the seat wind to the next direction.
         */
        void rotate_wind()
        {
            wind = (wind - 1) % 4;
        }
    };
} // namespace Mahjong
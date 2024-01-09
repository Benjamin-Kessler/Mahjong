/**
 * @file Set.hpp
 * @brief Defines the Set class representing the set of undrawn tiles in a Mahjong game.
 */

#pragma once
#include <algorithm>
#include <vector>

#include "Tile.hpp"

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{

    /**
     * @class Set
     * @brief Represents the set of undrawn tiles in a Mahjong game.
     *
     * The set is initialized with a standard collection of Mahjong tiles and provides
     * functionality to shuffle and pop tiles from the set.
     */
    class Set
    {
    private:
        std::vector<Tile> tiles; /**< The collection of tiles in the set. */

    public:
        /**
         * @brief Default constructor for Set.
         *
         * Initializes the set with a standard collection of Mahjong tiles.
         */
        Set()
        {
            tiles = {};
            for (size_t n = 0; n < 4; n++)
            {
                for (size_t i = 0; i < 5; i++)
                {
                    int max_rank;
                    if (i == 3)
                    {
                        max_rank = 4;
                    }
                    else if (i == 4)
                    {
                        max_rank = 3;
                    }
                    else
                    {
                        max_rank = 9;
                    }

                    for (size_t j = 0; j < max_rank; j++)
                    {
                        tiles.push_back(Tile(i, j));
                    }
                }
            }
        }

        /**
         * @brief Get the size of the set.
         * @return The number of tiles in the set.
         */
        int get_size() const
        {
            return tiles.size();
        }

        /**
         * @brief Shuffle the tiles in the set.
         *
         * Uses a random seed to shuffle the tiles using the Fisher-Yates shuffle algorithm.
         */
        void shuffle()
        {
            unsigned seed = time(0);
            std::shuffle(tiles.begin(), tiles.end(), std::default_random_engine(seed));
        }

        /**
         * @brief Pop a tile from the set.
         * @return The popped tile, or a default tile if the set is empty.
         */
        Tile pop_tile()
        {
            if (tiles.size() == 0)
            {
                return Tile(0, 0);
            }
            Tile tile_to_return = tiles.back();
            tiles.pop_back();
            return tile_to_return;
        }
    };

} // namespace Mahjong

/**
 * @file Discard_pile.hpp
 * @brief Defines the Discard_pile class representing the discard pile in a Mahjong game.
 */

#pragma once
#include <iostream>
#include <vector>

#include "Tile.hpp"

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    /**
     * @class Discard_pile
     * @brief Represents the discard pile in a Mahjong game.
     *
     * The discard pile is a collection of tiles that have been discarded by players
     * during the course of the game. It provides functionality to add, display, and
     * pop tiles from the pile.
     */
    class Discard_pile
    {
    private:
        std::vector<Mahjong::Tile> tiles; /**< The collection of discarded tiles in the pile. */

    public:
        /**
         * @brief Default constructor for Discard_pile.
         * Initializes the discard pile with an empty collection of tiles.
         */
        Discard_pile() : tiles(){};

        /**
         * @brief Add a discarded tile to the discard pile.
         * @param tile The tile to be added to the discard pile.
         */
        void add_discarded_tile(Mahjong::Tile tile)
        {
            tiles.push_back(tile);
        }

        /**
         * @brief Display the contents of the discard pile.
         * Outputs the tiles in the discard pile to the console.
         */
        void display_discard_pile() const
        {
            unsigned int pile_size = tiles.size();
            for (size_t i = 0; i < pile_size - 1; i++)
            {
                std::cout << tiles[i].get_tile_as_string() << " -- ";
            }

            std::cout << tiles[pile_size - 1].get_tile_as_string() << std::endl;
        }

        /**
         * @brief Pop a tile from the discard pile.
         * @return The popped tile, or a default tile if the pile is empty.
         */
        Mahjong::Tile pop_tile()
        {
            if (tiles.size() == 0)
            {
                return Mahjong::Tile(0, 0);
            }
            Mahjong::Tile tile_to_return = tiles.back();
            tiles.pop_back();
            return tile_to_return;
        }

        /**
         * @brief Get the tile at the top of the discard pile.
         * @return The tile at the top of the discard pile.
         */
        Mahjong::Tile back() const
        {
            return tiles.back();
        }

        /**
         * @brief Get the size of the discard pile.
         * @return The number of tiles in the discard pile.
         */
        unsigned int get_size() const
        {
            return tiles.size();
        }
    };
} // namespace Mahjong
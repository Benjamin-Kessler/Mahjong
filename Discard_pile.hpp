#pragma once
#include <iostream>
#include <vector>

#include "Tile.hpp"

class Discard_pile
{
private:
    std::vector<Tile> tiles;

public:
    Discard_pile() : tiles()
    {
    }

    void add_discarded_tile(Tile tile)
    {
        Discard_pile::tiles.push_back(tile);
    }

    void display_discard_pile()
    {
        unsigned int pile_size = tiles.size();
        for (size_t i = 0; i < pile_size - 1; i++)
        {
            std::cout << tiles[i].get_tile_as_string() << " -- ";
        }

        std::cout << tiles[pile_size - 1].get_tile_as_string() << std::endl;
    }
};
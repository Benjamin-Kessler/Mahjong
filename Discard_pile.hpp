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

    void display_discard_pile() const
    {
        unsigned int pile_size = Discard_pile::tiles.size();
        for (size_t i = 0; i < pile_size - 1; i++)
        {
            std::cout << Discard_pile::tiles[i].get_tile_as_string() << " -- ";
        }

        std::cout << Discard_pile::tiles[pile_size - 1].get_tile_as_string() << std::endl;
    }

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

    Tile back() const
    {
        return tiles.back();
    }

    unsigned int get_size() const
    {
        return tiles.size();
    }
};
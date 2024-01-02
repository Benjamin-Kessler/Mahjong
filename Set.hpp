#pragma once
#include <algorithm>
#include <vector>
// #include <bits/stdc++.h>

#include "Tile.hpp"

class Set
{
private:
    std::vector<Tile> tiles;

public:
    Set()
    // Function that creates an entire set of tiles.
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

    int get_size() const
    {
        return tiles.size();
    }

    void shuffle()
    {
        unsigned seed = time(0);
        std::shuffle(tiles.begin(), tiles.end(), std::default_random_engine(seed));
    }

    // Tile get_tile()
    // {
    //     if (tiles.size() == 0)
    //     {
    //         return Tile(0, 0);
    //     }

    //     return tiles.back();
    // }

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

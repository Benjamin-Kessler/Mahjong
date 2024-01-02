#pragma once
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <random>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Discard_pile.hpp"

const unsigned int HAND_SIZE = 13;

class Hand
{
private:
    std::vector<Tile> tiles;

public:
    Hand()
    {
        Hand::tiles = {};
    }

    void draw_hand(Set &set)
    {
        assert(tiles.size() == 0);
        for (size_t i = 0; i < HAND_SIZE; i++)
        {
            Hand::tiles.push_back(set.pop_tile());
        }
    }

    void draw_tile(Set &set, bool broadcast)
    {
        if (Hand::tiles.size() == HAND_SIZE)
        {
            Hand::tiles.push_back(set.pop_tile());
            if (broadcast)
            {
                std::cout << "Draw tile: " << tiles.back().get_tile_as_string() << std::endl;
            }
        }
        else
        {
            if (broadcast)
            {
                std::cout << "Too many tiles in hand. Discard tiles first." << std::endl;
            }
        }
    }

    void pick_tile_from_discard(Discard_pile &discard_pile, bool broadcast)
    {
        if (Hand::tiles.size() == HAND_SIZE)
        {
            Hand::tiles.push_back(discard_pile.pop_tile());
            if (broadcast)
            {
                std::cout << "Draw tile: " << tiles.back().get_tile_as_string() << std::endl;
            }
        }
        else
        {
            if (broadcast)
            {
                std::cout << "Too many tiles in hand. Discard tiles first." << std::endl;
            }
        }
    }

    void discard_tile(Discard_pile &discard_pile)
    {
        if (Hand::tiles.size() == HAND_SIZE + 1)
        {
            int to_discard;
            std::cout << "Select which tile to discard:" << std::endl;
            std::cin >> to_discard;
            if (0 <= to_discard && to_discard < HAND_SIZE + 1)
            {
                std::cout << "Discard " << Hand::tiles[to_discard].get_tile_as_string() << std::endl;
                discard_pile.add_discarded_tile(tiles[to_discard]);
                Hand::tiles.erase(Hand::tiles.begin() + to_discard);
            }
            else
            {
                std::cout << "Invalid number. Choice must be between 0 and 13.";
            }
        }
        else
        {
            std::cout << "Not enough tiles in hand. Draw tiles first." << std::endl;
        }
    }

    void discard_random_tile(Discard_pile &discard_pile)
    {
        assert(tiles.size() == HAND_SIZE + 1);
        unsigned seed = time(0);
        srand(seed);
        int to_discard = std::rand() % (HAND_SIZE + 1);
        std::cout << "Discard " << tiles[to_discard].get_tile_as_string() << std::endl;
        discard_pile.add_discarded_tile(tiles[to_discard]);
        tiles.erase(tiles.begin() + to_discard);
    }

    int get_hand_size()
    {
        return tiles.size();
    }

    void display_hand()
    {
        for (size_t i = 0; i < tiles.size(); i++)
        {
            std::cout << i << ": " << tiles[i].get_tile_as_string() << std::endl;
        }
    }

    void display_visible_hand()
    {
        unsigned int hand_size = tiles.size();
        for (size_t i = 0; i < hand_size; i++)
        {
            if (tiles[i].is_hidden())
            {
                continue;
            }
            std::cout << tiles[i].get_tile_as_string() << std::endl;
        }
    }

    void sort()
    {
        std::sort(tiles.begin(), tiles.end(), [](Tile &a, Tile &b)
                  {
                      if (a.get_suit() != b.get_suit())
                      {
                          return a.get_suit() < b.get_suit();
                      }
                      else
                      {
                          return a.get_rank() < b.get_rank();
                      } });
    }

    bool is_winning_hand()
    {
        if (Hand::tiles.size() != 14)
        {
            return false;
        }
        return false;
    }

    bool check_kong(const Tile &tile) const
    {
        return std::count(Hand::tiles.begin(), Hand::tiles.end(), tile) == 4;
    }

    bool check_pong(const Tile &tile) const
    {
        return std::count(Hand::tiles.begin(), Hand::tiles.end(), tile) == 3;
    }

    bool check_chow(const Tile &tile) const
    {
        int suitCount = 0;

        // Sort the tiles by rank to facilitate finding sequences
        std::vector<Tile> sortedTiles = tiles;
        std::sort(sortedTiles.begin(), sortedTiles.end(), [](Tile &a, Tile &b)
                  {
                      if (a.get_suit() != b.get_suit())
                      {
                          return a.get_suit() < b.get_suit();
                      }
                      else
                      {
                          return a.get_rank() < b.get_rank();
                      } });

        for (size_t i = 0; i < sortedTiles.size(); ++i)
        {
            if (sortedTiles[i].get_suit() == tile.get_suit())
            {
                suitCount = 1;

                for (size_t j = i + 1; j < sortedTiles.size(); ++j)
                {
                    if (sortedTiles[j].get_suit() != tile.get_suit())
                    {
                        break;
                    }

                    if (sortedTiles[j].get_rank() == sortedTiles[i].get_rank() + 1)
                    {
                        ++suitCount;

                        if (suitCount == 3)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        return false;
    }
};

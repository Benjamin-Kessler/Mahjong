#pragma once
#include <vector>
#include <iostream>
#include <assert.h>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Discard_pile.hpp"

int HAND_SIZE = 13;

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

    void draw_tile(Set &set)
    {
        if (Hand::tiles.size() == HAND_SIZE)
        {
            Hand::tiles.push_back(set.pop_tile());
            std::cout << tiles.back().get_tile_as_string() << std::endl;
        }
        else
        {
            std::cout << "Too many tiles in hand. Discard tiles first." << std::endl;
        }
    }

    void discard_tile(Discard_pile &discard_pile)
    {
        if (Hand::tiles.size() == HAND_SIZE + 1)
        {
            int to_discard;
            std::cout << "Select which tile to discard:" << std::endl;
            std::cin >> to_discard;
            if (0 <= to_discard < HAND_SIZE + 1)
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

    int get_hand_size()
    {
        return Hand::tiles.size();
    }

    void display_hand()
    {
        unsigned int hand_size = tiles.size();
        for (size_t i = 0; i < hand_size; i++)
        {
            std::cout << i << ": " << tiles[i].get_tile_as_string() << std::endl;
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
        return false;
    }
};

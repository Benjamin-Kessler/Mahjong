/**
 * @file Hand.hpp
 * @brief Defines the Hand class representing a player's hand in a Mahjong game.
 */

#pragma once
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <random>
#include <map>
#include <string>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Discard_pile.hpp"

const unsigned int HAND_SIZE = 13;

/**
 * @brief The `Hand` class represents a player's hand in a Mahjong game.
 *
 * This class manages the tiles in a player's hand, allowing various operations
 * such as drawing tiles, discarding tiles, and displaying the hand.
 */
class Hand
{
private:
    std::vector<Tile> tiles; /**< The tiles currently in hand. */

public:
    /**
     * @brief Default constructor for the `Hand` class.
     *
     * Initializes the `tiles` vector to an empty state.
     */
    Hand()
    {
        Hand::tiles = {};
    }

    /**
     * @brief Draws a complete hand from the given tile set.
     *
     * @param set Reference to the game's tile set.
     */
    void draw_hand(Set &set)
    {
        assert(tiles.size() == 0);
        for (size_t i = 0; i < HAND_SIZE; i++)
        {
            Hand::tiles.push_back(set.pop_tile());
        }
    }

    /**
     * @brief Draws a single tile from the set and adds it to the hand.
     *
     * @param set Reference to the game's tile set.
     * @param broadcast If true, displays the drawn tile.
     */
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

    /**
     * @brief Picks a tile from the discard pile and adds it to the hand.
     *
     * @param discard_pile Reference to the game's discard pile.
     */
    void pick_tile_from_discard(Discard_pile &discard_pile)
    {
        if (Hand::tiles.size() == HAND_SIZE)
        {
            Hand::tiles.push_back(discard_pile.pop_tile());
        }
        else
        {
            std::cout << "Too many tiles in hand. Discard tiles first." << std::endl;
        }
    }

    /**
     * @brief Discards a tile from the hand and adds it to the discard pile.
     *
     * The user is prompted to select a tile to discard.
     *
     * @param discard_pile Reference to the game's discard pile.
     */
    void discard_tile(Discard_pile &discard_pile)
    {
        if (Hand::tiles.size() == HAND_SIZE + 1)
        {
            int to_discard;
            bool valid_discard_tile = false;

            while (!valid_discard_tile)
            {
                std::cout << "Select which tile to discard:" << std::endl;
                std::cin >> to_discard;
                if (0 <= to_discard && to_discard < HAND_SIZE + 1)
                {
                    if (Hand::tiles[to_discard].is_hidden())
                    {
                        std::cout << "Discard " << Hand::tiles[to_discard].get_tile_as_string() << std::endl;
                        discard_pile.add_discarded_tile(tiles[to_discard]);
                        Hand::tiles.erase(Hand::tiles.begin() + to_discard);
                        valid_discard_tile = true;
                    }
                    else
                    {
                        std::cout << "Chosen tile must be hidden." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid number. Choice must be between 0 and 13." << std::endl;
                }
            }
        }
        else
        {
            std::cout << "Not enough tiles in hand. Draw tiles first." << std::endl;
        }
    }

    /**
     * @brief Discards a randomly selected hidden tile from the hand.
     *
     * This function randomly selects a hidden tile from the player's hand, discards it,
     * and adds it to the specified discard pile. The discarded tile is removed from the hand.
     *
     * @param discard_pile Reference to the game's discard pile.
     *
     * @note This function assumes that the hand has the required number of tiles (HAND_SIZE + 1)
     * and that there is at least one hidden tile to discard. It uses the rand() function with a time-based seed.
     */
    void discard_random_tile(Discard_pile &discard_pile)
    {
        assert(tiles.size() == HAND_SIZE + 1);
        unsigned seed = time(0);
        srand(seed);
        int to_discard;
        bool valid_discard = false;
        while (!valid_discard)
        {
            to_discard = std::rand() % (HAND_SIZE + 1);
            valid_discard = tiles[to_discard].is_hidden();
        }
        std::cout << "Discard " << tiles[to_discard].get_tile_as_string() << std::endl;
        discard_pile.add_discarded_tile(tiles[to_discard]);
        tiles.erase(tiles.begin() + to_discard);
    }

    /**
     * @brief Gets the current size of the hand.
     *
     * @return The number of tiles in the hand.
     */
    int get_hand_size()
    {
        return tiles.size();
    }

    /**
     * @brief Displays the entire hand with indices.
     *
     * Each tile is displayed with its index in the hand.
     */
    void display_hand()
    {
        for (size_t i = 0; i < tiles.size(); i++)
        {
            std::cout << i << ": " << tiles[i].get_tile_as_string_with_visibility() << std::endl;
        }
    }

    /**
     * @brief Displays only the visible (non-hidden) tiles in the hand.
     *
     * Tiles marked as hidden are excluded from the display.
     */
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

    /**
     * @brief Sorts the tiles in the hand based on suit and rank.
     *
     * Uses a lambda function as a sorting criterion.
     */
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

    /**
     * @brief Checks if the current hand is a winning one, i.e., if mahjong can be called.
     * @return True if the current hand is a winning hand, false otherwise.
     */
    bool is_winning_hand()
    {
        if (Hand::tiles.size() != 14)
        {
            return false;
        }
        if (get_hidden_hand().size() == 0)
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Retrieve hidden tiles from the hand.
     *
     * This function iterates through the tiles in the hand and collects the ones
     * that are marked as hidden. It returns a vector containing all hidden tiles.
     *
     * @note This function assumes the existence of a class 'Tile' and a class 'Hand'
     *       with a static member 'tiles' representing the tiles in the hand.
     *
     * @return A vector of Tile objects representing the hidden tiles in the hand.
     */
    std::vector<Tile> get_hidden_hand() const
    {
        std::vector<Tile> hidden_hand = {};
        for (size_t i = 0; i < Hand::tiles.size(); i++)
        {
            if (Hand::tiles[i].is_hidden())
            {
                hidden_hand.push_back(Hand::tiles[i]);
            }
        }
        return hidden_hand;
    }

    /**
     * @brief Reveals the correspondinig tiles after a pick up is performed.
     *
     * @param tile An instance of class Tile representing the tile that was picked up.
     * @param action A string parameter to indicate which pick up action was performed (i.e. kong, pong or chow).
     */
    void reveal_combination(Tile tile, std::string action)
    {
        if (action == "kong")
        {
            auto it = tiles.begin();
            while (it != tiles.end())
            {
                it = std::find(it, tiles.end(), tile);
                auto pos = std::distance(tiles.begin(), it);
                Tile &tile = tiles[pos];
                tile.set_visible();
            }
        }
        else if (action == "pong")
        {
            auto it = tiles.begin();
            unsigned int n_matches = 0;
            while (it != tiles.end() && n_matches < 3)
            {
                it = std::find(it, tiles.end(), tile);
                auto pos = std::distance(tiles.begin(), it);
                Tile &tile = tiles[pos];
                tile.set_visible();
                n_matches += 1;
            }
        }
    }

    /**
     * @brief Check if a given tile forms a kong in the hidden hand.
     *
     * This function checks whether a specified tile forms a kong in the hidden hand.
     * A kong is a set of four identical tiles in a player's concealed (hidden) hand.
     *
     * @param tile The tile to be checked for kong formation.
     *
     * @return True if the provided tile forms a kong in the hidden hand, false otherwise.
     */
    bool check_kong(const Tile tile) const
    {
        std::vector<Tile> hidden_hand = get_hidden_hand();
        return std::count(hidden_hand.begin(), hidden_hand.end(), tile) == 3;
    }

    /**
     * @brief Check if a given tile forms a pong in the hidden hand.
     *
     * This function checks whether a specified tile forms a pong in the hidden hand.
     * A pong is a set of three identical tiles in a player's concealed (hidden) hand.
     *
     * @param tile The tile to be checked for pong formation.
     *
     * @return True if the provided tile forms a pong in the hidden hand, false otherwise.
     */
    bool check_pong(const Tile tile) const
    {
        std::vector<Tile> hidden_hand = get_hidden_hand();
        return std::count(hidden_hand.begin(), hidden_hand.end(), tile) == 2;
    }

    /**
     * @brief Check whether a given tile, in combination with the current tiles in hand,
     * would form a chow, i.e., three consecutive numbered tiles from the suits bamboos,
     * circles, or characters.
     *
     * This function checks if adding the specified tile to the current hidden hand could
     * form a chow (three consecutive numbered tiles of the same suit). The tiles are
     * sorted by suit and rank to facilitate finding sequences.
     *
     * @param tile The Tile object representing the tile to check for chow formation.
     * @return True if adding the specified tile would form a chow, false otherwise.
     *
     * @note Dragon and Wind tiles cannot be part of a chow.
     */
    bool check_chow(const Tile tile) const
    {
        // Skip if suit equals Dragons or Winds as corresponding chows are not allowed.
        if (tile.get_suit() == 3 || tile.get_suit() == 4)
        {
            return false;
        }

        int suitCount = 0;
        // Sort the tiles by rank to facilitate finding sequences
        std::vector<Tile> sortedTiles = get_hidden_hand();
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

    /**
     * @brief Check a given player's available pickup actions based on the latest discard tile.
     *
     * This function checks the available pickup actions for a specified player, considering
     * the tiles currently in hand and the latest discard tile.
     *
     * @param discard_pile Reference variable of the game's discard pile.
     * @param player_number Integer referring to the player performing the pickup.
     * @param current_player Integer referring to the current player, i.e., the player who discarded the last tile.
     *
     * @return Vector containing all available actions as strings.
     *
     * @note The available actions include "kong" if a kong is possible, "pong" if a pong is possible,
     * and "chow" if a chow is possible and the pickup is performed by the next player in turn.
     */
    std::vector<std::string> check_available_actions(const Discard_pile &discard_pile, unsigned int player_number, int current_player) const
    {
        std::vector<std::string> available_actions = {};
        Tile tile = discard_pile.back();
        if (check_kong(tile))
        {
            available_actions.push_back("kong");
        }
        else if (check_pong(tile))
        {
            available_actions.push_back("pong");
        }
        else if (check_chow(tile) && (player_number == ((current_player + 1) % 4)))

        {
            available_actions.push_back("chow");
        }
        return available_actions;
    }
};

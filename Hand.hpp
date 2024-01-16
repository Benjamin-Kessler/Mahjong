/**
 * @file Hand.hpp
 * @brief Defines the Hand class representing a player's hand in a Mahjong game.
 */

#pragma once
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <set>
#include <random>
#include <map>
#include <string>

#include "Tile.hpp"
#include "Set.hpp"
#include "Game.hpp"
#include "Discard_pile.hpp"
#include "dlx_exact_cover_solver.hpp"

/** @brief Initial number of tiles in hand. */
const unsigned int HAND_SIZE = 13;

/** @brief Special combinations of tiles in hand. */
// const std::vector<std::set<Mahjong::Tile>> SPECIAL_COMBINATIONS = {
//     {Mahjong::Tile(3, 0), Mahjong::Tile(3, 1), Mahjong::Tile(3, 2), Mahjong::Tile(3, 3)},
//     {Mahjong::Tile(4, 0), Mahjong::Tile(4, 1), Mahjong::Tile(4, 2)}};

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    /**
     * @brief The `Hand` class represents a player's hand in a Mahjong game.
     *
     * This class manages the tiles in a player's hand, allowing various operations
     * such as drawing tiles, discarding tiles, and displaying the hand.
     */
    class Hand
    {
    private:
        std::vector<Mahjong::Tile> tiles; /**< The tiles currently in hand. */

    public:
        /**
         * @brief Default constructor for the `Hand` class.
         *
         * Initializes the `tiles` vector to an empty state.
         */
        Hand() : tiles() {}

        Hand(std::vector<std::pair<unsigned int, unsigned int>> input_tiles)
        {
            tiles = {};
            for (auto tile : input_tiles)
            {
                tiles.push_back(Mahjong::Tile(tile.first, tile.second));
            }
        }

        /**
         * @brief Draws a complete hand from the given tile set.
         *
         * @param set Reference to the game's tile set.
         */
        void draw_hand(Mahjong::Set &set)
        {
            assert(tiles.size() == 0);
            for (size_t i = 0; i < HAND_SIZE; i++)
            {
                tiles.push_back(set.pop_tile());
            }
        }

        /**
         * @brief Draws a single tile from the set and adds it to the hand.
         *
         * @param set Reference to the game's tile set.
         * @param broadcast If true, displays the drawn tile.
         */
        void draw_tile(Mahjong::Set &set, bool broadcast)
        {
            if (tiles.size() == HAND_SIZE)
            {
                tiles.push_back(set.pop_tile());
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
            if (tiles.size() == HAND_SIZE)
            {
                tiles.push_back(discard_pile.pop_tile());
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
            if (tiles.size() == HAND_SIZE + 1)
            {
                int to_discard;
                bool valid_discard_tile = false;

                while (!valid_discard_tile)
                {
                    std::cout << "Select which tile to discard:" << std::endl;
                    std::cin >> to_discard;
                    if (0 <= to_discard && to_discard < HAND_SIZE + 1)
                    {
                        if (tiles[to_discard].is_hidden())
                        {
                            std::cout << "Discard " << tiles[to_discard].get_tile_as_string() << std::endl;
                            discard_pile.add_discarded_tile(tiles[to_discard]);
                            tiles.erase(tiles.begin() + to_discard);
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
         */
        void discard_random_tile(Discard_pile &discard_pile)
        {
            srand(time(0));
            unsigned int to_discard;
            bool valid_discard = false;
            if (get_hidden_hand().size() == 0)
            {
                return;
            }
            while (valid_discard == false)
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
            std::cout << "Known tiles: " << std::endl;
            for (size_t i = 0; i < hand_size; i++)
            {
                if (tiles[i].is_hidden())
                {
                    continue;
                }

                std::cout << tiles[i].get_tile_as_string() << "  ";
            }
            std::cout << std::endl;
        }

        /**
         * @brief Sorts the tiles in the hand based on suit and rank.
         *
         * Uses a lambda function as a sorting criterion.
         */
        void sort()
        {
            std::sort(tiles.begin(), tiles.end(), [](Mahjong::Tile &a, Mahjong::Tile &b)
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
         * @brief Prints a vector of sets of integers (usually combinations or covers) to the terminal.
         * @note Mostly for debugging purposes
         */
        void print_combinations(std::vector<std::set<int>> combinations)
        {
            unsigned int index = 0;
            for (auto set : combinations)
            {
                std::cout << index << ": ";
                for (int value : set)
                    std::cout << value << " ";
                std::cout << "\n";
                index++;
            }
        }

        /**
         * @brief Check if the current hand is a winning Mahjong hand.
         *
         * This function determines if the given set of tiles forms a winning Mahjong hand.
         * The hand must consist of 14 tiles, contain at least one pair, and have enough combinations
         * to form an exact cover using Knuth's algorithm X. A winning hand is defined as an exact cover
         * of 5 combinations, with at least one of the combinations being a pair.
         *
         * @return True if the hand is a winning Mahjong hand, false otherwise.
         */
        bool is_winning_hand()
        {
            // std::cout << "Checking winning hand...\n";

            // Early abort of computations if not enough tiles in hand.
            if (tiles.size() != 14)
            {
                // std::cout << "Not enough tiles in hand.\n";
                return false;
            }

            // Early abort of computations if no pair in hand.
            if (get_pairs().size() == 0)
            {
                // std::cout << "No pair in hand.\n";
                return false;
            }

            // std::cout << "Computing combinations...\n";
            std::vector<std::set<int>> combinations = get_combinations();

            // Early abort of computations if not enough combinations on hand to win.
            if (combinations.size() < 5)
            {
                // std::cout << "Not enough combinations in hand.\n";
                return false;
            }

            // Early abort of computations if not all tiles are in at least one combination.
            std::set<int> used_tiles;
            for (std::set<int> combination : combinations)
            {
                used_tiles.insert(combination.begin(), combination.end());
            }
            if (used_tiles.size() != HAND_SIZE + 1)
            {
                // std::cout << "Not all tiles included in at least one combination.\n";
                return false;
            }

            // Find all exact covers (i.e. a set of combinations such that each tile is in exactly one combination).
            std::cout << "Computing covers...\n";
            DLX::exact_cover_solver ecs = DLX::exact_cover_solver();
            std::vector<std::set<int>> covers = ecs.find_exact_covers(combinations);
            print_combinations(covers);

            // Check if there is a winning cover, i.e. an exact cover of 5 combinations and containing at least one pair.
            for (std::set<int> cover : covers)
            {
                if (cover.size() == 5)
                {
                    for (int index : cover)
                    {
                        if (combinations[index].size() == 2)
                            return true;
                    }
                }
            }
            // std::cout << "Not the right covers in hand.\n";
            return false;
        }

        /**
         * @brief Retrieve hidden tiles from the hand.
         *
         * This function iterates through the tiles in the hand and collects the ones
         * that are marked as hidden. It returns a vector containing all hidden tiles.
         *
         * @return A vector of Tile objects representing the hidden tiles in the hand.
         */
        std::vector<Mahjong::Tile> get_hidden_hand() const
        {
            std::vector<Mahjong::Tile> hidden_hand = {};
            for (size_t i = 0; i < tiles.size(); i++)
            {
                if (tiles[i].is_hidden())
                {
                    hidden_hand.push_back(tiles[i]);
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
        void reveal_combination(Mahjong::Tile tile, std::string action)
        {
            if (action == "kong")
            {
                auto it = tiles.begin();
                while (it != tiles.end())
                {
                    it = std::find(it, tiles.end(), tile);
                    auto pos = std::distance(tiles.begin(), it);
                    Mahjong::Tile &tile = tiles[pos];
                    tile.set_visible();
                    it++;
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
                    Mahjong::Tile &tile = tiles[pos];
                    tile.set_visible();
                    n_matches += 1;
                    it++;
                }
            }
        }

        /**
         * @brief Find all index positions of pairs currently in hand.
         * @return Vector of sets of indices representing the positions of two identical tiles.
         */
        std::vector<std::set<int>> get_pairs()
        {
            std::vector<std::set<int>> pairs;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    // Check if the tiles are identical
                    if (tiles[i] == tiles[j])
                    {
                        std::set<int> pair = {i, j};
                        pairs.push_back(pair);
                    }
                }
            }

            return pairs;
        }

        /**
         * @brief Find all index positions of chows (three tiles of suit bamboo, character or circles of increasing ranks)
         * currently in hand.
         * @return Vector of sets of indices representing the positions of three tiles forming a chow.
         */
        std::vector<std::set<int>> get_chows()
        {
            std::vector<std::set<int>> chows;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                if (tiles[i].get_suit() == 3 || tiles[i].get_suit() == 3)
                    continue;
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    if (tiles[i].get_suit() != tiles[j].get_suit())
                        continue;
                    for (int k = j + 1; k < tiles.size(); k++)
                    {
                        if (tiles[j].get_suit() != tiles[k].get_suit())
                            continue;
                        // Check if the tiles are identical
                        int arr[] = {tiles[i].get_rank(), tiles[j].get_rank(), tiles[k].get_rank()};
                        std::sort(arr, arr + 3);
                        if (arr[1] - arr[0] == 1 && arr[2] - arr[1] == 1)
                        {
                            std::set<int> pong = {i, j, k};
                            chows.push_back(pong);
                        }
                    }
                }
            }

            return chows;
        }

        /**
         * @brief Find all index positions of pongs (three of a kind) currently in hand.
         * @return Vector of sets of indices representing the positions of three identical tiles.
         */
        std::vector<std::set<int>> get_pongs()
        {
            std::vector<std::set<int>> pongs;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    for (int k = j + 1; k < tiles.size(); k++)
                    {
                        // Check if the tiles are identical
                        if (tiles[i] == tiles[j] && tiles[j] == tiles[k])
                        {
                            std::set<int> pong = {i, j, k};
                            pongs.push_back(pong);
                        }
                    }
                }
            }

            return pongs;
        }

        /**
         * @brief Find all index positions of kongs (four of a kind) currently in hand.
         * @return Vector of sets of indices representing the positions of four identical tiles.
         */
        std::vector<std::set<int>> get_kongs()
        {
            std::vector<std::set<int>> kongs;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    for (int k = j + 1; k < tiles.size(); k++)
                    {
                        for (int l = k + 1; l < tiles.size(); l++)
                        {
                            // Check if the tiles are identical
                            if (tiles[i] == tiles[j] && tiles[j] == tiles[k] && tiles[k] == tiles[l])
                            {
                                std::set<int> kong = {i, j, k, l};
                                kongs.push_back(kong);
                            }
                        }
                    }
                }
            }

            return kongs;
        }

        /**
         * @brief Get all possible combinations of sets of tiles, including pairs, chows, pongs, and kongs.
         *
         * This function retrieves sets of tiles representing pairs, chows, pongs, and kongs,
         * and combines them into a vector of sets representing all possible combinations.
         *
         * @return A vector of sets of integers, where each set represents a combination of tiles.
         *         The vector includes sets for pairs, chows, pongs, and kongs.
         *         Each set contains integers representing the unique identifiers of tiles in the combination.
         *         The order of sets in the vector is pairs, chows, pongs, and kongs.
         */
        std::vector<std::set<int>> get_combinations()
        {

            // Retrieve sets of pairs, chows, pongs, and kongs
            std::vector<std::set<int>> pairs = get_pairs();
            std::vector<std::set<int>> chows = get_chows();
            std::vector<std::set<int>> pongs = get_pongs();
            std::vector<std::set<int>> kongs = get_kongs();

            // Combine sets into a vector of all possible combinations
            std::vector<std::set<int>> combinations(pairs.begin(), pairs.end());
            combinations.insert(combinations.end(), chows.begin(), chows.end());
            combinations.insert(combinations.end(), pongs.begin(), pongs.end());
            combinations.insert(combinations.end(), kongs.begin(), kongs.end());

            return combinations;
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
        bool check_kong(const Mahjong::Tile tile) const
        {
            std::vector<Mahjong::Tile> hidden_hand = get_hidden_hand();
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
        bool check_pong(const Mahjong::Tile tile) const
        {
            std::vector<Mahjong::Tile> hidden_hand = get_hidden_hand();
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
        bool check_chow(const Mahjong::Tile tile) const
        {
            // Skip if suit equals Dragons or Winds as corresponding chows are not allowed.
            if (tile.get_suit() == 3 || tile.get_suit() == 4)
            {
                return false;
            }

            int suitCount = 0;
            // Sort the tiles by rank to facilitate finding sequences
            std::vector<Mahjong::Tile> sortedTiles = get_hidden_hand();
            std::sort(sortedTiles.begin(), sortedTiles.end(), [](Mahjong::Tile &a, Mahjong::Tile &b)
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
            Mahjong::Tile tile = discard_pile.back();
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
} // namespace Mahjong

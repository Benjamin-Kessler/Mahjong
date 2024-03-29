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
#include <tuple>

#include "Tile.hpp"
#include "Set.hpp"
#include "Discard_pile.hpp"
#include "dlx_exact_cover_solver.hpp"
#include "score_table.hpp"
#include "Wind.hpp"

/** @brief Initial number of tiles in hand. */
const unsigned int HAND_SIZE = 13;

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
         * @brief Adds a given tile to the hand.
         *
         * @param tile Tile to be added.
         */
        void add_tile(Mahjong::Tile tile)
        {
            tiles.push_back(tile);
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
         * @brief Discards a tile from the player's hand by its index.
         *
         * This function removes the tile at the specified index from the player's hand,
         * adds it to the discard pile, and prints a message indicating the discarded tile.
         *
         * @param discard_pile A reference to the discard pile object.
         * @param index The index of the tile to be discarded from the player's hand.
         */
        void discard_tile_by_index(Discard_pile &discard_pile, int index)
        {
            std::cout << "Discard " << tiles[index].get_tile_as_string() << std::endl;
            discard_pile.add_discarded_tile(tiles[index]);
            tiles.erase(tiles.begin() + index);
        }

        /**
         * @brief Returns indices of valid tiles that can be discarded.
         *
         * This function scans the player's hand for hidden tiles and returns a vector
         * containing the indices of those tiles. These indices represent the valid tiles
         * that can be discarded during gameplay.
         *
         * @return A vector of integers representing the indices of valid tiles for discarding.
         */
        std::vector<int> get_valid_discards()
        {
            std::vector<int> valid_discards = {};
            for (int index = 0; index < tiles.size(); index++)
            {
                if (tiles[index].is_hidden())
                    valid_discards.push_back(index);
            }
            return valid_discards;
        }

        /**
         * @brief Gets the current size of the hand.
         *
         * @return The number of tiles in the hand.
         */
        int get_hand_size() const
        {
            return tiles.size();
        }

        /**
         * @brief Displays the entire hand with indices.
         *
         * Each tile is displayed with its index in the hand.
         */
        void display_hand() const
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
        void display_visible_hand() const
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
        void print_combinations(std::vector<std::set<int>> combinations) const
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
        bool is_winning_hand() const
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
        void reveal_combination(Mahjong::Tile tile, std::string action, bool is_human)
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
            else if (action == "chow")
            {
                std::vector<int> relevant_indices = {};
                unsigned int relevant_suit = tile.get_suit();

                // Get all indexes of relevant tiles
                for (int index = 0; index < tiles.size(); index++)
                {
                    Mahjong::Tile hand_tile = tiles[index];
                    if (!hand_tile.is_hidden())
                        continue;
                    if (relevant_suit != hand_tile.get_suit())
                        continue;
                    if (std::abs(tile.get_rank() - hand_tile.get_rank()) > 2)
                        continue;

                    relevant_indices.push_back(index);
                }

                // Skip further computations if only one option for chow is available
                if (relevant_indices.size() == 3)
                {
                    for (int index : relevant_indices)
                    {
                        Mahjong::Tile &tile_to_reveal = tiles[index];
                        tile_to_reveal.set_visible();
                    }
                    return;
                }

                std::set<int> all_ranks = {};
                for (int index : relevant_indices)
                {
                    all_ranks.insert(tiles[index].get_rank());
                }

                // Skip further computations if only one option for chow is available
                if (all_ranks.size() == 3)
                {
                    for (int rank : all_ranks)
                    {
                        Mahjong::Tile relevant_tile = Mahjong::Tile(relevant_suit, rank);
                        auto it = std::find(tiles.begin(), tiles.end(), relevant_tile);
                        auto pos = std::distance(tiles.begin(), it);
                        Mahjong::Tile &tile_to_reveal = tiles[pos];
                        tile_to_reveal.set_visible();
                    }
                    return;
                }

                std::set<int> chow_starters = find_chow_starter_ranks(all_ranks);

                // Identify all possible chows
                int chow_starter = 0;
                if (chow_starters.size() == 1)
                    chow_starter = *std::next(chow_starters.begin(), 0);
                else
                {
                    // Get human choice if multiple chows are possible and player is human
                    if (is_human)
                    {
                        std::cout << "Multiple chows possible. Select which tile to start the chow with:\n";
                        for (int index = 0; index < chow_starters.size(); index++)
                        {
                            int relevant_rank = *std::next(chow_starters.begin(), index);
                            Mahjong::Tile temp_tile = Mahjong::Tile(relevant_suit, relevant_rank);
                            std::cout << index << ": " << temp_tile.get_tile_as_string() << "\n";
                        }
                        bool valid_input = false;
                        unsigned int input;
                        while (!valid_input)
                        {

                            std::cin >> input;
                            if (input < chow_starters.size())
                                valid_input = true;
                            else
                                std::cout << "Invalid input. Please select from the available options.\n";
                        }

                        chow_starter = *std::next(chow_starters.begin(), input);
                    }
                    // Select random chow choice if not human player
                    else
                    {
                        unsigned int input = std::rand() % (chow_starters.size());
                        chow_starter = *std::next(chow_starters.begin(), input);
                    }

                    // Reveal chosen chow
                    for (int rank_add = 0; rank_add < 3; rank_add++)
                    {
                        Mahjong::Tile relevant_tile = Mahjong::Tile(relevant_suit, chow_starter + rank_add);
                        auto it = std::find(tiles.begin(), tiles.end(), relevant_tile);
                        auto pos = std::distance(tiles.begin(), it);
                        Mahjong::Tile &tile_to_reveal = tiles[pos];
                        tile_to_reveal.set_visible();
                    }
                }
            }
        }

        /**
         * @brief Finds integers within a set that can serve as the starting point for a Mahjong chow combination.
         *
         * This function iterates through the provided set of integers and identifies those integers for which
         * the next two consecutive integers are also present in the set. These integers can serve as the starting
         * point for a Mahjong chow combination, where three consecutive ranks form a valid set.
         *
         * @param all_ranks A set of integers representing Mahjong tile ranks.
         * @return A set of integers that can be used as the starting point for Mahjong chow combinations.
         */
        std::set<int> find_chow_starter_ranks(const std::set<int> &all_ranks)
        {
            std::set<int> result;

            for (int num : all_ranks)
            {
                if (all_ranks.count(num + 1) && all_ranks.count(num + 2))
                {
                    result.insert(num);
                }
            }

            return result;
        }

        /**
         * @brief Finds all pairs of identical and hidden tiles in the Mahjong hand.
         *
         * This function iterates through the Mahjong hand and identifies all pairs of tiles
         * that have the same rank, suit, and are both hidden.
         *
         * @return A vector of sets, where each set represents the indices of tiles forming a pair.
         */
        std::vector<std::set<int>> get_pairs() const
        {
            std::vector<std::set<int>> pairs;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    // Check if the tiles are identical and are both hidden
                    if ((tiles[i] == tiles[j]) && tiles[i].is_hidden() && tiles[j].is_hidden())
                    {
                        std::set<int> pair = {i, j};
                        pairs.push_back(pair);
                    }
                }
            }

            return pairs;
        }

        /**
         * @brief Finds all chow combinations in the Mahjong hand.
         *
         * This function iterates through the Mahjong hand and identifies all chow combinations,
         * where three tiles have consecutive ranks, belong to the same suit, and have the same visibility state.
         *
         * @return A vector of sets, where each set represents the indices of tiles forming a chow combination.
         */
        std::vector<std::set<int>> get_chows() const
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
                        // Check if the tiles are identical and have the same visibility
                        int arr[] = {tiles[i].get_rank(), tiles[j].get_rank(), tiles[k].get_rank()};
                        std::sort(arr, arr + 3);
                        if ((arr[1] - arr[0] == 1 && arr[2] - arr[1] == 1) && (tiles[i].is_hidden() == tiles[j].is_hidden()) && (tiles[j].is_hidden() == tiles[k].is_hidden()))
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
         * @brief Finds all pong combinations in the Mahjong hand.
         *
         * This function iterates through the Mahjong hand and identifies all pong combinations,
         * where three tiles are identical and have the same visibility state.
         *
         * @return A vector of sets, where each set represents the indices of tiles forming a pong combination.
         */
        std::vector<std::set<int>> get_pongs() const
        {
            std::vector<std::set<int>> pongs;

            // Iterate through the vector
            for (int i = 0; i < tiles.size(); ++i)
            {
                for (int j = i + 1; j < tiles.size(); ++j)
                {
                    for (int k = j + 1; k < tiles.size(); k++)
                    {
                        // Check if the tiles are identical and have the same visibility
                        if (tiles[i] == tiles[j] && tiles[j] == tiles[k] && (tiles[i].is_hidden() == tiles[j].is_hidden()) && (tiles[j].is_hidden() == tiles[k].is_hidden()))
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
         * @brief Finds all kong combinations in the Mahjong hand.
         *
         * This function iterates through the Mahjong hand and identifies all kong combinations,
         * where four tiles are identical.
         *
         * @return A vector of sets, where each set represents the indices of tiles forming a kong combination.
         */
        std::vector<std::set<int>> get_kongs() const
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
        std::vector<std::set<int>> get_combinations() const
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
         * @brief Check if a given tile forms a chow in the concealed (hidden) hand.
         *
         * This function checks whether a specified tile forms a chow in the concealed (hidden) hand.
         * A chow is a set of three consecutive ranks in the same suit (except winds or dragons) in a
         * player's concealed hand.
         *
         * @param tile The tile to be checked for chow formation.
         *
         * @return True if the provided tile forms a chow in the concealed hand, false otherwise.
         */
        bool check_chow(const Mahjong::Tile tile)
        {
            unsigned int relevant_suit = tile.get_suit();

            if (relevant_suit == 3 || relevant_suit == 4)
                return false;

            unsigned int relevant_rank = tile.get_rank();

            std::vector<int> relevant_indices = {};

            // Get all indexes of relevant tiles
            for (int index = 0; index < tiles.size(); index++)
            {
                Mahjong::Tile hand_tile = tiles[index];
                if (!hand_tile.is_hidden())
                    continue;
                if (relevant_suit != hand_tile.get_suit())
                    continue;
                if (std::abs(tile.get_rank() - hand_tile.get_rank()) > 2)
                    continue;

                relevant_indices.push_back(index);
            }

            std::set<int> all_ranks = {};
            for (int index : relevant_indices)
            {
                all_ranks.insert(tiles[index].get_rank());
            }

            std::set<int> chow_starters = find_chow_starter_ranks(all_ranks);
            if (chow_starters.size() > 1)
                return true;
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
        std::vector<std::string> check_available_actions(const Discard_pile &discard_pile, unsigned int player_number, int current_player)
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

        /**
         * @brief Determines the type of Mahjong combination based on the given set of tile indices.
         *
         * This function identifies the type of Mahjong combination (e.g., pair, chow, pong, kong) based on the
         * number of tiles and their ranks in the provided combination.
         *
         * @param combination A set of integers representing the indices of tiles in the combination.
         * @return An unsigned integer representing the Mahjong combination type:
         *         - 0: Pair
         *         - 1: Chow
         *         - 2: Pong
         *         - 3: Kong
         */
        unsigned int get_combination_type(std::set<int> combination) const
        {
            // If combination contains two tiles then it must be a pair.
            if (combination.size() == 2)
                return 0;

            // If combination contains four tiles then it must be a kong.
            else if (combination.size() == 4)
                return 3;

            // If the combination contains 3 tiles and two have matching rank, it must be a pong.
            if (tiles[*std::next(combination.begin(), 0)].get_rank() == tiles[*std::next(combination.begin(), 1)].get_rank())
                return 2;

            // Return chow if no other valid combination.
            return 1;
        }

        /**
         * @brief Computes the maximum Mahjong score and its corresponding multiplier for the current hand.
         *
         * This function initializes the Mahjong score table, generates all possible combinations of tiles,
         * and calculates the maximum score and the sum of multipliers for the current hand based on the scoring table.
         *
         * @param round_wind The current round wind.
         * @param seat_wind The corresponding player's seat wind.
         *
         * @return A tuple containing the maximum Mahjong score and the corresponding sum of multipliers.
         */
        std::tuple<int, int> get_max_score(Mahjong::Wind round_wind, Mahjong::Wind seat_wind) const
        {
            unsigned int max_sum = 0;
            unsigned int max_multiplier_sum = 0;

            Mahjong::initialize_score_table();

            std::vector<std::set<int>> combinations = get_combinations();
            std::set<int> used_tiles;

            std::tie(max_sum, max_multiplier_sum) = get_score_recursive(combinations, used_tiles, 0, 0, round_wind, seat_wind);

            return std::make_tuple(max_sum, max_multiplier_sum);
        }

        /**
         * @brief Recursively explores all possible combinations of tiles to find the maximum Mahjong score.
         *
         * This function is part of the Mahjong scoring algorithm and is called recursively to explore different combinations
         * of tiles while avoiding overlaps. It calculates the maximum score and the sum of multipliers for the current hand.
         *
         * @param combinations A vector of sets representing all possible combinations of tiles.
         * @param used_tiles A set containing indices of tiles that have been used in the current exploration.
         * @param current_index The index of the current combination being explored.
         * @param current_multiplier_sum The current sum of multipliers for the explored combinations.
         * @param round_wind The current round wind.
         * @param seat_wind The corresponding player's seat wind.
         *
         * @return A tuple containing the maximum Mahjong score and the corresponding sum of multipliers.
         */
        std::tuple<int, int> get_score_recursive(const std::vector<std::set<int>> &combinations, std::set<int> &used_tiles, int current_index, int current_multiplier_sum, Mahjong::Wind round_wind, Mahjong::Wind seat_wind) const
        {
            int max_sum = 0;
            int max_multiplier_sum = current_multiplier_sum;

            for (int i = current_index; i < combinations.size(); i++)
            {
                const std::set<int> &current_combination = combinations[i];

                bool overlap = false;
                for (int tile : current_combination)
                {
                    if (used_tiles.count(tile) > 0)
                    {
                        overlap = true;
                        break;
                    }
                }

                if (!overlap)
                {
                    used_tiles.insert(current_combination.begin(), current_combination.end());
                    auto [current_score, current_multi] = get_combination_score(current_combination, round_wind, seat_wind);

                    // Recursively explore other combinations
                    auto [next_sum, next_multiplier_sum] = get_score_recursive(combinations, used_tiles, i + 1, current_multiplier_sum, round_wind, seat_wind);

                    // Update max_sum and max_multiplier_sum if needed
                    if (next_sum + current_score > max_sum)
                    {
                        max_sum = next_sum + current_score;
                        max_multiplier_sum = next_multiplier_sum + current_multi;
                    }

                    // Backtrack
                    for (int tile : current_combination)
                    {
                        used_tiles.erase(tile);
                    }
                }
            }

            return std::make_tuple(max_sum, max_multiplier_sum);
        }

        /**
         * @brief Computes the Mahjong score for a given combination of tiles.
         *
         * This function initializes the Mahjong score table and calculates the score for a specific combination
         * of tiles based on the combination type, suit, visibility of the tiles as well as the current seat and round winds.
         *
         * @param combination A set of integers representing the indices of tiles in the combination.
         * @param round_wind The current round wind.
         * @param seat_wind The corresponding player's seat wind.
         *
         * @return A tuple containing the Mahjong score and the corresponding multiplier for the given combination.
         */
        std::tuple<int, int> get_combination_score(std::set<int> combination, Mahjong::Wind round_wind, Mahjong::Wind seat_wind) const
        {
            Mahjong::initialize_score_table();

            unsigned int type = get_combination_type(combination);
            unsigned int suit = tiles[*std::next(combination.begin(), 0)].get_suit();
            unsigned int visibility = 0;
            unsigned int wind = 0;

            std::set<int> all_visibilities;
            for (int index : combination)
            {
                all_visibilities.insert(static_cast<int>(tiles[index].is_hidden()));
            }

            if (all_visibilities.count(0) == 0)
            {
                visibility = 1; // No visible tiles, i.e. all hidden.
            }
            else if (all_visibilities.count(1) == 0)
            {
                visibility = 1; // No hidden tiles, i.e. all visible.
            }
            else
            {
                visibility = 2; // Multiple visibility states
            }

            // Add wind information if needed
            if ((suit == 3) && (type > 1))
            {
                unsigned int combination_wind = tiles[*std::next(combination.begin(), 0)].get_rank();
                if (combination_wind == round_wind.get_wind())
                {
                    wind += 1;
                }
                if (combination_wind == seat_wind.get_wind())
                {
                    wind += 1;
                }
                // std::cout << type << suit << visibility << wind << "\n";
            }

            // std::cout << type << suit << visibility << wind << "\n";

            return Mahjong::score_table[{type, suit, visibility, wind}];
        }

        /**
         * @brief Returns the Mahjong score associated to the visible tiles.
         *
         * Constructs a temporary instance of a hand containing only the visible tiles and computes the score accordingly.
         *
         * @param round_wind The current round wind.
         * @param seat_wind The corresponding player's seat wind.
         *
         * @return A tuple containing the Mahjong score and the corresponding multiplier for the given combination.
         */
        std::tuple<int, int> get_visible_score(Mahjong::Wind round_wind, Mahjong::Wind seat_wind) const
        {

            Mahjong::Hand temp_hand = Hand();
            for (Mahjong::Tile tile : tiles)
            {
                if (!tile.is_hidden())
                    temp_hand.add_tile(tile);
            }

            return temp_hand.get_max_score(round_wind, seat_wind);
        }

        /**
         * @brief Returns a vector of tiles representing all the tiles in the player's hand.
         *
         * @return A vector of Mahjong::Tile objects representing the tiles in the hand.
         */
        std::vector<Mahjong::Tile> get_tiles() const
        {
            return tiles;
        }

        /**
         * @brief Returns the tile at the specified index in the player's hand.
         *
         * @param index The index of the tile to retrieve.
         *
         * @return The Mahjong::Tile object at the specified index.
         */
        Mahjong::Tile get_tile_by_index(int index) const
        {
            if (index == -1)
            {
                return tiles.back();
            }
            return tiles[index];
        }

        /**
         * @brief Returns the number of occurrences of a specific tile in the player's hand.
         *
         * @param tile The tile whose occurrences are to be counted.
         *
         * @return The number of occurrences of the specified tile.
         */
        unsigned int get_n_tile_occurence(Mahjong::Tile tile) const
        {
            return std::count(tiles.begin(), tiles.end(), tile);
        }

        /**
         * @brief Returns the number of tiles of a specific suit in the player's hand.
         *
         * @param suit The suit for which the number of tiles is to be counted.
         *
         * @return The number of tiles of the specified suit.
         */
        unsigned int get_n_tiles_of_suit(int suit) const
        {
            unsigned int n = 0;
            for (Mahjong::Tile tile : tiles)
            {
                if (tile.get_suit() == suit)
                    n += 1;
            }
            return n;
        }

        /**
         * @brief Sets the visibility of Mahjong tiles at specified indices to true.
         *
         * @param indices A vector containing the indices of the Mahjong tiles in the hand whose visibility should be set to true.
         */
        void set_tiles_visible(std::vector<int> indices)
        {
            for (int index : indices)
            {
                Mahjong::Tile &tile = tiles[index];
                tile.set_visible();
            }
        }

        /**
         * @brief Gets all suits currently in hand.
         *
         * @return A set of all integer values representing the suits in hand.
         */
        std::set<int> get_all_suits() const
        {
            std::set<int> all_suits;
            for (Mahjong::Tile tile : tiles)
            {
                all_suits.insert(tile.get_suit());
            }
            return all_suits;
        }

        /**
         * @brief Gets all ranks currently in hand. Wind and dragon tiles are ignored.
         *
         * @return A set of all integer values representing the ranks in hand.
         */
        std::set<int> get_all_ranks() const
        {
            std::set<int> all_ranks;
            for (Mahjong::Tile tile : tiles)
            {
                if (tile.get_suit() < 3)
                {
                    all_ranks.insert(tile.get_rank());
                }
            }
            return all_ranks;
        }
    };
} // namespace Mahjong

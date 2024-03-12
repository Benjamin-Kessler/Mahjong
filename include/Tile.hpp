/**
 * @file Tile.hpp
 * @brief Defines the Tile class representing a single tile in a Mahjong game.
 */

#pragma once
#include <string>

/** @brief String names for the ranks of the tiles with Dragon suit. */
std::string DRAGONS[3] = {"Red", "Green", "White"};

/** @brief String names for the ranks of the tiles with Wind suit. */
std::string WINDS[4] = {"East", "South", "West", "North"};

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{

    /*
    Class for tiles. Each tile consits of a suit and a rank. The number of available ranks depends on the given suit.
    */
    class Tile
    {
    private:
        int suit;           /**< The suit of the tile. */
        int rank;           /**< The rank of the tile. */
        bool hidden = true; /**< Flag indicating whether the tile is hidden or visible. */

    public:
        /**
         * @brief Default constructor for Tile.
         *
         * Initializes the tile with a random suit and rank, ensuring proper distribution based on the suit.
         */
        Tile() : suit(std::rand() % 5)
        {
            if (suit == 3)
            {
                rank = std::rand() % 4;
            }
            else if (suit == 4)
            {
                rank = std::rand() % 3;
            }
            else
            {
                rank = std::rand() % 9 + 1;
            }
        }

        /**
         * @brief Constructor for Tile using input variabls
         *
         * @param suit_in Int of the desired suit
         * @param rank_in Int of the desired rank
         * @return Tile with the provided suit and rank
         */
        Tile(int suit_in, int rank_in) : suit(suit_in), rank(rank_in) {}

        /**
         * @brief Get the rank of the tile.
         * @return The rank of the tile.
         */
        int get_rank() const
        {
            return rank;
        }

        /**
         * @brief Get the suit of the tile.
         * @return The suit of the tile.
         */
        int get_suit() const
        {
            return suit;
        }

        /**
         * @brief Check if the tile is hidden.
         * @return True if the tile is hidden, false if it's visible.
         */
        bool is_hidden() const
        {
            return hidden;
        }

        /**
         * @brief Get a string representation of the tile (suit and rank).
         * @return A string representing the tile.
         */
        std::string get_tile_as_string() const
        {
            std::string suit_string, rank_string;

            switch (suit)
            {
            case 0:
                suit_string = "Circles";
                rank_string = std::to_string(rank);
                break;
            case 1:
                suit_string = "Bamboos";
                rank_string = std::to_string(rank);
                break;
            case 2:
                suit_string = "Characters";
                rank_string = std::to_string(rank);
                break;
            case 3:
                suit_string = "Winds";
                rank_string = WINDS[rank];
                break;
            case 4:
                suit_string = "Dragons";
                rank_string = DRAGONS[rank];
                break;
            }
            return suit_string + " " + rank_string;
        }

        /**
         * @brief Get a string representation of the tile with visibility status.
         * @return A string representing the tile, including visibility status.
         */
        std::string get_tile_as_string_with_visibility() const
        {
            std::string suit_string, rank_string;

            switch (suit)
            {
            case 0:
                suit_string = "Circles";
                rank_string = std::to_string(rank);
                break;
            case 1:
                suit_string = "Bamboos";
                rank_string = std::to_string(rank);
                break;
            case 2:
                suit_string = "Characters";
                rank_string = std::to_string(rank);
                break;
            case 3:
                suit_string = "Winds";
                rank_string = WINDS[rank];
                break;
            case 4:
                suit_string = "Dragons";
                rank_string = DRAGONS[rank];
                break;
            }
            std::string hidden_flag = (hidden) ? " (Hidden)" : " (Open)";
            return suit_string + " " + rank_string + hidden_flag;
        }

        /**
         * @brief Equality operator for comparing two tiles.
         * @param other The other tile to compare.
         * @return True if the tiles are equal, false otherwise.
         */
        bool operator==(const Tile &other) const
        {
            return (suit == other.suit) && (rank == other.rank);
        }

        /**
         * @brief Set the visibility of the tile to open.
         */
        void set_visible()
        {
            hidden = false;
        }
    };
} // namespace Mahjong

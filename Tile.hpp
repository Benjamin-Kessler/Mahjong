#pragma once
#include <string>

std::string dragons[3] = {"Red", "Green", "White"};
std::string winds[4] = {"East", "South", "West", "North"};

/*
Class for tiles. Each tile consits of a suit and a rank. The number of available ranks depends on the given suit.
*/
class Tile
{
private:
    int suit;
    int rank;
    bool hidden = true;

public:
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

    Tile(int suit_in, int rank_in) : suit(suit_in), rank(rank_in) {}

    int get_rank() const
    // Function that returns the rank of a tile.
    {
        return Tile::rank;
    }

    int get_suit() const
    // Function that returns the suit of a tile.
    {
        return Tile::suit;
    }

    bool is_hidden() const
    {
        return Tile::hidden;
    }

    std::string get_tile_as_string() const
    // Function that returns the suit and rank of a tile as a string.
    {
        std::string suit_string, rank_string;

        switch (Tile::suit)
        {
        case 0:
            suit_string = "Circles";
            rank_string = std::to_string(Tile::rank);
            break;
        case 1:
            suit_string = "Bamboos";
            rank_string = std::to_string(Tile::rank);
            break;
        case 2:
            suit_string = "Characters";
            rank_string = std::to_string(Tile::rank);
            break;
        case 3:
            suit_string = "Winds";
            rank_string = winds[Tile::rank];
            break;
        case 4:
            suit_string = "Dragons";
            rank_string = dragons[Tile::rank];
            break;
        }
        return suit_string + " " + rank_string;
    }

    std::string get_tile_as_string_with_visibility() const
    // Function that returns the suit and rank of a tile as a string.
    {
        std::string suit_string, rank_string;

        switch (Tile::suit)
        {
        case 0:
            suit_string = "Circles";
            rank_string = std::to_string(Tile::rank);
            break;
        case 1:
            suit_string = "Bamboos";
            rank_string = std::to_string(Tile::rank);
            break;
        case 2:
            suit_string = "Characters";
            rank_string = std::to_string(Tile::rank);
            break;
        case 3:
            suit_string = "Winds";
            rank_string = winds[Tile::rank];
            break;
        case 4:
            suit_string = "Dragons";
            rank_string = dragons[Tile::rank];
            break;
        }
        std::string hidden_flag = (Tile::hidden) ? " (Hidden)" : " (Open)";
        return suit_string + " " + rank_string + hidden_flag;
    }

    bool operator==(const Tile &other) const
    {
        return (suit == other.suit) && (rank == other.rank);
    }

    void set_visible()
    {
        hidden = false;
    }
};
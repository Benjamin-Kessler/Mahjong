#pragma once

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
    Tile()
    // Function that creates a random tile.
    {
        // Get random suit and random rank according to the given possibilities for each suit.
        suit = std::rand() % (int)(5);
        if (suit == 3)
        {
            rank = std::rand() % (int)(4);
        }
        else if (suit == 4)
        {
            rank = std::rand() % (int)(3);
        }
        else
        {
            rank = std::rand() % (int)(9);
        }
    }

    Tile(int suit_in, int rank_in)
    // Function that creates a tile which corresponds to the given integers.
    {
        suit = suit_in;
        rank = rank_in;
    }

    int get_rank()
    // Function that returns the rank of a tile.
    {
        return rank;
    }

    int get_suit()
    // Function that returns the suit of a tile.
    {
        return suit;
    }

    std::string get_tile_as_string()
    // Function that returns the suit and rank of a tile as a string.
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
            rank_string = winds[rank];
            break;
        case 4:
            suit_string = "Dragons";
            rank_string = dragons[rank];
            break;
        }
        return suit_string + " " + rank_string;
    }
};
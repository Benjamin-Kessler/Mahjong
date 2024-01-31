#pragma once
#include <iostream>
#include <unordered_map>
#include <tuple>

/**
 * @brief Custom hash function for tuple.
 */
struct TupleHash
{
    /**
     * @brief Hashes a tuple.
     * @tparam Tuple Type of the tuple.
     * @param t The tuple to hash.
     * @return Hash value.
     */
    template <class Tuple>
    std::size_t operator()(const Tuple &t) const
    {
        // Using a combination of the hash values of the tuple elements
        return std::hash<typename std::tuple_element<0, Tuple>::type>{}(std::get<0>(t)) ^
               std::hash<typename std::tuple_element<1, Tuple>::type>{}(std::get<1>(t)) ^
               std::hash<typename std::tuple_element<2, Tuple>::type>{}(std::get<2>(t));
    }
};

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{

    /**
     * @brief Declaration of the lookup table with custom hash function.
     *
     * The keys represent the combination type (0 = pair, 1 = chow, 2 = pong, 3 = kong),
     * the suit of the corresponding tiles (0 to 4) and whether the tiles are hidden (1) or not (0).
     * The values represent the score of the given combination as well as it's multiplyer as a power of two.
     *
     */
    std::unordered_map<std::tuple<int, int, int>, std::tuple<int, int>, TupleHash> score_table;

    void initialize_score_table()
    {
        // Set scores for pairs
        score_table[{0, 0, 1}] = {0, 0};
        score_table[{0, 1, 1}] = {0, 0};
        score_table[{0, 2, 1}] = {0, 0};
        score_table[{0, 3, 1}] = {2, 0};
        score_table[{0, 4, 1}] = {2, 0};

        // Set scores for chows
        score_table[{1, 0, 0}] = {0, 0};
        score_table[{1, 0, 1}] = {0, 0};

        score_table[{1, 1, 0}] = {0, 0};
        score_table[{1, 1, 1}] = {0, 0};

        score_table[{1, 2, 0}] = {0, 0};
        score_table[{1, 2, 1}] = {0, 0};

        score_table[{1, 3, 0}] = {0, 0};
        score_table[{1, 3, 1}] = {0, 0};

        score_table[{1, 4, 0}] = {0, 0};
        score_table[{1, 4, 1}] = {0, 0};

        // Set scores for pongs
        score_table[{2, 0, 0}] = {4, 0};
        score_table[{2, 0, 1}] = {8, 0};

        score_table[{2, 1, 0}] = {4, 0};
        score_table[{2, 1, 1}] = {8, 0};

        score_table[{2, 2, 0}] = {4, 0};
        score_table[{2, 2, 1}] = {8, 0};

        score_table[{2, 3, 0}] = {8, 1};
        score_table[{2, 3, 1}] = {16, 1};

        score_table[{2, 4, 0}] = {8, 1};
        score_table[{2, 4, 1}] = {16, 1};

        // Set scores for kongs (a visibility of 2 mans that it is partially open)
        score_table[{3, 0, 0}] = {8, 1};
        score_table[{3, 0, 1}] = {16, 1};
        score_table[{3, 0, 2}] = {16, 1};

        score_table[{3, 1, 0}] = {8, 1};
        score_table[{3, 1, 1}] = {16, 1};
        score_table[{3, 1, 2}] = {16, 1};

        score_table[{3, 2, 0}] = {8, 1};
        score_table[{3, 2, 1}] = {16, 1};
        score_table[{3, 2, 2}] = {16, 1};

        score_table[{3, 3, 0}] = {16, 2};
        score_table[{3, 3, 1}] = {32, 2};
        score_table[{3, 3, 2}] = {32, 2};

        score_table[{3, 4, 0}] = {16, 2};
        score_table[{3, 4, 1}] = {32, 2};
        score_table[{3, 4, 2}] = {32, 2};
    }

} // namespace Mahjong
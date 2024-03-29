/**
 * @file Game.hpp
 * @brief Definition of the Mahjong::Game class representing a game of Mahjong.
 */
#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <tuple>

#include "Set.hpp"
#include "State.hpp"
#include "Player.hpp"
#include "Discard_pile.hpp"
#include "Wind.hpp"

/** @brief Number of players per game. */
unsigned int N_PLAYERS = 4;

/**
 * @namespace Mahjong
 * @brief Namespace for Mahjong-related classes and functions.
 */
namespace Mahjong
{
    /**
     * @class Game
     * @brief Represents a Mahjong game with players, a set of tiles, and a discard pile.
     */
    class Game
    {
    private:
        int id;                      ///< Unique identifier for the game.
        bool running;                ///< Flag indicating whether the game is currently running.
        std::vector<Player> players; ///< List of players participating in the game.
        std::vector<int> scores;     ///< List of player scores.
        Mahjong::Set set;            ///< Set of Mahjong tiles used in the game.
        Discard_pile discard_pile;   ///< Discard pile for tiles during the game.
        unsigned int current_player; ///< Index of the current player taking their turn.
        int n_rounds;                ///< Number of rounds completed in the game.
        Mahjong::Wind round_wind;    ///< Current round wind

    public:
        /**
         * @brief Constructor for the Game class.
         *
         * @param id_in Unique identifier for the game.
         */
        Game(int id_in) : id(id_in), running(true), current_player(0), n_rounds(0), round_wind(0)
        {
            std::cout << "Initiated Game with ID " << id << "\n";

            discard_pile = Discard_pile();

            set = Mahjong::Set();
            set.shuffle();

            players = {};
            for (size_t player_number = 0; player_number < N_PLAYERS; player_number++)
            {
                players.push_back(Player(player_number, set));
                scores.push_back(0);
                std::cout << "Initiated Player " << player_number << "\n";
            }

            std::cout
                << "Number of tiles in set: " << set.get_size() << "\n";
        }

        /**
         * @brief Starts the next round of the game.
         */
        void next_round()
        {
            std::cout << "Start round " << n_rounds << "\n";
            running = true;
            n_rounds += 1;
            round_wind = Mahjong::Wind(n_rounds % 4);
            current_player = n_rounds % 4;

            discard_pile = Mahjong::Discard_pile();

            set = Mahjong::Set();
            set.shuffle();

            players = {};
            for (size_t player_number = 0; player_number < N_PLAYERS; player_number++)
            {
                Mahjong::Player new_player = Player(player_number, set);
                for (int i = 0; i < n_rounds; i++)
                {
                    new_player.rotate_seat_wind();
                }
                players.push_back(new_player);
            }
        }

        /**
         * @brief Resets the game to its initial state.
         */
        void reset()
        {
            std::cout << "Reset Game with ID " << id << "\n";

            running = true;
            current_player = 0;

            discard_pile = Mahjong::Discard_pile();

            set = Mahjong::Set();
            set.shuffle();

            players = {};
            for (size_t player_number = 0; player_number < N_PLAYERS; player_number++)
            {
                players.push_back(Player(player_number, set));
                std::cout << "Initiated Player " << player_number << "\n";
            }

            std::cout
                << "Number of tiles in set: " << set.get_size() << "\n";
        }

        /**
         * @brief Updates the set of tiles used in the game.
         *
         * @param set New set of tiles.
         */
        void update_set(Mahjong::Set set)
        {
            set = set;
        }

        /**
         * @brief Displays the hand of a specified player.
         *
         * @param player_number Index of the player.
         */
        void display_player_hand(unsigned int player_number)
        {
            Player &player = players[player_number];
            player.display_hand();
        }

        /**
         * @brief Displays the visible portion of a player's hand.
         *
         * @param player_number Index of the player.
         */
        void display_visible_player_hand(unsigned int player_number)
        {
            Player &player = players[player_number];
            player.display_visible_hand();
        }

        /**
         * @brief Sorts the hand of a specified player.
         *
         * @param player_number Index of the player.
         */
        void sort_player_hand(unsigned int player_number)
        {
            Player &player = players[player_number];
            player.sort_player_hand();
        }

        /**
         * @brief Simulates a player drawing a tile during their turn.
         *
         * @param player_number Index of the player.
         * @param broadcast Indicates whether the draw should be broadcasted.
         */
        void player_draw(unsigned int player_number, bool broadcast)
        {
            Player &player = players[player_number];
            player.draw_tile(set, broadcast);
        }

        /**
         * @brief Simulates a player picking a tile from the discard pile.
         *
         * @param player_number Index of the player.
         * @param action Type of action (kong, pong, chow).
         */
        void player_pick_from_discard(unsigned int player_number, std::string action)
        {
            Player &player = players[player_number];
            Mahjong::Tile tile_to_pickup = discard_pile.back();
            player.pick_tile_from_discard(discard_pile);
            player.reveal_combination(tile_to_pickup, action);
        }

        /**
         * @brief Simulates a player discarding a tile during their turn.
         *
         * @param player_number Index of the player.
         */
        void player_discard(unsigned int player_number)
        {
            Player &player = players[player_number];
            player.discard_tile(discard_pile, get_game_state_for_player(player_number));
        }

        /**
         * @brief Allows a player to choose a pickup action based on the current game state.
         *
         * @param player_number Index of the player.
         * @param current_player Index of the current player.
         * @return The chosen pickup action.
         */
        std::string player_choose_pickup_action(unsigned int player_number, unsigned int current_player)
        {
            Player &player = players[player_number];
            return player.choose_pickup_action(discard_pile, current_player, get_game_state_for_player(player_number));
        }

        /**
         * @brief Prioritizes the pickup actions of all players and returns the highest priority action.
         *
         * The priority is kong, pong, and at last chow.
         *
         * @param player_actions Vector containing pickup actions of all players.
         *
         * @return A tuple containing the index and type of the highest priority action.
         */
        std::tuple<int, std::string> prioritize_pickup_action(std::vector<std::string> player_actions)
        {
            auto index = std::find(player_actions.begin(), player_actions.end(), "kong") - player_actions.begin();
            if (index < player_actions.size())
            {
                return std::make_tuple(static_cast<int>(index), "kong");
            }
            index = std::find(player_actions.begin(), player_actions.end(), "pong") - player_actions.begin();
            if (index < player_actions.size())
            {
                return std::make_tuple(static_cast<int>(index), "pong");
            }
            index = std::find(player_actions.begin(), player_actions.end(), "chow") - player_actions.begin();
            if (index < player_actions.size())
            {
                return std::make_tuple(static_cast<int>(index), "chow");
            }
            return std::make_tuple(-1, "none");
        }

        /**
         * @brief Determines the pickup action with highest priority and the corresponding player.
         *
         * Evaluates all available pickup actions for all players. After the players chose with
         * which action they wish to proceed, the priority of the chosen actions is determined.
         * Returns a tuple containing the player number and the action (as string) of the prioritized
         * action.
         *
         * @param current_player Index of the current player.
         *
         * @return A tuple containing the player number and type of the determined pickup action.
         */
        std::tuple<int, std::string> pickup_action(unsigned int current_player)
        {
            std::vector<std::string> player_actions = {};
            for (size_t i = 0; i < players.size(); i++)
            {
                if (i == current_player) // Players can't pick up tiles they discarded themself.
                {
                    player_actions.push_back("none");
                }
                else
                {
                    std::string player_action = player_choose_pickup_action(i, current_player);
                    player_actions.push_back(player_action);
                }
            }
            std::tuple<int, std::string> pickup_tuple = prioritize_pickup_action(player_actions);
            return pickup_tuple;
        }

        /**
         * @brief Check if a player has a winning Mahjong hand.
         *
         * This function checks if the specified player has a winning Mahjong hand by calling the
         * `has_winning_hand` method of the Player class. If the player has a winning hand, a message
         * is displayed indicating the winning status, and the game is set to a non-running state.
         *
         * @param player_number The player number to check for a winning hand.
         */
        void player_has_winning_hand(unsigned int player_number)
        {
            Player &player = players[player_number];
            if (player.has_winning_hand())
            {
                std::cout << "Player " << player_number << " has a winning hand. Congratulations.\n";
                player.display_player_score(round_wind, true, true);
                running = false;
            }
        }

        /**
         * @brief Display the score of the specified player.
         *
         * This function displays the score of the specified player. It delegates the task of displaying
         * the score to the `display_player_score` method of the Player class.
         *
         * @param player_number The number of the player whose score is to be displayed.
         * @param full_hand Indicates whether to display the full hand score.
         * @param mahjong Indicates whether the player has Mahjong.
         */
        void display_player_score(unsigned int player_number, bool full_hand = false, bool mahjong = false)
        {
            Mahjong::Player &player = players[player_number];
            player.display_player_score(round_wind, full_hand, mahjong);
        }

        /**
         * @brief Get the score of the specified player.
         *
         * This function calculates and returns the score of the specified player based on the round
         * wind, full hand status, and whether the player has Mahjong. It delegates the task of
         * calculating the score to the `get_player_score` method of the Player class.
         *
         * @param player_number The number of the player whose score is to be calculated.
         * @param full_hand Indicates whether to calculate the full hand score.
         * @param mahjong Indicates whether the player has Mahjong.
         * @return The calculated score of the player.
         */
        int get_player_score(unsigned int player_number, bool full_hand, bool mahjong)
        {
            Mahjong::Player &player = players[player_number];
            std::tuple<int, int> scores = player.get_player_score(round_wind, full_hand);
            int score = std::get<0>(scores);
            if (mahjong)
                score += 20;
            return (score * std::pow(2, std::get<1>(scores)));
        }

        /**
         * @brief Simulates a player's turn, including drawing, sorting, and discarding.
         *
         * @param player_number Index of the player.
         * @param broadcast Indicates whether the turn should be broadcasted.
         */
        void player_turn(unsigned int player_number, bool broadcast)
        {
            player_draw(player_number, broadcast);
            sort_player_hand(player_number);
            if (broadcast)
                display_player_hand(player_number);
            else
                display_visible_player_hand(player_number);
            display_player_score(player_number, broadcast, false);

            player_has_winning_hand(player_number);
            if (running)
                player_discard(player_number);
        }

        /**
         * @brief Gets the size of the set of tiles.
         *
         *  @return The size of the set.
         */
        int get_set_size() const
        {
            return set.get_size();
        }

        /**
         * @brief Gets the size of the discard pile.
         *
         * @return The size of the discard pile.
         */
        unsigned int get_pile_size() const
        {
            return discard_pile.get_size();
        }

        /**
         * @brief Displays the contents of the discard pile.
         */
        void display_discard_pile() const
        {
            std::cout << "Discard pile:\n";
            discard_pile.display_discard_pile();
            std::cout << "\n";
        }

        /**
         * @brief Sets a player as a human player.
         *
         * @param player_number Index of the player to set as human.
         */
        void set_human(unsigned int player_number)
        {
            Player &player = players[player_number];
            player.set_human();
        }

        /**
         * @brief Gets the list of players in the game.
         *
         * @return Vector of Player objects.
         */
        std::vector<Player> get_players() const
        {
            return players;
        }

        /**
         * @brief Checks if the game is currently running.
         *
         * @return True if the game is running, false otherwise.
         */
        bool is_running() const
        {
            return running;
        }

        /**
         * @brief Finishes the game.
         */
        void finish()
        {
            running = false;
        }

        /**
         * @brief Gets the index of the current player.
         *
         * @return The index of the current player.
         */
        unsigned int get_current_player() const
        {
            return current_player;
        }

        /**
         * @brief Sets the index of the current player.
         *
         * @param new_current_player The new index of the current player.
         */
        void set_current_player(unsigned int new_current_player)
        {
            current_player = new_current_player;
        }

        /**
         * @brief Sets the policy for a specific player.
         *
         * @param player_number The index of the player whose policy is to be set.
         * @param new_policy The new policy to be set for the player.
         */
        void set_player_policy(unsigned int player_number, std::string new_policy)
        {
            Player &player = players[player_number];
            player.set_policy(new_policy);
        }

        /**
         * @brief Retrieves the game state from the perspective of a specific player.
         *
         * This function constructs and returns the game state object from the perspective of a specific player.
         * The game state includes the hands of all players, where the hand of the specified player may be either
         * the full hand or only the visible tiles depending on the player's perspective.
         *
         * @param player_number The index of the player for whom the game state is to be retrieved.
         *
         * @return A Mahjong::State object representing the game state from the perspective of the specified player.
         */
        Mahjong::State get_game_state_for_player(unsigned int player_number)
        {
            std::vector<Mahjong::Hand> hands = {};

            for (int i = 0; i < N_PLAYERS; i++)
            {
                Mahjong::Player &player = players[i];
                int pos = i + 1;
                if (i == player_number)
                    hands.push_back(player.get_full_hand());
                else
                    hands.push_back(player.get_visible_hand());
            }

            Mahjong::Player player = get_players()[player_number];
            unsigned int seat_wind = player.get_seat_wind().get_wind();
            unsigned int round_wind_int = round_wind.get_wind();

            Mahjong::State game_state = Mahjong::State(player_number, seat_wind, round_wind_int, hands, discard_pile);
            return game_state;
        }

        /**
         * @brief Adds the final score of the specified player to the cumulative scores.
         *
         * This function calculates the final score of the specified player and adds it to the cumulative scores.
         * The final score is calculated based on the round wind, the full hand status, and whether the player
         * has Mahjong. The cumulative score of the player is then updated accordingly.
         *
         * @param player_number The index of the player for whom the final score is to be added.
         * @param mahjong Indicates whether the player has Mahjong.
         */
        void add_final_score(unsigned int player_number, bool mahjong = false)
        {
            Mahjong::Player &player = players[player_number];
            std::tuple<int, int> score = player.get_player_score(round_wind, true, mahjong);
            unsigned int unmodified_score = std::get<0>(score);
            unsigned int multiplier = std::get<1>(score);

            int total_score = unmodified_score * std::pow(2, multiplier);
            if (total_score > 3000)
            {
                total_score = 3000;
            }
            scores[player_number] += total_score;
        }

        /**
         * @brief Displays the cumulative scores of all players.
         *
         * This function displays the cumulative scores of all players in the current game.
         */
        void display_cumulative_scores() const
        {
            std::cout << "Current scores:\n";
            for (int i = 0; i < players.size(); i++)
            {
                std::cout << "Player " << i << ": " << scores[i] << "\n";
            }
        }
    };
} // namespace Mahjong

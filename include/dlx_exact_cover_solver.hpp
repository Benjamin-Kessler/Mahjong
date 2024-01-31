/**
 * @file dlx_exact_cover_solver.hpp
 * @brief Implementation of Knuth's Algorithm X using Dancing Links for Exact Cover problems.
 */
#pragma once
#include <iostream>
#include <set>
#include <vector>

// Define maximum number of rows and columns in the problem matrix
#define MAX_ROW 100
#define MAX_COL 100

// Define the default number of columns
#define N_COLUMNS 14

/**
 * @namespace DLX
 * @brief Namespace for dancing links classes and functions.
 */
namespace DLX
{
    /**
     * @struct Node
     * @brief Represents elements in the linked matrix.
     */
    struct Node
    {
    public:
        struct Node *left;   /**< Pointer to the left node. */
        struct Node *right;  /**< Pointer to the right node. */
        struct Node *up;     /**< Pointer to the upper node. */
        struct Node *down;   /**< Pointer to the lower node. */
        struct Node *column; /**< Pointer to the column header. */
        int row_id;          /**< Row identifier. */
        int col_id;          /**< Column identifier. */
        int node_count;      /**< Count of nodes in the column. */
    };

    /**
     * @class exact_cover_solver
     * @brief Class implementing Knuth's Algorithm X with Dancing Links for Exact Cover problems.
     */
    class exact_cover_solver
    {
    private:
        Node *header;                                /**< Head node for the linked matrix. */
        Node matrix[MAX_ROW][MAX_COL];               /**< 2D array representing the linked matrix. */
        bool prob_matrix[MAX_ROW][MAX_COL];          /**< Problem matrix indicating presence of elements. */
        std::vector<struct Node *> solutions;        /**< Vector to store solution nodes. */
        std::vector<std::set<int>> solutions_vector; /**< Vector to store unique solutions. */
        int n_row, n_col = 0;                        /**< Number of rows and columns in the matrix. */

    public:
        /**
         * @brief Constructor: Allocates memory for the header node.
         */
        exact_cover_solver()
        {
            // Allocate memory for the header node
            header = new Node();
        };

        /**
         * @brief Destructor: Frees memory allocated for the header node.
         */
        ~exact_cover_solver()
        {
            delete header;
        }

        /**
         * @brief Gets the next index in the right direction for a given index.
         * @param i Index.
         * @return Next index in the right direction.
         */
        int get_right(int i) { return (i + 1) % n_col; }

        /**
         * @brief Gets the next index in the left direction for a given index.
         * @param i Index.
         * @return Next index in the left direction.
         */
        int get_left(int i) { return (i - 1 < 0) ? n_col - 1 : i - 1; }

        /**
         * @brief Gets the next index in the up direction for a given index.
         * @param i Index.
         * @return Next index in the up direction.
         */
        int get_up(int i) { return (i - 1 < 0) ? n_row : i - 1; }

        /**
         * @brief Gets the next index in the down direction for a given index.
         * @param i Index.
         * @return Next index in the down direction.
         */
        int get_down(int i) { return (i + 1) % (n_row + 1); }

        /**
         * @brief Creates a 4-way linked matrix of nodes (Toroidal Matrix).
         */
        void create_toridol_matrix()
        {
            // One extra row for list header nodes
            // for each column
            for (int i = 0; i <= n_row; i++)
            {
                for (int j = 0; j < n_col; j++)
                {
                    // If it's 1 in the problem matrix then
                    // only create a node
                    if (prob_matrix[i][j])
                    {
                        int a, b;

                        // If it's 1, other than 1 in 0th row
                        // then count it as node of column
                        // and increment node count in column header
                        if (i)
                            matrix[0][j].node_count += 1;

                        // Add pointer to column header for this
                        // column node
                        matrix[i][j].column = &matrix[0][j];

                        // set row and column id of this node
                        matrix[i][j].row_id = i;
                        matrix[i][j].col_id = j;

                        // Link the node with neighbors

                        // Left pointer
                        a = i;
                        b = j;
                        do
                        {
                            b = get_left(b);
                        } while (!prob_matrix[a][b] && b != j);
                        matrix[i][j].left = &matrix[i][b];

                        // Right pointer
                        a = i;
                        b = j;
                        do
                        {
                            b = get_right(b);
                        } while (!prob_matrix[a][b] && b != j);
                        matrix[i][j].right = &matrix[i][b];

                        // Up pointer
                        a = i;
                        b = j;
                        do
                        {
                            a = get_up(a);
                        } while (!prob_matrix[a][b] && a != i);
                        matrix[i][j].up = &matrix[a][j];

                        // Down pointer
                        a = i;
                        b = j;
                        do
                        {
                            a = get_down(a);
                        } while (!prob_matrix[a][b] && a != i);
                        matrix[i][j].down = &matrix[a][j];
                    }
                }
            }

            // link header right pointer to column
            // header of first column
            header->right = &matrix[0][0];

            // link header left pointer to column
            // header of last column
            header->left = &matrix[0][n_col - 1];

            matrix[0][0].left = header;
            matrix[0][n_col - 1].right = header;
        }

        /**
         * @brief Covers the given node completely.
         * @param target_node Node to be covered.
         */
        void cover(struct Node *target_node)
        {
            struct Node *row, *right_node;

            // get the pointer to the header of column
            // to which this node belong
            struct Node *col_node = target_node->column;

            // unlink column header from it's neighbors
            col_node->left->right = col_node->right;
            col_node->right->left = col_node->left;

            // Move down the column and remove each row
            // by traversing right
            for (row = col_node->down; row != col_node; row = row->down)
            {
                for (right_node = row->right; right_node != row;
                     right_node = right_node->right)
                {
                    right_node->up->down = right_node->down;
                    right_node->down->up = right_node->up;

                    // after unlinking row node, decrement the
                    // node count in column header
                    matrix[0][right_node->col_id].node_count -= 1;
                }
            }
        }

        /**
         * @brief Uncovers the given node completely.
         * @param target_node Node to be uncovered.
         */
        void uncover(struct Node *target_node)
        {
            struct Node *row_node, *left_node;

            // get the pointer to the header of column
            // to which this node belong
            struct Node *col_node = target_node->column;

            // Move down the column and link back
            // each row by traversing left
            for (row_node = col_node->up; row_node != col_node; row_node = row_node->up)
            {
                for (left_node = row_node->left; left_node != row_node;
                     left_node = left_node->left)
                {
                    left_node->up->down = left_node;
                    left_node->down->up = left_node;

                    // after linking row node, increment the
                    // node count in column header
                    matrix[0][left_node->col_id].node_count += 1;
                }
            }

            // link the column header from it's neighbors
            col_node->left->right = col_node;
            col_node->right->left = col_node;
        }

        /**
         * @brief Gets the column with the minimum node count.
         * @return Pointer to the column header.
         */
        Node *get_min_column()
        {
            struct Node *h = header;
            struct Node *min_col = h->right;
            h = h->right->right;
            do
            {
                if (h->node_count < min_col->node_count)
                {
                    min_col = h;
                }
                h = h->right;
            } while (h != header);

            return min_col;
        }

        /**
         * @brief Adds the current solution to the solutions vector.
         */
        void add_solution()
        {
            std::set<int> solutions_set;
            for (auto i = solutions.begin(); i != solutions.end(); i++)
            {
                solutions_set.insert(((*i)->row_id) - 1);
            }

            solutions_vector.push_back(solutions_set);
        }

        /**
         * @brief Prints the current solution.
         */
        void print_solution()
        {
            std::cout << "Printing Solutions: ";
            std::vector<struct Node *>::iterator i;

            for (auto i = solutions.begin(); i != solutions.end(); i++)
                std::cout << (*i)->row_id << " ";
            std::cout << "\n";
        }

        /**
         * @brief Searches for exact covers recursively.
         * @param k Current level in the search.
         */
        void search(int k)
        {
            struct Node *row_node;
            struct Node *right_node;
            struct Node *left_node;
            struct Node *column;

            // if no column left, then we must
            // have found the solution
            if (header->right == header)
            {
                // print_solution();
                add_solution();
                return;
            }

            // choose column deterministically
            column = get_min_column();

            // cover chosen column
            cover(column);

            for (row_node = column->down; row_node != column;
                 row_node = row_node->down)
            {
                solutions.push_back(row_node);

                for (right_node = row_node->right; right_node != row_node;
                     right_node = right_node->right)
                    cover(right_node);

                // move to level k+1 (recursively)
                search(k + 1);

                // if solution in not possible, backtrack (uncover)
                // and remove the selected row (set) from solution
                solutions.pop_back();

                column = row_node->column;
                for (left_node = row_node->left; left_node != row_node;
                     left_node = left_node->left)
                    uncover(left_node);
            }

            uncover(column);
        }

        /**
         * @brief Finds all exact covers for the given sets.
         * @param sets Vector of sets representing the problem.
         * @return Vector of sets representing unique solutions.
         */
        std::vector<std::set<int>> find_exact_covers(std::vector<std::set<int>> sets)
        {
            std::set<int> merged_set;
            for (std::set<int> set : sets)
            {
                merged_set.insert(set.begin(), set.end());
            }

            n_col = 14;
            n_row = sets.size() + 1;

            // Initialize the problem matrix with headers 1.
            for (int i = 0; i <= n_row; i++)
            {
                for (int j = 0; j < n_col; j++)
                {
                    if (i == 0)
                        prob_matrix[i][j] = true;
                    else
                        prob_matrix[i][j] = false;
                }
            }

            // Populate the problem matrix
            int current_row = 1;
            for (std::set subset : sets)
            {
                for (int value : subset)
                {
                    prob_matrix[current_row][value] = true;
                }
                current_row++;
            }

            create_toridol_matrix();

            search(0);

            return solutions_vector;
        }
    };
}
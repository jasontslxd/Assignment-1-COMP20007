/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */

#ifndef PARKRANGER_H
#define PARKRANGER_H

#include <stdbool.h>
#include "deque.h"

// This function must read in a ski slope map and determine whether or not
// it is possible for the park ranger to trim all of the trees on the ski slope
// in a single run starting from the top of the mountain.
//
// The ski slope map is provided via stdin in the following format:
//
//   n m
//   from to
//   from to
//   ...
//   from to
//
// Here n denotes the number of trees that need trimming, which are labelled
// {1, ..., n}. The integer m denotes the number "reachable pairs" of trees.
// There are exactly m lines which follow, each containing a (from, to) pair
// which indicates that tree `to` is directly reachable from tree `from`.
// `from` and `to` are integers in the range {0, ..., n}, where {1, ..., n}
// denote the trees and 0 denotes the top of the mountain.
//
// For example the following input represents a ski slope with 3 trees and
// 4 reachable pairs of trees.
//
// input:            map:          0
//   3 4                          / \
//   0 1                         /  1
//   0 2                        / /
//   1 2                        2
//   2 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 1, 2, 3).
//
// Your function should must:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.

// The node that represents the top of the mountain
#define TOP_OF_MOUNTAIN 0

// The graph which will contain all our nodes using adjaceny lists
typedef struct graph Graph;

struct graph{
  int num_vertices;
  int num_edges;
  Node **adjacency_list;
};

bool is_single_run_possible();

// TODO: Add any additional functions or types required to solve this problem.

// Creates an empty graph with an empty adjacency list to store n vertices
Graph* create_graph(Data vertices, Data edges);

// Frees the graph and the nodes in its adjacency list
void free_graph(Graph* graph);

// Frees the topologically sorted list
void free_top_sort(Node* top_sort);

// Reads and stores the list of vertices from the input files into its adjacency list
// and returns a pointer to the graph. Includes the top of the mountain as a vertex
// in the graph.
Graph* read_graph();

// Depth first search algorithm for topological sorting based on the pseudocode given in 
// COMP20007 lecture 7
int* DFS_top_sort(Graph* graph, Data source);

// Algorithm to explore neighbouring nodes, stores the vertices popped from the function call 
//stack in an array called popped_order
void DFS_explore(Graph* graph, Data vertex, int* mark_array, int* order, int* popped_order);

// Returns a linked list of topologically sorted nodes by reversing the popped order
Node* process_top_sort(int* array, int size);

// Checks whether there is a single path connecting all nodes by checking whether edges exist
// in the topological sort
bool check_top_sort(Graph* graph, Node* top_sorted);

#endif

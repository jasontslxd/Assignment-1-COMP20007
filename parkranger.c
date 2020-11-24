/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parkranger.h"
#include "util.h"

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
//   0 1                         /  2
//   0 2                        / /
//   2 1                        1
//   1 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 2, 1, 3).
//
// Your function should must:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.
bool is_single_run_possible() {
  Graph* graph = read_graph();
  int* to_be_sorted = DFS_top_sort(graph, TOP_OF_MOUNTAIN);
  Node* top_sorted = process_top_sort(to_be_sorted, graph->num_vertices);
  bool is_possible = check_top_sort(graph, top_sorted);
  free_graph(graph);
  free(to_be_sorted);
  free_top_sort(top_sorted);
  return is_possible;
}

// TODO: Add any additional functions or types required to solve this problem.

// Creates an empty graph with an empty adjacency list to store n vertices
Graph* create_graph(Data vertices, Data edges){
  Graph* graph = malloc(sizeof(Graph));
  assert(graph);

  graph->num_vertices = vertices;
  graph->num_edges = edges;
  graph->adjacency_list = malloc(vertices * sizeof(Node));
  assert(graph->adjacency_list);

  int i;
  for (i = 0; i < vertices; i++){
    graph->adjacency_list[i] = NULL;
  }

  return graph;
}

// Frees the graph and the nodes in its adjacency list
void free_graph(Graph* graph){
  int i = 0;
  Node* tmp = graph->adjacency_list[i];

  while(i < graph->num_vertices){
    if(tmp != NULL){
      tmp = tmp->next;
      free_node(graph->adjacency_list[i]);
      graph->adjacency_list[i] = tmp;
    }
    else{
      free_node(graph->adjacency_list[i]);
      i++;
      tmp = graph->adjacency_list[i];
    }
  }
  free(graph);
}

// Frees the topologically sorted list
void free_top_sort(Node* top_sort){ 
  Node* tmp = top_sort;
  while(tmp != NULL){
    tmp = tmp->next;
    free_node(top_sort);
    top_sort = tmp;
  }
}

// Reads and stores the list of vertices from the input files into its adjacency list
// and returns a pointer to the graph. Includes the top of the mountain as a vertex
// in the graph.
Graph* read_graph(){
  int num_vertices = 0, num_edges = 0, curr_vertex = 0, adj_vertex = 0, i;
  scanf("%d%d", &num_vertices, &num_edges);
  Graph* graph = create_graph(num_vertices + 1, num_edges);

  // Read every edge of a vertex into an adjacency list
  for (i = 0; i < num_edges; i++){
    scanf("%d%d", &curr_vertex, &adj_vertex);
    Node* curr_node = new_node(adj_vertex);
    curr_node->next = graph->adjacency_list[curr_vertex];
    graph->adjacency_list[curr_vertex] = curr_node;
  }

  return graph;
}

// Depth first search algorithm for topological sorting based on the pseudocode given in 
// COMP20007 lecture 7
int* DFS_top_sort(Graph* graph, Data source){
  int* mark_array = calloc(graph->num_vertices, sizeof(int));
  int* popped_order = calloc(graph->num_vertices, sizeof(int));
  assert(mark_array);
  assert(popped_order);
  int order = 0, i;

  // Calls DFS_explore to recursively explore each unexplored node
  for (i = 0; i < graph->num_vertices; i++){
    if (mark_array[i] == 0){
      DFS_explore(graph, i, mark_array, &order, popped_order);
    }
  }

  free(mark_array);
  return popped_order;
}

// Algorithm to explore neighbouring nodes, stores the vertices popped from the function call 
// stack in an array called popped_order
void DFS_explore(Graph* graph, Data vertex, int* mark_array, int* order, int* popped_order){
  mark_array[vertex] = 1;
  Node* tmp = graph->adjacency_list[vertex];

  // Checks each edge in the adjaceny list, explores a node if it has not been visited
  while (tmp != NULL){
    if(mark_array[tmp->data] == 0){
      DFS_explore(graph, tmp->data, mark_array, order, popped_order);
    }
    tmp = tmp->next;
  }
  popped_order[*order] = vertex;
  *order += 1;
}

// Returns a linked list of topologically sorted nodes by reversing the popped order
Node* process_top_sort(int* array, int size){
  int i;
  Node* top_sort = NULL;
  for (i = 0; i < size; i++){
    Node* curr_vertex = new_node(array[i]);
    curr_vertex->next = top_sort;
    top_sort = curr_vertex;
  }
  return top_sort;
}

// Checks whether there is a single path connecting all nodes by checking whether edges in the topological
// sort also exists in the graph
bool check_top_sort(Graph* graph, Node* top_sorted){
  // A path wthat visits n vertices must be connected by n-1 edges
  int edges_connecting_all = graph->num_vertices - 1, count = 0;
  Node** adj = graph->adjacency_list;

  // Check if we have reached the end of topological sorted list and adjacency list
  while(top_sorted != NULL && adj[top_sorted->data] != NULL){
    // If edge exists in both lists
    if(adj[top_sorted->data]->data == top_sorted->next->data){
      top_sorted = top_sorted->next;
      count++;
    }

    else{
      adj[top_sorted->data] = adj[top_sorted->data]->next;
    }
  }

  if (count == edges_connecting_all){
    return true;
  }
  else{
    return false;
  }
}
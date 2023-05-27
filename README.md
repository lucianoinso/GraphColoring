# GraphColoring
Program that takes an input graph in DIMACS format (can have millions of edges and vertices), and effienctly provides a greedy coloring.

It also includes several functions for reordering the vertices colouring order (a good greedy coloring efficacy is hugely tied to the processing order of the vertices).

# Implementation
The solution was challenging to implement, it doesn't use matrix adjacency (it's unfeasable for large numbers of vertices/edges), it had to take into account memory usage and efficiency.

It includes a personal implementation from scratch of a *hash table*, with extra functions that are useful for this project so it can finish in seconds even when the input is a graph with millions of vertices and/or edges.

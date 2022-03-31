# GraphColoring
Program that takes an input graph in DIMACS format (can have millions of edges and vertices), and fastly provides a greedy coloring.

It also includes several functions to reorder the vertices so that you can color them in many ways (greedy coloring efficacy is hugely tied to the vertices order in which you feed the algorithm).

# Implementation
The code was a challenge to implement, it had to take into account memory usage and efficient running.

It includes a personal implementation from zero of a *hash table*, adapted to this project so it can finish in seconds even when the input is a graph with millions of vertices and/or edges.

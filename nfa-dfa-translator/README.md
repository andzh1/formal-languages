# How to use this translator

## Input format:
- First string — n (number of vectices) and m (number of sequences).
- Then m strings — sequences of tuples (int, int, char) — indexes of vertices and letter on edge between them.
- Then int k, and k indexes of terminate vertices in random order.    
- Last number — index of starting vertice.


## Output format
Outputs all edges of DFA, that is equivalent to given NFA. Also outputs all terminate vertices of DFA. All vertices are given as sets of vertices from NFA.


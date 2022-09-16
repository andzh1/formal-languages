#include <bits/stdc++.h>
/*
Input format: n - number of vectices, m - number ofsequences.
Then n sequences of tuples (int, int, char) - indexes of vertices and letter on edge between them;
Then int k, and k indexes of terminate vertices in random order.    
Last number - index of starting vertice (by default = 0)
*/

const std::vector<char> LETTERS({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
                                'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
                                'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});

typedef uint64_t set;
bool contains(set set, int index);


struct Edge {
    set start;
    set end;
    char letter;
};


class FiniteAutomation
{
protected:
    struct Vertice {
        std::vector<Edge> _edges;
        bool _is_terminate = false;
    };
    int index_of_starting_vertice = 0;
    FiniteAutomation(int index = 0);
};

class DeterministicFiniteAutomation : protected FiniteAutomation {
    std::map<set, Vertice> _vertices;
public:
    DeterministicFiniteAutomation();
    
    DeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0);
    bool contains_vertice(const set& set) const;
    void insert_vertice(const set& set, const Vertice& vertice = Vertice());
    
    friend std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomation& dfa);
};

std::string set_as_string(const set& set_to_print);

std::ostream& operator<<(std::ostream& out, const Edge& edge);

std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomation& dfa);

class NondeterministicFiniteAutomation : protected FiniteAutomation {
public:
    std::vector<Vertice> _vertices;
    NondeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0);

    size_t size() const;

private:
    bool is_terminate(const set& set) const;

public:
    /* Converts nondeterministic finite automaton to a deterministic one; 
    it correctly and fastly works for automations of size <= 60.
    We also assume that letter on edges are English lowercase letters, 
    but you can set it by changing constant LETTERS*/
    DeterministicFiniteAutomation convert_to_DFA() const;
};

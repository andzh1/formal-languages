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
bool contains(set set, int index) {
    return set & (1 << index);
}


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
    FiniteAutomation(int index = 0): index_of_starting_vertice(index) {}
};

class DeterministicFiniteAutomation : protected FiniteAutomation {
    std::map<set, Vertice> _vertices;
public:
    DeterministicFiniteAutomation(): FiniteAutomation() {}
    
    DeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0):
    FiniteAutomation(index_of_starting_vertice) {
        for (const Edge& e : edges) {
            _vertices[e.start]._edges.push_back(e);
        }
        for (const int& index : indexes_of_terminate_vertices) {
            _vertices[index]._is_terminate = true;
        }
    }

    bool contains_vertice(const set& set) const {
        return _vertices.find(set) != _vertices.end();
    }

    void insert_vertice(const set& set, const Vertice& vertice = Vertice()) {
        _vertices[set] = vertice;
    }

    friend std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomation& dfa);
};

std::string set_as_string(const set& set_to_print) {
    int ip = 0;
    std::string result = "{ ";
    while ((1 << ip) <= set_to_print) {
        if (set_to_print & (1 << ip)) result += std::to_string(ip) + " ";
        ++ip;
    }
    result += "}";
    return result;
}

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
    out << set_as_string(edge.start);
    out << " -> " << edge.letter << " -> ";
    out << set_as_string(edge.end);
    return out;
}

std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomation& dfa) {
    out << "Edges:\n";
    for (const auto& vertice : dfa._vertices) {
        for (const Edge& edge : vertice.second._edges) {
            out << edge << '\n';
        }
    }
    out << "\n Terminate vertices:\n";
    for (const auto& vertice : dfa._vertices) {
        if (vertice.second._is_terminate) out << set_as_string(vertice.first);
    }
    return out;
}

class NondeterministicFiniteAutomation : protected FiniteAutomation {
public:
    std::vector<Vertice> _vertices;
    NondeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0):
    FiniteAutomation(index_of_starting_vertice) {
        _vertices.resize(number_of_vertices);
        for (const Edge& e : edges) {
            _vertices[e.start]._edges.push_back(e);
        }
        for (const int& index : indexes_of_terminate_vertices) {
            _vertices[index]._is_terminate = true;
        }
    }

    size_t size() const {
        return _vertices.size();
    }

private:
    bool is_terminate(const set& set) const {
        for (int i = 0; i < _vertices.size(); ++i) {
            if (contains(set, i) && _vertices[i]._is_terminate) return true;
        }
        return false;
    }

public:
    /* Converts nondeterministic finite automaton to a deterministic one; 
    it correctly and fastly works for automations of size <= 60.
    We also assume that letter on edges are English lowercase letters, 
    but you can set it by changing constant LETTERS*/
    DeterministicFiniteAutomation convert_to_DFA() const {
        DeterministicFiniteAutomation equivalent_DFA;
        std::queue<set> indexes_of_vertices_to_proceed;
        indexes_of_vertices_to_proceed.push(1 << index_of_starting_vertice);
        while (!indexes_of_vertices_to_proceed.empty()) {
            set current_set = indexes_of_vertices_to_proceed.front();
            indexes_of_vertices_to_proceed.pop();
            std::vector<Edge> new_edges (LETTERS.size());
            for (int i = 0; i < new_edges.size(); ++i) {
                new_edges[i].letter = LETTERS[i];
                new_edges[i].start = current_set;
                new_edges[i].end = 0;
            }
            for (const Vertice& vectice : _vertices) {
                for (const Edge& edge : vectice._edges) {
                    if (contains(current_set, edge.start)) {
                        new_edges[edge.letter - 'a'].end |= (1 << edge.end); // TODO: Fix <edge.letter - 'a'> - need to get index somehow else
                        
                    }
                }
            }
            for (const auto& edge : new_edges) {
                if (!equivalent_DFA.contains_vertice(edge.end) && edge.end) {
                    Vertice new_vertice;
                    new_vertice._is_terminate = is_terminate(edge.end);
                    indexes_of_vertices_to_proceed.push(edge.end);
                }
            }
            Vertice vertice_to_insert;
            for (int i = 0; i < new_edges.size(); ++i) {
                if (new_edges[i].end) vertice_to_insert._edges.push_back(new_edges[i]);
            }
            vertice_to_insert._is_terminate = is_terminate(current_set);
            equivalent_DFA.insert_vertice(current_set, vertice_to_insert);
        }
        return equivalent_DFA;
    }

};


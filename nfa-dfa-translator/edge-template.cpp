#include <bits/stdc++.h>
#include <type_traits>
/*
Input format: n - number of vectices, m - number of sequences.
Then n sequences of tuples (int, int, std::string) - indexes of vertices and word on edge between them;
if your word is empty, you should input it as "#".
Then int k, and k indexes of terminate vertices in random order.    
Last number - index of starting vertice (by default = 0)
*/

typedef uint64_t set;
bool contains(set set, int index) {
    return set & (1 << index);
}

typedef std::string word_t;
typedef set DFA_vertex_t;
typedef int NFA_vertex_t;

template<typename vertex_t>
struct OtherTypeClass {
    // typedef ((std::is_same_v<vertex_t, DFA_vertex_t>)? NFA_vertex_t : DFA_vertex_t) value;
    using value = NFA_vertex_t;
};

template<>
struct OtherTypeClass<DFA_vertex_t> {
    using value = NFA_vertex_t;
};

template<>
struct OtherTypeClass<NFA_vertex_t> {
    using value = DFA_vertex_t;
};

template<typename vertex_t>
using OtherType = typename OtherTypeClass<vertex_t>::value;

template<typename vertex_t>
struct Edge {
    vertex_t start;
    vertex_t end;
    word_t word;
};

using NFA_edge_t = Edge<NFA_vertex_t>;
using DFA_edge_t = Edge<DFA_vertex_t>;

template<typename vertex_t>
class FiniteAutomata
{
private:
    typedef Edge<vertex_t> current_edge_t;

    struct Vertice {
        std::vector<current_edge_t> _edges;
        bool _is_terminate = false;
    };
    int index_of_starting_vertice = 0;
    std::set<word_t> _letters;
    std::map<word_t, vertex_t> index_of_letter;
    std::map<vertex_t, Vertice> _vertices;
public:
    FiniteAutomata(int index = 0): index_of_starting_vertice(index) {}
    
private:
    void _get_letters(const std::vector<current_edge_t>& edges, int index = 0) {
        index_of_starting_vertice = index;
        for (const current_edge_t& e : edges) {
            _letters.insert((e.word));
        }
        auto iter_in_letters = _letters.begin();
        int index_of_current_letter = 0;
        while (iter_in_letters != _letters.end()) {
            index_of_letter[*iter_in_letters] = index_of_current_letter;
            ++index_of_current_letter; ++iter_in_letters;
        }
    }

    void normalize_one_edge(const current_edge_t& edge_to_normalize);

    bool is_terminate(const DFA_vertex_t& dfa_vertex) const;
public:
   
    /* Converts nondeterministic finite automaton to a deterministic one; 
it correctly works for Automatas of size <= 60.*/
    FiniteAutomata<DFA_vertex_t> convert_to_DFA() const;

    bool contains_vertice(const vertex_t& vertex_index) const {
        return _vertices.find(vertex_index) != _vertices.end();
    }

    void insert_vertice(const vertex_t& vertex_index, const Vertice& vertice = Vertice()) {
        _vertices[vertex_index] = vertice;
    }
    
    size_t size() const {
        return _vertices.size();
    }

    FiniteAutomata(const std::vector<current_edge_t>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0) {
        _get_letters(edges, index_of_starting_vertice);
        for (const current_edge_t& e : edges) {
            _vertices[e.start]._edges.push_back(e);
        }
        for (const int& index : indexes_of_terminate_vertices) {
            _vertices[index]._is_terminate = true;
        }
    }

public:

    /* Makes length of word on each edge = 1 (and new NFA is equal to given one). */
    void normalize();
    
    /*Returns normalized copy of this NFA*/
    FiniteAutomata<vertex_t> normalized() const;

    friend class FiniteAutomata<OtherType<vertex_t>>;

    template<class T>
    friend std::ostream& operator<<(std::ostream& out, const FiniteAutomata<T>& fa);
};

using NondeterministicFiniteAutomata = FiniteAutomata<NFA_vertex_t>;
using DeterministicFiniteAutomata = FiniteAutomata<DFA_vertex_t>;

template<typename V>
std::string set_as_string(const V& set_to_print);

template<>
std::string set_as_string(const NFA_vertex_t& vertex_to_print) {
    int ip = 0;
    std::string result = "{ ";
    result += std::to_string(vertex_to_print) + "}";
    return result;
}

template<>
std::string set_as_string(const DFA_vertex_t& vertex_to_print) {
    int ip = 0;
    std::string result = "{ ";
    while ((1 << ip) <= vertex_to_print) {
        if (vertex_to_print & (1 << ip)) result += std::to_string(ip) + " ";
        ++ip;
    }
    result += "}";
    return result;
}

template<typename V>
std::ostream& operator<<(std::ostream& out, const Edge<V>& edge) {
    out << set_as_string(edge.start);
    out << " -> " << edge.word << " -> ";
    out << set_as_string(edge.end);
    return out;

}


template<typename vertex_t>
std::ostream& operator<<(std::ostream& out, const FiniteAutomata<vertex_t>& fa) {
    out << "Edges:\n";
    for (const auto& vertice : fa._vertices) {
        for (const Edge<vertex_t>& edge : vertice.second._edges) {
            out << edge << '\n';
        }
    }
    out << "\nTerminate vertices:\n";
    for (const auto& vertice : fa._vertices) {
        if (vertice.second._is_terminate) out << set_as_string(vertice.first);
    }
    return out;
}


template<>
bool FiniteAutomata<NFA_vertex_t>::is_terminate(const DFA_vertex_t& dfa_vertex) const {
    for (const auto& v : _vertices) {
        if (contains(dfa_vertex, v.first) && v.second._is_terminate) return true;
    }
    return false;
}

template<>
DeterministicFiniteAutomata NondeterministicFiniteAutomata::convert_to_DFA() const {
    DeterministicFiniteAutomata equivalent_DFA;
    std::queue<set> indexes_of_vertices_to_proceed;
    indexes_of_vertices_to_proceed.push(1 << index_of_starting_vertice);
    while (!indexes_of_vertices_to_proceed.empty()) {
        set current_set = indexes_of_vertices_to_proceed.front();
        indexes_of_vertices_to_proceed.pop();
        std::vector<DFA_edge_t> new_edges (_letters.size());
        auto iter_in_letters = _letters.begin();
        for (int i = 0; i < new_edges.size(); ++i) {
            new_edges[i].word = *iter_in_letters;
            ++iter_in_letters;
            new_edges[i].start = current_set;
            new_edges[i].end = 0;
        }
        for (auto const& [_, vertice] : _vertices) {
            for (const NFA_edge_t& edge : vertice._edges) {
                if (contains(current_set, edge.start)) {
                    new_edges[index_of_letter.at(edge.word)].end |= (1 << edge.end); 
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
        DeterministicFiniteAutomata::Vertice vertice_to_insert;
        for (int i = 0; i < new_edges.size(); ++i) {
            if (new_edges[i].end) vertice_to_insert._edges.push_back(new_edges[i]);
        }
        vertice_to_insert._is_terminate = is_terminate(current_set);
        equivalent_DFA.insert_vertice(current_set, vertice_to_insert);
    }
    return equivalent_DFA;
}

template<typename vertex_t>
void FiniteAutomata<vertex_t>::normalize_one_edge(const Edge<vertex_t>& edge_to_normalize) {
    set current_start = edge_to_normalize.start;
    set current_end = _vertices.size();
    for (int i = 0; i < edge_to_normalize.word.size() - 1; ++i) {
        _vertices.insert(Vertice());
        Edge<vertex_t> current_edge({current_start, current_end, edge_to_normalize.word.substr(i, 1)});
        _vertices[current_start]._edges.push_back(current_edge);
        current_start = current_end;
        ++current_end;
    }
    Edge<vertex_t> last_edge({current_start, edge_to_normalize.end, 
    edge_to_normalize.word.substr(edge_to_normalize.word.size() - 1, 1)});
    _vertices[current_start]._edges.push_back(last_edge);
}

template<typename vertex_t>
void FiniteAutomata<vertex_t>::normalize() {
    std::vector<Edge<vertex_t>> edges;
    for (auto& v : _vertices) {
        for (const Edge<vertex_t>& e : v._edges) {
            edges.push_back(e);
        }
        v._edges.clear();
    }
    for (const Edge<vertex_t>& e : edges) {
        normalize_one_edge(e);
    }
}

template<typename vertex_t>
FiniteAutomata<vertex_t> FiniteAutomata<vertex_t>::normalized() const {
    auto copy = *this;
    copy.normalize();
    return copy;
}


signed main() {
    freopen("tests.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int number_of_vertices, number_of_edges;
    std::cin >> number_of_vertices >> number_of_edges;

    std::vector<NFA_edge_t> edges(number_of_edges);
    for (NFA_edge_t& edge : edges) {
        std::cin >> edge.start >> edge.end >> edge.word;
    }
    int number_of_terminate_vertices;
    std::cin >> number_of_terminate_vertices;
    std::vector<int> terminates (number_of_terminate_vertices);
    for (int& index : terminates) std::cin >> index;
    int index_of_starting_vertice;
    std::cin >> index_of_starting_vertice;
    NondeterministicFiniteAutomata hfa(edges, number_of_vertices, terminates, index_of_starting_vertice);
    auto dfa = hfa.convert_to_DFA();
    std::cout << dfa;
}
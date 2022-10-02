#include <bits/stdc++.h>
#include <type_traits>
/*
Input format: n - number of vectices, m - number of sequences.
Then n sequences of tuples (int, int, std::string) - indexes of vertices and word on edge between them;
if your word is empty, you should input it as "#".
Then int k, and k indexes of terminate vertices in random order.    
Last number - index of starting vertice (by default = 0)
*/

bool is_epsilon_transition(const std::string& word) {
    return word == "#";
}

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
struct IsDfaClass {
    // typedef ((std::is_same_v<vertex_t, DFA_vertex_t>)? NFA_vertex_t : DFA_vertex_t) value;
    static const bool value = false;
};

template<>
struct IsDfaClass<DFA_vertex_t> {
    static const bool value = true;
};


template<typename vertex_t>
using OtherType = typename OtherTypeClass<vertex_t>::value;

template<typename vertex_t>
bool IsDFA = IsDfaClass<vertex_t>::value;



template<typename vertex_t>
struct Edge {
    vertex_t start;
    vertex_t end;
    word_t word;
};

using NFA_edge_t = Edge<NFA_vertex_t>;
using DFA_edge_t = Edge<DFA_vertex_t>;

template<typename vertex_t> class FiniteAutomata;
using NondeterministicFiniteAutomata = FiniteAutomata<NFA_vertex_t>;
using DeterministicFiniteAutomata = FiniteAutomata<DFA_vertex_t>;


template<typename vertex_t>
class FiniteAutomata
{
    typedef Edge<vertex_t> current_edge_t;

    struct Vertice {
        std::vector<current_edge_t> _edges;
        bool _is_terminate = false;
    };
private:
    int index_of_starting_vertice = 0;
    std::set<word_t> _letters;
    std::map<word_t, int> index_of_letter;
    std::map<vertex_t, Vertice> _vertices;
public:
    FiniteAutomata(int index = 0): index_of_starting_vertice(index) {}
    
private:
    void _get_letters(const std::vector<current_edge_t>& edges, int index = 0);

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
    
    std::map<vertex_t, Vertice> get_vertices() const {
        return _vertices;
    }

    size_t size() const {
        return _vertices.size();
    }

    FiniteAutomata(const std::vector<current_edge_t>& edges, int number_of_vertices, 
    const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice = 0);

public:

    /* Makes length of word on each edge = 1 (and new NFA is equal to given one). */
    void normalize();
    
    /*Returns normalized copy of this NFA*/
    FiniteAutomata<vertex_t> normalized() const;

    friend class FiniteAutomata<OtherType<vertex_t>>;

    template<class T>
    friend std::ostream& operator<<(std::ostream& out, const FiniteAutomata<T>& fa);
private:
    // Returns vertives that can be reached from start only by epsilon transitions.
    bool reachable_by_epsilon_transitions(vertex_t start, std::vector<bool>& visited) const;

    vertex_t min_suitable_index_of_vertice() const;
public:
    // Builds equivalent finite automation, where there are no epsilon transitions.
    void remove_epsilon_transitions();

    // Builds equivalent complete (Full) DFA. That works for DFA only.
    void make_fdfa();

    // Returns fdfa that is equivalent to this one.
    DeterministicFiniteAutomata fdfa_copy() const;

    /* Builds complement DFA (if word W is accepted by DFA, 
    it isn't accepted by complement one, and vice versa).
    You can also give a hint to program that it doesn't need 
    to make your dfa complete, by passing true as argument.*/
    void make_complement_fdfa(bool is_fdfa = false);

    // Returns complement DFA.
    DeterministicFiniteAutomata complemented_fdfa(bool is_fdfa = false) const;

    void make_minimal_fdfa(); // TODO:

    void minimal_fdfa() const;

    // TODO: Regular expressions
};

template<typename V>
std::string vertice_as_string(const V& set_to_print);

template<>
std::string vertice_as_string(const NFA_vertex_t& vertex_to_print) {
    int ip = 0;
    std::string result = "{";
    result += std::to_string(vertex_to_print) + "}";
    return result;
}

template<>
std::string vertice_as_string(const DFA_vertex_t& vertex_to_print) {
    int ip = 0;
    std::string result = "{";
    while ((1 << ip) <= vertex_to_print) {
        if (vertex_to_print & (1 << ip)) result += std::to_string(ip) + " ";
        ++ip;
    }
    result += "}";
    return result;
}

template<typename V>
std::ostream& operator<<(std::ostream& out, const Edge<V>& edge) {
    out << vertice_as_string(edge.start);
    out << " -> " << edge.word << " -> ";
    out << vertice_as_string(edge.end);
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
        if (vertice.second._is_terminate) out << vertice_as_string(vertice.first);
    }
    return out;
}


template<typename vertex_t>
void FiniteAutomata<vertex_t>::_get_letters(const std::vector<current_edge_t>& edges, int index) {
    index_of_starting_vertice = index;
    for (const current_edge_t& e : edges) {
        _letters.insert(e.word);
    }
    auto iter_in_letters = _letters.begin();
    int index_of_current_letter = 0;
    while (iter_in_letters != _letters.end()) {
        index_of_letter[*iter_in_letters] = index_of_current_letter;
        ++index_of_current_letter; ++iter_in_letters;
    }
}

template<typename vertex_t>
FiniteAutomata<vertex_t>::FiniteAutomata(const std::vector<current_edge_t>& edges, int number_of_vertices, 
const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice) {
    _get_letters(edges, index_of_starting_vertice);
    for (const current_edge_t& e : edges) {
        _vertices[e.start]._edges.push_back(e);
        if (!contains_vertice(e.end)) _vertices[e.end] = Vertice();
    }
    for (const int& index : indexes_of_terminate_vertices) {
        _vertices[index]._is_terminate = true;
    }
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
    equivalent_DFA.index_of_letter = index_of_letter;
    equivalent_DFA._letters = _letters;
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
    vertex_t current_start = edge_to_normalize.start;
    vertex_t current_end = _vertices.size();
    current_end = min_suitable_index_of_vertice(); 
    for (int i = 0; i < edge_to_normalize.word.size() - 1; ++i) {
        _vertices[current_end] = Vertice();
        Edge<vertex_t> current_edge({current_start, current_end, edge_to_normalize.word.substr(i, 1)});
        _vertices[current_start]._edges.push_back(current_edge);
        current_start = current_end;
        ++current_end;
    }
    Edge<vertex_t> last_edge({current_start, edge_to_normalize.end, 
    edge_to_normalize.word.substr(edge_to_normalize.word.size() - 1, 1)});
    _vertices[current_start]._edges.push_back(last_edge);
    _letters.erase(edge_to_normalize.word);
    for (int i = 0; i < edge_to_normalize.word.size(); ++i) {
        _letters.insert(edge_to_normalize.word.substr(i, 1));
    }
}

template<typename vertex_t>
void FiniteAutomata<vertex_t>::normalize() {
    std::vector<Edge<vertex_t>> edges;
    for (auto& v : _vertices) {
        for (const Edge<vertex_t>& e : v.second._edges) {
            edges.push_back(e);
        }
        v.second._edges.clear();
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

template<typename vertex_t>
bool FiniteAutomata<vertex_t>::reachable_by_epsilon_transitions
(vertex_t start, std::vector<bool>& visited) const {
    bool reaches_terminate = _vertices[start]._is_terminate;
    for (const auto& e : _vertices[start]._edges) {
        if (visited[e.end]) continue;
        visited[e.end] = true;
        if (is_epsilon_transition(e.word)) {
            reaches_terminate |= reachable_by_epsilon_transitions(e.end, visited);
        }
    }
    return reaches_terminate;
}

template<typename vertex_t>
void FiniteAutomata<vertex_t>::remove_epsilon_transitions() {
    std::vector<std::vector<bool>> is_reachable_by_eps_transition
    (_vertices.size(), std::vector<bool>(_vertices.size()));
    int number_of_vertice = 0;
    for (auto& [index, vertice]: _vertices) {
        reachable_by_epsilon_transitions(index, is_reachable_by_eps_transition[number_of_vertice]);
        ++number_of_vertice;
    }
    number_of_vertice = 0;
    for (auto& [index, vertice]: _vertices) {
        for (int i = 0; i < _vertices.size(); ++i) {
            if (is_reachable_by_eps_transition[number_of_vertice][i]) {
                for (const auto& e : _vertices[i]._edges) {
                    vertice._edges.push_back(Edge<vertex_t>({index, e.end, e.word}));
                }
            }
        }
        ++number_of_vertice;
    }
    for (auto& [_, vertice]: _vertices) {
        std::vector<Edge<vertex_t>> edges_without_epsilons;
        for (const auto& e : vertice._edges) {
            if (!is_epsilon_transition(e.word)) edges_without_epsilons.push_back(e);
        }
        vertice._edges = edges_without_epsilons;
    }
}

template<> 
DFA_vertex_t DeterministicFiniteAutomata::min_suitable_index_of_vertice() const {
    DFA_vertex_t answer = 0;
    for (auto const& [index, _] : _vertices) {
        answer = std::max(answer, index);
    }
    return answer + 1; 
}


template<> 
NFA_vertex_t NondeterministicFiniteAutomata::min_suitable_index_of_vertice() const {
    return _vertices.size(); 
}

template<>
void DeterministicFiniteAutomata::make_fdfa() {
    Vertice stock;
    DFA_vertex_t index_of_stock = min_suitable_index_of_vertice();
    for (const auto& w : _letters) {
        stock._edges.push_back(DFA_edge_t({index_of_stock, index_of_stock, w}));
    }
    normalize();
    bool is_already_fdfa = true;
    for (auto& [index, vertice] : _vertices) {
        std::vector<bool> has_edge_with (_letters.size());
        for (const auto& e : vertice._edges) {
            has_edge_with[index_of_letter[e.word]] = true; // FIXME:
        }
        auto current_word_ptr = _letters.begin();
        for (int i = 0; i < _letters.size() && current_word_ptr != _letters.end(); ++i, ++current_word_ptr) {
            if (has_edge_with[i]) continue;
            vertice._edges.push_back(DFA_edge_t({index, index_of_stock, *current_word_ptr}));
            is_already_fdfa = false;
        }
    }
    if (!is_already_fdfa) {
        _vertices[index_of_stock] = stock;
    }
}

template<>
DeterministicFiniteAutomata DeterministicFiniteAutomata::fdfa_copy() const {
    auto copy = *this;
    copy.make_fdfa();
    return copy;
}

template<>
void DeterministicFiniteAutomata::make_complement_fdfa(bool is_fdfa) {
    if (!is_fdfa) make_fdfa();
    for (auto& [_, vertice]: _vertices) {
        vertice._is_terminate = !vertice._is_terminate;
    }
}

template<>
DeterministicFiniteAutomata DeterministicFiniteAutomata::complemented_fdfa(bool is_fdfa) const {
    auto copy = *this;
    copy.make_complement_fdfa(is_fdfa);
    return copy;
}

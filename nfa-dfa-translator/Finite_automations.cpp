#include "Finite_automations.h"


typedef uint64_t set;
bool contains(set set, int index) {
    return set & (1 << index);
}




FiniteAutomation::FiniteAutomation(int index): index_of_starting_vertice(index) {}

DeterministicFiniteAutomation::DeterministicFiniteAutomation(): FiniteAutomation() {}

DeterministicFiniteAutomation::DeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_verticeFiniteAutomation):
FiniteAutomation(index_of_starting_vertice) {
    for (const Edge& e : edges) {
        _vertices[e.start]._edges.push_back(e);
    }
    for (const int& index : indexes_of_terminate_vertices) {
        _vertices[index]._is_terminate = true;
    }
}

bool DeterministicFiniteAutomation::contains_vertice(const set& set) const {
    return _vertices.find(set) != _vertices.end();
}

void DeterministicFiniteAutomation::insert_vertice(const set& set, const Vertice& vertice) {
    _vertices[set] = vertice;
}

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

NondeterministicFiniteAutomation::NondeterministicFiniteAutomation(const std::vector<Edge>& edges, int number_of_vertices, 
const std::vector<int>& indexes_of_terminate_vertices, int index_of_starting_vertice):
FiniteAutomation(index_of_starting_vertice) {
    _vertices.resize(number_of_vertices);
    for (const Edge& e : edges) {
        _vertices[e.start]._edges.push_back(e);
    }
    for (const int& index : indexes_of_terminate_vertices) {
        _vertices[index]._is_terminate = true;
    }
}


size_t NondeterministicFiniteAutomation::size() const {
    return _vertices.size();
}


bool NondeterministicFiniteAutomation::is_terminate(const set& set) const {
    for (int i = 0; i < _vertices.size(); ++i) {
        if (contains(set, i) && _vertices[i]._is_terminate) return true;
    }
    return false;
}

DeterministicFiniteAutomation NondeterministicFiniteAutomation::convert_to_DFA() const {
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

// signed main() {
//     freopen("tests.txt", "r", stdin);
//     // freopen("output.txt", "w", stdout);
//     int number_of_vertices, number_of_edges;
//     std::cin >> number_of_vertices >> number_of_edges;

//     std::vector<Edge> edges(number_of_edges);
//     for (Edge& edge : edges) {
//         std::cin >> edge.start >> edge.end >> edge.letter;
//     }
//     int number_of_terminate_vertices;
//     std::cin >> number_of_terminate_vertices;
//     std::vector<int> terminates (number_of_terminate_vertices);
//     for (int& index : terminates) std::cin >> index;
//     int index_of_starting_vertice;
//     std::cin >> index_of_starting_vertice;
//     NondeterministicFiniteAutomation hfa(edges, number_of_vertices, terminates, index_of_starting_vertice);
//     auto dfa = hfa.convert_to_DFA();
// }

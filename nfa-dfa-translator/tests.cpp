#include "Finite_automatas.h"

void Convert_To_DFA_Test() {
    freopen("tests.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int number_of_vertices, number_of_edges;
    std::cin >> number_of_vertices >> number_of_edges;

    std::vector<Edge> edges(number_of_edges);
    for (Edge& edge : edges) {
        std::cin >> edge.start >> edge.end >> edge.letter;
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


signed main() {
    int number_of_tests = 1;
    // std::cin >> number_of_tests;
    while(number_of_tests--) Convert_To_DFA_Test();
}
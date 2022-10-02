#include "Finite_automatas.cpp"
// #include "Finite_automatas.h"

#include "gtest/gtest.h"

TEST(OutputTest, TestVerticeAsString) {
    EXPECT_EQ("{2}", vertice_as_string<NFA_vertex_t>(2));
    EXPECT_EQ("{1 }", vertice_as_string<DFA_vertex_t>(2));
}


TEST(FunctionsTest, TestAll) {
    ASSERT_TRUE(is_epsilon_transition("#"));
    ASSERT_FALSE(is_epsilon_transition("."));
    ASSERT_TRUE(contains(2, 1));
    ASSERT_FALSE(contains(2, 4));
    NFA_edge_t a ({1, 2, "a"}), b({1, 2, "b"});
    ASSERT_TRUE(a < b);
    b.word = "a";
    a.start = 0;
    ASSERT_TRUE(a < b);
}

TEST(MinorMethodsTest, TestContains) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "a"}}, 3, {2}, 0);
    EXPECT_TRUE(nfa.contains_vertice(0));
    EXPECT_TRUE(nfa.contains_vertice(1));
    EXPECT_TRUE(nfa.contains_vertice(2));
    EXPECT_FALSE(nfa.contains_vertice(3));
    EXPECT_FALSE(nfa.contains_vertice(6));
    EXPECT_FALSE(nfa.contains_vertice(111));
}

TEST(MinorMethodsTest, TestInsert) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "a"}}, 3, {2}, 0);
    nfa.insert_vertice(3);
    EXPECT_TRUE(nfa.contains_vertice(3));
    EXPECT_TRUE(nfa.contains_vertice(0));
    EXPECT_TRUE(nfa.contains_vertice(1));
    EXPECT_TRUE(nfa.contains_vertice(2));
    EXPECT_FALSE(nfa.contains_vertice(4));
}

TEST(TestConvertion, TestAll) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "a"}}, 3, {2}, 0);
    auto dfa = nfa.convert_to_DFA();
    EXPECT_TRUE(dfa.contains_vertice(6));
    EXPECT_FALSE(dfa.contains_vertice(2));
    EXPECT_TRUE(dfa.contains_vertice(1));
    EXPECT_FALSE(dfa.contains_vertice(0));
}

TEST(TestNormalisation, TestDFA) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "abc"}}, 3, {2}, 0);
    auto dfa = nfa.convert_to_DFA();
    dfa.normalize();
    auto vertices = dfa.get_vertices();
    EXPECT_EQ(vertices.size(), 5);
    bool all_length_1 = true;
    for (const auto& v : vertices) {
        for (const auto& e : v.second._edges) {
            all_length_1 &= e.word.length() == 1;
        }
    }
    EXPECT_TRUE(all_length_1);
    EXPECT_FALSE(dfa.contains_vertice(0));
}

TEST(TestNormalisation, TestNFA) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "abc"}}, 3, {2}, 0);
    nfa.normalize();
    auto vertices = nfa.get_vertices();
    EXPECT_EQ(vertices.size(), 5);
    bool all_length_1 = true;
    for (const auto& v : vertices) {
        for (const auto& e : v.second._edges) {
            all_length_1 &= e.word.length() == 1;
        }
    }
    EXPECT_TRUE(all_length_1);
}


TEST(MakeFdfaTest, TestDFA) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {2}, 0);
    dfa.make_fdfa();
    auto vertices = dfa.get_vertices();
    EXPECT_EQ(vertices.size(), 4);
    bool is_full = true;
    for (const auto& v : vertices) {
        is_full &= v.second._edges.size() == 2; // number of letters
    }
    EXPECT_TRUE(is_full);
}

TEST(MakeFdfaTest, TestNFA) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {2}, 0);
    auto dfa = nfa.convert_to_DFA();
    dfa.make_fdfa();
    auto vertices = dfa.get_vertices();
    EXPECT_EQ(vertices.size(), 4);
    bool is_full = true;
    for (const auto& v : vertices) {
        is_full &= v.second._edges.size() == 2; // number of letters
    }
    EXPECT_TRUE(is_full);
}

TEST(MakeFdfaTest, TestAlreadyFDFA) {
    NondeterministicFiniteAutomata nfa({{0, 1, "a"}, {0, 2, "a"}}, 3, {2}, 0);
    auto dfa = nfa.convert_to_DFA();
    dfa.make_fdfa();
    auto vertices = dfa.get_vertices();
    EXPECT_EQ(vertices.size(), 3);
    bool is_full = true;
    for (const auto& v : vertices) {
        is_full &= (v.second._edges.size() == 1); // number of letters
    }
    EXPECT_TRUE(is_full);
}

TEST(FdfaCopyTest, TestAll) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {2}, 0);
    auto fdfa = dfa.fdfa_copy();
    auto vertices = dfa.get_vertices();
    EXPECT_EQ(vertices.size(), 3);
    bool is_full = true;
    for (const auto& v : vertices) {
        is_full &= (v.second._edges.size() == 1); // number of letters
    }
    EXPECT_FALSE(is_full);
    vertices = fdfa.get_vertices();
    is_full = true;
    for (const auto& v : vertices) {
        is_full &= (v.second._edges.size() == 2); // number of letters
    }
    EXPECT_TRUE(is_full);
}

TEST(MakeComplementFdfaTest, TestAll) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {2}, 0);
    dfa.make_fdfa();
    auto fdfa_vertices = dfa.get_vertices();
    dfa.make_complement_fdfa(true);
    auto cfdfa_vertices = dfa.get_vertices();
    bool terminate_changed = true;
    for (int i = 0; i < cfdfa_vertices.size(); ++i) {
        terminate_changed &= (cfdfa_vertices[i]._is_terminate != fdfa_vertices[i]._is_terminate);
    }
    EXPECT_TRUE(terminate_changed);
}


TEST(CopyComplementFdfaTest, TestAll) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {2}, 0);
    dfa.make_fdfa();
    auto fdfa_vertices = dfa.get_vertices();
    auto complement_fdfa = dfa.complemented_fdfa(true);
    auto cfdfa_vertices = complement_fdfa.get_vertices();
    bool terminate_changed = true;
    for (int i = 0; i < fdfa_vertices.size(); ++i) {
        terminate_changed &= (fdfa_vertices[i]._is_terminate != cfdfa_vertices[i]._is_terminate);
    }
    EXPECT_TRUE(terminate_changed);
}


TEST(MinimalFdfaTest, TestMakeMinimalTest) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {1, 2}, 0);
    dfa.make_minimal_fdfa();
    auto fdfa_vertices = dfa.get_vertices();
    ASSERT_TRUE(fdfa_vertices.size() == 3);
    ASSERT_TRUE(fdfa_vertices[2]._is_terminate);
    ASSERT_EQ(fdfa_vertices[0]._edges[0].word, "a");
    ASSERT_EQ(fdfa_vertices[0]._edges[1].word, "b");
    ASSERT_EQ(fdfa_vertices[0]._edges[0].end, 2);
    ASSERT_EQ(fdfa_vertices[0]._edges[1].end, 2);
}

TEST(MinimalFdfaTest, TestCopyMinimalTest) {
    DeterministicFiniteAutomata dfa({{0, 1, "a"}, {0, 2, "b"}}, 3, {1, 2}, 0);
    auto copy = dfa.minimal_fdfa();
    dfa.make_minimal_fdfa();
    auto dfa_vertices = dfa.get_vertices();
    auto copy_vertices = copy.get_vertices();
    ASSERT_EQ(dfa_vertices.size(), copy_vertices.size());
    for (int i = 0; i < dfa_vertices.size(); ++i) {
        ASSERT_TRUE(dfa_vertices[i] == copy_vertices[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
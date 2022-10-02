# formal-languages
## Some tools to work with formal languages.

## What can you find here:

- Translator from nondeterministic finite automata (NFA) to deterministic finite automata (DFA).
- Converter from DFA to complete DFA.
- Converter from complete DFA to it's complement DFA.
- Normalization of finite automata (shortens all edges so they become length of 1).

### Code is covered by Google tests. To run them, you need to:

- Checkout to branch `template-dev`.
- Run bash-script `.tools/coverage.sh` -- that will create all nesessary files.
- Open `./tools/report/index.html` in your browser (for exapmle, you can do it by running `firefox ./tools/report/index.html` from project folder).

You can find tests in `tools/google_tests.cpp`. You can also change `./coverage.sh` by removing comments in it, so that it will show covered branches.

## Usage
You can include in your file `Finite_automatas.h`, or write code in `main.cpp` and then run it. All methods have short description. You can read about finite automatas [here](https://en.wikipedia.org/wiki/Deterministic_finite_automaton).

/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2
 */
#ifndef __REGEX_FACTORY_H__
#define __REGEX_FACTORY_H__

#include <algorithm>
#include <string>
#include <deque>
#include <stack>
#include <set>
#include <stdlib.h>
#include "infix_parse.h"
#include "state.h"
#include "regex.h"

using namespace std;

/**
 * Regex Factory
 * This class generates both NFA's and DFA's...
 * representation of the given language in regular...
 * expression format.
 * Afterwards the build, it returns a regex instance...
 * ready to be used.
 */
class regex_factory {
private:

    /**
     * Holds all intermediate states generated in...
     * the proccess to build the NFA.
     */
    deque<deque<state *> *> *__nfa_comp;

    /**
     * Holds the expression already in postfix format.
     */
    string *__lang;
    int __id = 0; // holds the number of states

    /**
     * PUSH postfix operation.
     * It creates two states that are connected...
     * through a given character and push to stack.
     */
    void push_op(char);

    /**
     * CONCAT postfix operation.
     * It pops two elements from the stack and...
     * operates between them.
     * It basically connects the last state from...
     * the first poped machine to the first state...
     * of the last poped machine.
     * Afterwards, it pushes to the stack one ref...
     * of the concatenated machine.
     */
    bool concat_op();

    /**
     * OR (union) postfix operation.
     * It pops two elements from the stack, creates...
     * two new states (initial and final) and epsilon...
     * connect them to the initial and final states of...
     * both machines.
     * Afterwards, it pushes to the stack one ref...
     * of the united machine.
     */
    bool or_op();

    /**
     * STAR postfix operation.
     * It pops one element from the stack and connects...
     * its first and last states with epsilon interchangeably.
     * Afterwards it pushes the machine to the stack.
     */
    bool star_op();

    /**
     * Creates a set of epsilon steps from a predefined...
     * set of states.
     */
    void epsilon_steps(set<state *>, set<state *> &);

    /**
     * Join a set of states in a little set with repeated...
     * states together (by a common connection).
     */
    void step(char, set<state *>, set<state *> &);

    /**
     * Generates a NFA representation of the language...
     * and return the NFA in a deque format (basically...
     * a stack that can be manipulated on both ways).
     */
    deque<state *> *generate_nfa();

    /**
     * Generates a DFA representation of the already...
     * generated NFA representation of the language.
     */
    deque<state *> *generate_dfa(deque<state *> *);

public:
    /**
     * Initializes the language given in infix format...
     * to postfix.
     */
    regex_factory(string);

    /**
     * Frees all the memory allocated by the instance.
     */
    ~regex_factory();

    /**
     * Builds a complete regex object with both NFA and...
     * DFA representations of the language.
     */
    regex *build();
};

#endif

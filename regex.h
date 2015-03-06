/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2
 */
#ifndef __REGEX_H__
#define __REGEX_H__

// Can be used to debug the match function.
//#define DEBUG

#include <deque>
#include <iostream>
#include "state.h"

using namespace std;

/**
 * Regex
 * This class holds both NFA and DFA of a language...
 * and verify a match for a given string.
 */
class regex {
private:
    /**
     * Deque of a NFA and DFA.
     */
    deque<state *> *__nfa;
    deque<state *> *__dfa;

public:
    /**
     * Receives both NFA and DFA objects.
     */
    regex(deque<state *> *, deque<state *> *);

    /**
     * Frees both objects. (maybe a poor design?)
     */
    ~regex();

    /**
     * Outputs the DFA and NFA to stdout.
     */
    void display_dfa(unsigned int);
    void display_nfa(unsigned int);

    /**
     * Verify if a string belongs to the language.
     */
    bool match(string);
};

#endif

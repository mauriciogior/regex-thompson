/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2
 */
#ifndef __STATE_H__
#define __STATE_H__

#include <set>
#include <deque>

using namespace std;

class state {
public:
    /**
     * Holds the transitions between states.
     */
    struct connection {
        ::state *state; // :: for scope
        char input;
    };
    typedef struct connection connection;

private:
    deque<connection> *__connections;

    /**
     * Contain a set of states that...
     * it has transition for a determined...
     * character.
     */
    set<state *> __collection;
    int __id;
    bool __final = false;

public:
    /**
     * Builds a single state with an id.
     */
    state(int);

    /**
     * Builds a single state from a set...
     * of other states (used by DFA).
     * Ex: {q0, q1, q2} in {qx}
     */
    state(int, set<state *>);
    ~state();
    
    void put_connection(state *, char);
    void set_final();

    int get_id();
    bool is_final();
    deque<connection> *get_connections();

    set<state *> get_collection();
};

#endif

/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2
 */
#include "regex.h"

regex::regex(deque<state *> *nfa, deque<state *> *dfa) {
    __nfa = nfa;
    __dfa = dfa;
}

regex::~regex() {
    for (unsigned int i=0; i<__nfa->size(); i++) {
        delete __nfa->at(i);
    }

    for (unsigned int i=0; i<__dfa->size(); i++) {
        if (__dfa->at(i) != NULL) {
            delete __dfa->at(i);
        }
    }

    delete __nfa;
    delete __dfa;
}

void regex::display_dfa(unsigned int i) {
    if (__dfa->size() <= i) {
        return;
    }

    state *s = __dfa->at(i);
    deque<state::connection> *conns = s->get_connections();

    if (i == 0) cout << "q" << s->get_id() << " is initial" << endl;

    for (unsigned int j=0; j<conns->size(); j++) {
        cout << "q" << s->get_id() << "---" << conns->at(j).input;
        cout << "--->" << "q" << conns->at(j).state->get_id() << endl;
    }

    if (s->is_final()) cout << "q" << s->get_id() << " is final" << endl;

    display_dfa(++i);
}

void regex::display_nfa(unsigned int i) {
    if (__nfa->size() <= i) {
        return;
    }

    state *s = __nfa->at(i);
    deque<state::connection> *conns = s->get_connections();

    if (i == 0) cout << "q" << s->get_id() << " is initial" << endl;

    for (unsigned int j=0; j<conns->size(); j++) {
        cout << "q" << s->get_id() << "---" << conns->at(j).input;
        cout << "--->" << "q" << conns->at(j).state->get_id() << endl;
    }

    if (s->is_final()) cout << "q" << s->get_id() << " is final" << endl;

    display_nfa(++i);
}

bool regex::match(string input) {
    state *s = __dfa->at(0);

    for (unsigned int i=0; i<input.size(); i++) {
        char c = input.at(i);
        if (c == 'e') continue;
        if (c != 'a' && c != 'b') return false;

#ifdef DEBUG
        cout << "q" << s->get_id() << "---" << c << "--->";
#endif

        deque<state::connection> *conns = s->get_connections();

        for (unsigned int j=0; j<conns->size(); j++) {
            state::connection conn = conns->at(j);

            if (conn.input == c) {
                s = conn.state;
                break;
            }
        }
    }

    if (!s->is_final()) {
        return false;
    }

#ifdef DEBUG
    cout << "q" << s->get_id() << endl;
#endif

    return true;
}

/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2
 */
#include "regex_factory.h"

regex_factory::regex_factory(string lang) {
    __lang = infix_parse::to_postfix(&lang);
    __nfa_comp = new deque<deque<state *> *>();
}

regex_factory::~regex_factory() {
    delete __lang;
    delete __nfa_comp;
}

void regex_factory::push_op(char c) {
    state *q0 = new state(++__id);
    state *q1 = new state(++__id);

    q0->put_connection(q1, c);

    deque<state *> *nfa = new deque<state *>();
    nfa->push_back(q0);
    nfa->push_back(q1);

    __nfa_comp->push_back(nfa);
}

bool regex_factory::concat_op() {
    deque<state *> *nfaA, *nfaB;

    nfaB = __nfa_comp->back();
    if (nfaB == NULL) return false;
    __nfa_comp->pop_back();

    nfaA = __nfa_comp->back();
    if (nfaA == NULL) return false;
    __nfa_comp->pop_back();

    state *qB0 = nfaB->front();
    state *qAn = nfaA->back();

    qAn->put_connection(qB0, 'e');

    for (unsigned int i=0; i<nfaB->size(); i++) {
        nfaA->push_back(nfaB->at(i));
    }

    delete nfaB;

    __nfa_comp->push_back(nfaA);

    return true;
}

bool regex_factory::or_op() {
    deque<state *> *nfaA, *nfaB;

    nfaA = __nfa_comp->back();
    if (nfaA == NULL) return false;
    __nfa_comp->pop_back();

    nfaB = __nfa_comp->back();
    if (nfaB == NULL) return false;
    __nfa_comp->pop_back();

    state *q0 = new state(++__id);
    state *q1 = new state(++__id);
    state *qA0 = nfaA->front();
    state *qB0 = nfaB->front();
    state *qAn = nfaA->back();
    state *qBn = nfaB->back();

    q0->put_connection(qA0, 'e');
    q0->put_connection(qB0, 'e');
    qAn->put_connection(q1, 'e');
    qBn->put_connection(q1, 'e');

    nfaA->push_front(q0);
    nfaB->push_back(q1);

    for (unsigned int i=0; i<nfaB->size(); i++) {
        nfaA->push_back(nfaB->at(i));
    }

    delete nfaB;

    __nfa_comp->push_back(nfaA);
    
    return true;
}

bool regex_factory::star_op() {
    deque<state *> *nfa;

    nfa = __nfa_comp->back();
    if (nfa == NULL) return false;
    __nfa_comp->pop_back();

    // OLD
    // I created two states to connect...
    // between the intial and final states...
    // of the machine.
    // This can be shortcuted to connect...
    // directly the first state to the last...
    // interchangeably using epsilon.
    //state *q0 = new state(++__id);
    //state *qn = new state(++__id);

    state *qA0 = nfa->front();
    state *qAn = nfa->back();

    qA0->put_connection(qAn, 'e');
    qAn->put_connection(qA0, 'e');

    // OLD
    //q0->put_connection(qn, 'e');
    //q0->put_connection(qA0, 'e');
    //qAn->put_connection(q1, 'e');
    //qAn->put_connection(qA0, 'e');
    //nfa->push_front(q0);
    //nfa->push_back(q1);

    __nfa_comp->push_back(nfa);

    return true;
}

deque<state *> *regex_factory::generate_nfa() {
    for (unsigned int i=0; i<__lang->size(); i++) {
        char c = __lang->at(i);

        if (c == 'a' || c == 'b' || c == 'e') {
            push_op(c);
        } else if (c == '|') {
            if (!or_op()) return NULL;
        } else if (c == '*') {
            if (!star_op()) return NULL;
        } else if (c == '.') {
            if (!concat_op()) return NULL;
        } else {
            return NULL;
        }
    }

    deque<state *> *nfa = __nfa_comp->back();
    if (nfa == NULL) return NULL;
    __nfa_comp->pop_back();

    nfa->back()->set_final();

    return nfa;
}

void regex_factory::epsilon_steps(set<state *> in, set<state *> &out) {
    deque<state::connection> *connections;
    stack<state *> to_process;

    out.clear();
    out = in;

    for (set<state *>::iterator it = in.begin(); it != in.end(); it++) {
        to_process.push(*it);
    }

    while (!to_process.empty()) {
        state *s = to_process.top();
        to_process.pop();

        connections = s->get_connections();

        for (unsigned int i=0; i<connections->size(); i++) {
            state::connection c = connections->at(i);

            if (c.input != 'e') continue;

            if (out.find(c.state) == out.end()) {
                out.insert(c.state);
                to_process.push(c.state);
            }
        }
    }
}

void regex_factory::step(char c, set<state *> in, set<state *> &out) {
    deque<state::connection> *connections;
    out.clear();

    for (set<state *>::iterator it = in.begin(); it != in.end(); it++) {
        state *s = *it;

        connections = s->get_connections();

        for (unsigned int i=0; i<connections->size(); i++) {
            state::connection conn = connections->at(i);

            if (conn.input == c) {
                out.insert(conn.state);
            } 
        }
    }
}

deque<state *> *regex_factory::generate_dfa(deque<state *> *nfa) {
    deque<state *> *dfa = new deque<state *>();

    __id = 0;
    stack<state *> to_process;

    set<state *> dfa_start_set;
    set<state *> nfa_start_set;
    nfa_start_set.insert(nfa->at(0));
    epsilon_steps(nfa_start_set, dfa_start_set);

    state *dfa_start = new state(++__id, dfa_start_set);
    dfa->push_back(dfa_start);
    to_process.push(dfa_start);

    while (!to_process.empty()) {
        state *curr = to_process.top();
        to_process.pop();

        // a, b inputs
        for (int i=0; i<2; i++) {
            char c = i == 0 ? 'a' : 'b';

            set<state *> step_set, epsilon_step_set;
            step(c, curr->get_collection(), step_set);
            epsilon_steps(step_set, epsilon_step_set);

            bool enclosed = false;
            state *enclosure = NULL;

            for (unsigned int j=0; j<dfa->size(); j++) {
                enclosure = dfa->at(j);

                if (enclosure->get_collection() == epsilon_step_set) {
                    enclosed = true;
                    break;
                }
            }

            if (enclosed) {
                curr->put_connection(enclosure, c);
            } else {
                state *join = new state(++__id, epsilon_step_set);
                to_process.push(join);
                dfa->push_back(join);

                curr->put_connection(join, c);
            }
        }
    }

    return dfa;
}

regex *regex_factory::build() {
    deque<state *> *nfa = generate_nfa();
    deque<state *> *dfa = generate_dfa(nfa);

    return new regex(nfa, dfa);
}

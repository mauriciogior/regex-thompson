#include "state.h"

state::state(int id) {
    __id = id;
    __connections = new deque<connection>();
}

state::state(int id, set<state *> collection) : state(id) {
    for (set<state *>::iterator it = collection.begin();
        it != collection.end(); it ++) {

        if ((*it)->is_final()) {
            __final = true;
            break;
        }
    }

    __collection = collection;
}

state::~state() {
    delete __connections;
}

void state::put_connection(state *s, char c) {
    connection conn;

    conn.state = s;
    conn.input = c;

    __connections->push_back(conn);
}

void state::set_final() {
    __final = true;
}

bool state::is_final() {
    return __final;
}

int state::get_id() {
    return __id;
}

deque<state::connection> *state::get_connections() {
    return __connections;
}

set<state *> state::get_collection() {
    return __collection;
}

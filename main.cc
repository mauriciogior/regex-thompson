#include <string.h>
#include <string>
#include <iostream>
#include "regex_factory.h"

using namespace std;

int main(int argc, char **argv) {
    bool display = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-v") == 0) {
            display = true;
        }
    }

    string in("");

    regex_factory *factory = NULL;
    regex *machine = NULL;

    int count = 0;

    while (getline(cin, in)) {
        if (count == 0) {
            factory = new regex_factory(in);
            machine = factory->build();

            if (display) {
                cout << "==NFA==" << endl;
                machine->display_nfa(0);
                cout << endl << "==DFA==" << endl;
                machine->display_dfa(0);
                cout << endl;
            }
        } else {
            if (machine->match(in)) {
                cout << "yes" << endl;
            } else {
                cout << "no" << endl;
            }
        }

        count ++;
    }

    if (factory != NULL) {
        delete factory;
    }

    if (machine != NULL) {
        delete machine;
    }

    return 0;
}

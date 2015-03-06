/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/21/2015
 * License: GPLv2
 */
#include "infix_parse.h"

string *infix_parse::transform(string *expression) {
    string *transformed = new string("");

    int size = expression->size();
    int i;

    for (i=0; i<size; i++) {
        char fst = expression->at(i);

        if (size > i+1) {
            char snd = expression->at(i+1);

            transformed->push_back(fst);

            if (fst != '(' && fst != '|'
             && snd != ')' && snd != '|' && snd != '*') {
                transformed->push_back('.');
            }
        }
    }

    transformed->push_back(expression->at(i-1));

    return transformed;
}

string *infix_parse::to_postfix(string *expression) {
    string *postfix = new string("");
    string *t_expression = transform(expression);

    stack<char> extra;
    int size = t_expression->size();

    for (int i=0; i<size; i++) {
        char c = t_expression->at(i);

        switch (c) {
            case '(':
                extra.push(c);
                break;

            case ')':
                while (extra.top() != '(') {
                    postfix->push_back(extra.top());
                    extra.pop();
                }

                extra.pop();
                break;

            default:
                while (extra.size() > 0) {
                    char top = extra.top();

                    int p_char = PRIORITY(c);
                    int p_extra = PRIORITY(top);

                    if (p_extra >= p_char) {
                        postfix->push_back(top);
                        extra.pop();
                    } else {
                        break;
                    }
                }

                extra.push(c);
                break;
        }
    }

    while (extra.size() > 0) {
        postfix->push_back(extra.top());
        extra.pop();
    }

    delete t_expression;

    return postfix;
}

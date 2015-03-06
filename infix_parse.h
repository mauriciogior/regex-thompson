/**
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/21/2015
 * License: GPLv2
 */
#ifndef __INFIX_PARSE_H__
#define __INFIX_PARSE_H__

#include <string>
#include <stack>

#define PRIORITY(a) ((a == '(') ? 1 : ((a == '|') ? 2 : ((a == '.') ? 3 : (a == '*') ? 4 : 5)))

using namespace std;

/**
 * Infix to Postfix parser.
 */
class infix_parse {
private:
    /**
     * Transform a regular expression by inserting '.'...
     * as explicit concatenation.
     */
    static string *transform(string *);
public:
    /**
     * Uses Shunting-yard algorithm to convert expression...
     * from infix to postfix format.
     */
    static string *to_postfix(string *);
};

#endif

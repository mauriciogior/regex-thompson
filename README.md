# About me
 * Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
 * Date: 2/22/2015
 * License: GPLv2

# How To

There is a helper `Makefile` inside this directory. To compile the program, you may execute `make`. To run the program, you may execute either `make run` or `./regex`.

There is an input file within this directory. To test it, you may use `make run < input`.

The first line of the file must contain a regular expression in infix representation with only UNION and STAR operations. It also only accepts the following alphabet: ('a', 'b', 'e').

# About

## infix_parse

It simply transforms an expression from infix to postfix representation.

## state

This class holds a state representation of a machine.

## regex

This class uses the DFA built by `regex_factory` and verifies if a determined string belongs to a determined language.

## regex_factory

This class builds the NFA and DFA machines using a postfix notation to operate between PUSH, UNION, STAR and CONCATENATION.

The NFA is build inside method `regex_factory::generate_nfa` while the DFA is build inside `regex_factory::generate_dfa` (it requires a built NFA).

### NFA
To implement the NFA, the program basically runs through all the expression and, for each character, it executes the determined operation.

Regular characters (a, b, e) from the alphabet calls the PUSH operation (creates one machine with two states and pushes the machine to the stack).

Star (*) characters calls the STAR operation (pops one machine from the stack, adds an epsilon connection between the start and final states and pushes the machine back to the stack).

Union (|) characters calls the OR (or UNION) operation (pops two machines from the stack, creates two states [one start and one final] and connect them to the machines' respective states with epsilon connection. Afterwards, it pushes only one of the machines with all states).

Concatenation (.) characters calls the CONCAT operation (pops two machines from the stack, connects the final state from the first machine to the start state from the last machine with epsilon connection and pushes only one of the machines with all states).

After all the operations, like any postfix calculator, the only remaining machine on the stack is the NFA (so we just pop it).

### DFA

To build the DFA, we search all epsilon connections from a start state, reduce them to remove redundancy and proceed with the next states. For each connection a or b, it searches the NFA for repeated states for that determined connection and join them in one single state.

The final built DFA does not contain any epsilon connections, facilitating the match function and avoiding the need to guess the correct path.

# References

*http://www.codeproject.com/Articles/5412/Writing-own-regular-expression-parser
*http://swtch.com/~rsc/regexp/regexp1.html

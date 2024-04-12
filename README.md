# Simplang

_If you're going to simp for anything on the Internet, it may as well be a crappy programming language you found on GitHub_

Simplang is a simple and lightweight, interpreted scripting language for performing arithmetic operations.

Simplang supports 64-bit signed integers, string literals, user input, the four main arithmetic operations, increment/decrement, if statements (no nested if blocks yet), and do-while loops (with nested loop support).

## Usage

Simplang supports executing script files or operating as a repl interactively.
To execute a script, run `simplang <script file>`, and to use the repl, just run `simplang` with no arguments.

Here's an example Simplang script:

```
SIMPLANG
ldstr s0 "Welcome to Simplang!"
print_s s0
asgn r1 6
asgn r2 3
add r0 r1 r2
print r0
done
```

Running this script will print "Welcome to Simplang!" on one line, followed by 9 on the next.

All Simplang scripts need to begin with `SIMPLANG` on the first line and end with `done` on the last line.
Simplang uses registers to store variables, and by default there are 8 registers available for each type.
Registers `r0` - `r7` are used for integers and `s0` - `s7` are used for strings.

There are more examples that you can run in the tests/ directory.

## Building

```
git clone https://github.com/calebstein1/simplang
cd simplang
make
```

This will produce a `simplang` binary in the bin/ directory.
There are no dependencies outside of the C standard library.

## Reference

`asgn <a1> <a2>` - stores the value specified by a2 in the register specified by a1

`rand <a1> <a2>` - stores a pseudo-randomly generated number in the register specified by a1 between 0 and a2 - 1, a2 may be a register or a number

`ldstr <a1> <a2>` - stores the string literal specified by a2 in the register specified by a1

`geti <a1>` - gets an integer from the user and stores it in the register specified by a1

`(add, subtr, mul, div) <a1> <a2> <a3>` - perform the arithmetic operation specified on a2 and a3 in that order, then store the result in the register specified by a1

`(incr, decr) <a1>` - increment or decrement the value stored at the register specified by a1

`beglp` - begins a loop

`cont` - jump back to the beginning of the current loop

`(endlpeq, endlpne, endlplt, endlple) <a1> <a2>` - ends execution of a loop based on the condition comparing a1 and a2, otherwise jump back to `beglp`

`(ifeq, ifne, iflt, ifle) <a1> <a2>` - executes the following code block based on the condition comparing a1 and a2, otherwise jump ahead to the next `endif`

`endif` - signifies the end of an if block

`(print, printn) <a1>` - prints the integer value stored in the register specified by a1, the n variant omits the trailing newline

`(print_s, printn_s) <a1>` - prints the string value stored in the register specified by a1, print_s cannot print string literals directly, the n varirant omits the trailing newline

`done` - ends execution of the program

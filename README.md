# Simplang

Simplang is a simple and lightweight interpreted scripting language with an assembly-like syntax.

Simplang supports 64-bit signed integers, string literals, user input, the four main arithmetic operations, increment/decrement, if statements (with nesting support), and do-while loops (with nested loop support).

## Usage

Simplang supports executing script files or operating as a repl interactively.
To execute a script, use `#!/usr/bin/env simplang` as your shebang, or run `simplang <script file>`, and to use the repl, just run `simplang` with no arguments.
You can pass integers to your Simplang scripts as command line arguments, and the script will run with its int registers pre-initialized to the supplied values, sequentially starting from r0.

Here's an example Simplang script:

```
#!/usr/bin/env simplang

ldstr s0 "Welcome to Simplang!"
print s0
asgn r1 6
asgn r2 3
add r0 r1 r2
print r0
done
```

Running this script will print "Welcome to Simplang!" on one line, followed by 9 on the next.

All Simplang scripts need to end with `done` on the last line.
Simplang uses registers to store variables, and by default there are 8 registers available for each type.
Registers `r0` - `r7` are used for integers and `s0` - `s7` are used for strings.
Comments begin with `;`.
They must be on their own line, but may be indented, and there must be at at least one whitespace character between the semicolon and the comment text.

There are more examples that you can run in the tests/ directory.

## Building/Installing

```
git clone https://github.com/calebstein1/simplang
cd simplang
make
sudo make install
```

This will produce a `simplang` binary in the bin/ directory.
There are no dependencies outside of the C standard library.
Simplang is self-contained in a single binary, `make install` copies this binary to `/usr/local/bin`, and `make uninstall` deletes it.

## Reference

`asgn <a1> <a2>` - stores the value specified by a2 in the register specified by a1

`rand <a1> <a2>` - stores a pseudo-randomly generated number in the register specified by a1 between 0 and a2 - 1, a2 may be a register or a number

`ldstr <a1> <a2>` - stores the string literal specified by a2 in the register specified by a1

`geti <a1>` - gets an integer from the user and stores it in the register specified by a1

`gets <a1>` - gets a string from the user and stoes it in the register specified by a1

`(add, subtr, mul, div) <a1> <a2> <a3>` - perform the arithmetic operation specified on a2 and a3 in that order, then store the result in the register specified by a1

`(incr, decr) <a1>` - increment or decrement the value stored at the register specified by a1

`swp <a1> <a2>` - swap the values stored in the registers specified by a1 and a2

`beglp` - begins a loop

`cont` - jump back to the beginning of the current loop

`(endlpeq, endlpne, endlplt, endlple, endlpgt, endlpge) <a1> <a2>` - ends execution of a loop based on the condition comparing a1 and a2, otherwise jump back to `beglp`

`(ifeq, ifne, iflt, ifle, ifgt, ifge) <a1> <a2>` - executes the following code block based on the condition comparing a1 and a2, otherwise jump ahead to the next `endif`

`endif` - signifies the end of an if block

`(print, printn) <a1>` - prints the integer value or string stored in the register specified by a1, or print a number or string literal passed directly as an argument, the n variant omits the trailing newline

`done` - ends execution of the program

Hint: all int registers (r0 - r7) are initialized to 0 by default, so you can immediately use any of them as a loop counter with the `incr` instruction without having to explicitly initialize them.

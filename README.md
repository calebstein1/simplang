# Simplang

Simplang is a simple and lightweight interpreted scripting language with an assembly-like syntax.

Simplang supports dynamic typing, 64-bit signed integers, string literals, user input, the four main arithmetic operations, increment/decrement, if statements (with nesting support), and do-while loops (with nested loop support).

## Usage

Simplang supports executing script files or operating as a repl interactively.
To execute a script, use `#!/usr/bin/env simplang` as your shebang, or run `simplang <script file>`, and to use the repl, just run `simplang` with no arguments.
You can pass integers to your Simplang scripts as command line arguments, and the script will run with its int registers pre-initialized to the supplied values, sequentially starting from r0.

Here's an example Simplang script:

```
#!/usr/bin/env simplang

load_string welcome_msg "Welcome to Simplang!"
print welcome_msg
assign num1 6
assign num2 3
add num1 num2
print num1
done
```

Running this script will print "Welcome to Simplang!" on one line, followed by 9 on the next.

All Simplang scripts need to end with `done` on the last line.
Simplang supports up to 24 uniquely named variables, which can either be integers or strings.
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

`assign <a1> <a2>` - stores the value specified by a2 in the register specified by a1

`random <a1> <a2>` - stores a pseudo-randomly generated number in the register specified by a1 between 0 and a2 - 1, a2 may be a register or a number

`load_string <a1> <a2>` - stores the string literal specified by a2 in the register specified by a1

`get_option <a1> <a2>` - prints a string literal prompt specified by a2 and gets and integer from the user to store in the regester specified by a1 if that register hasn't already been filled

`get_number <a1>` - gets an integer from the user and stores it in the register specified by a1

`get_string <a1>` - gets a string from the user and stoes it in the register specified by a1

`(add, subtract, multiply, divide, modulo) <a1> <a2>` - perform the arithmetic operation specified on the value stored in the a1 register with the number specified by a2, then store the result back in a1

`(increment, decrement) <a1>` - increment or decrement the value stored at the register specified by a1

`swap <a1> <a2>` - swap the values stored in the registers specified by a1 and a2

`begin_loop` - begins a loop

`continue` - jump back to the beginning of the current loop

`(end_loop_if_equal, end_loop_if_not_equal, end_loop_if_less, end_loop_if_less_or_equal, end_loop_if_greater, end_loop_if_greater_or_equal) <a1> <a2>` - ends execution of a loop based on the condition comparing a1 and a2, otherwise jump back to `beglp`

`(if_equal, if_not_equal, if_less, if_less_or_equal, if_greater, if_greater_or_equal) <a1> <a2>` - executes the following code block based on the condition comparing a1 and a2, otherwise jump ahead to the next `end_if`

`end_if` - signifies the end of an if block

`(print, print_n) <a1>` - prints the integer value or string stored in the register specified by a1, or print a number or string literal passed directly as an argument, the n variant omits the trailing newline

`done` - ends execution of the program

Hint: all int registers (r0 - r7) are initialized to 0 by default, so you can immediately use any of them as a loop counter with the `incr` instruction without having to explicitly initialize them.

## Known Bugs:
- Trying to assign a register to a constant number within a loop doesn't seem to be working properly

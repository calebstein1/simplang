# Simplang

_If you're going to simp for anything on the Internet, it may as well be a crappy math language you found on GitHub_

Simplang is a simple and lightweight, interpreted scripting language for performing arithmetic operations.

At the moment, the only supported data type is a signed 64-bit int.
Simplang supports the four main arithmetic operations at the moment, as well increment/decrement and do-while loops.

## Usage

Simplang supports executing script files or operating as a repl interactively.
To execute a script, run `simplang <script file>`, and to use the repl, just run `simplang` with no arguments.

Here's an example Simplang script:

```
SIMPLANG
asgn r1 6
asgn r2 3
add r0 r1 r2
print r0
done
```

Running this script will print 9.

All Simplang scripts need to begin with `SIMPLANG` on the first line and end with `done` on the last line.
Simplang uses registers to store variables, and by default there are 8 registers available.

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

`(add, subtr, mul, div) <a1> <a2> <a3>` - perform the arithmetic operation specified on a2 and a3 in that order, then store the result in the register specified by a1

`(incr, decr) <a1>` - increment or decrement the value stored at the register specified by a1

`beglp` - begins a loop

`endlpeq <a1> <a2>` - ends execution of a loop if the values of a1 and a2 are equal, otherwise jump back to `beglp`

`print <a1>` - prints the value of a1

`done` - ends execution of the program

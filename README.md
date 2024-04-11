# SimpLang

_If you're going to simp for anything on the Internet, it may as well be a crappy math language you found on GitHub_

SimpLang is a simple and lightweight, interpreted scripting language for performing arithmetic operations.

At the moment, the only supported data type is a signed 64-bit int, and the only supported operations are addition, subtraction, multiplication, and division (truncates down to the nearest int).

## Usage

SimpLang uses registers to store variables, and by default there are four registers available.

Here's an example SimpLang file:

```
SIMPLANG
ASGN r1 6
ASGN r2 3
ADD r0 r1 r2
PRINT r0
DONE
```

Running this script will print 9.

All SimpLang scripts need to begin with `SIMPLANG` on the first line and end with `DONE` on the last line.

There are more examples that you can run in the tests/ directory.

## Building

```
git clone https://github.com/calebstein1/simplang
cd simplang
make
```

This will produce a `simplang` binary in the bin/ directory.
There are no dependencies outside of the C standard library.

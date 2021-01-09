# Hydra
A small interpreted language with a lisp-like syntax
The goal of hydra is to make a sort of 'omni-language': support for multiple paradigms, in typing
(static/dynamic), memory management (manual/gc) and style (functional/object-oriented).

## Syntax
Hydra is composed of S-expressions, e.g.
```
(if (>= 1 3)
    "One is bigger"
    "Three is bigger")
```

## Values
Values in hydra are like those of other languages: there are currently integers, strings, lists,
input streams and characters. Many have predictable literals, e.g. '"' for strings.

Values can be placed in the global namespace with `def`, e.g.
```
(def x 2)
```

The primary composite data structure is the list, which is formed from cons cells, e.g.
```
(cons 1 (cons 2 nil))
```
The first value in the cons cell is called the `car`, and accessibel by the same name

Custom values (ADTs/classes) WIP

## Functions

Hydra *only* has lambda functions:
```
(fn (x y) (+ x y))
```

So, to get a global function, use `def`:
```
> (def inc (fn (x) (+ x 1)))
* user-defined operation
> (incf 1)
* 2
```

The `lang.hd` file provides the `defn` convenience macro: 
```
(defn inc (x) (+ x 1))
```

# Try it out
The project uses cmake, so to build/run:
```sh
git clone https://www.github.com/rationalis-petra/hydra
mkdir hydra/build
cd hydra/build
cmake ..
make
```

Then run with `./hydra`. IMPORTANT: currently, the language must be run out of the build folder,
as it looks for various functions/macros in `hydra/lang.hd`, relative to the binary!

## Future Goals
+ convenient ffi
+ complex types (objects/ADTs)
+ generic function machinery 
+ static typing
+ compiler/transpiler

# Hydra
A small interpreted language with a lisp-like syntax
## Syntax
Hydra is composed of S-expressions, e.g.
```
(if (>= 1 3)
    "One is bigger"
    "Three is bigger")
```

The aim is to investigate & play with various type-systems.

## Values
Values in hydra are like those of other languages: there are currently integers, strings, lists,
input streams and characters. Currently, many have predictable literals, e.g. '"' for strings.

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
> (defn inc (x) (+ x 1))
```

## Compiling
The project uses cmake as a build system

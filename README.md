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
> (def x 2)
* 2
```

The primary composite data structure is the list, which is formed from cons cells, e.g.
```
> (def lst (cons 1 (cons 2 nil)))
* (1 2)
```
The first value in the cons cell is called the `car`, and the second the `cdr`. They are and accessible by functions of the same name:
```
> (car lst)
* 1
> (cdr list)
* (2)
```

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

The language also has macros, ffi and modules. I am currently working on the [wiki](https://github.com/rationalis-petra/hydra/wiki).


# Try it out
The project uses [cmake](https://cmake.org/) to build, and [conan](https://conan.io/) for dependencies. To install:
```sh
git clone https://www.github.com/rationalis-petra/hydra
mkdir hydra/build
cd hydra/build
conan install ..
```
Then, depending on your os:
```sh
(windows)
cmake .. -G "Visual Studio 16"
cmake --build . --config Release
```
```
(linux, osx)
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```


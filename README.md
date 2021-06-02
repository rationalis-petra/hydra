# Hydra
A small interpreted, object-oriented language which is based off of self and lisp programming languages.
As far as documentation goes, I use the github [wiki](https://github.com/rationalis-petra/hydra/wiki), although 
this doesn't (currently) document all features.

If you are looking through the source-code, the [readme-source](README_SOURCE.md) describes the reasoning behind
the baisic file/class structure.

## Syntax
Hydra syntax is extremely simple - the general form of a program is `(operation-to-perform value-1 value-2 ...)`. For example, and if statement:
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

The `hydra:core` module (enabled by default) provides the `defn` convenience macro: 
```
(defn inc (x) (+ x 1))
```

For more examples and syntax, check the [wiki](https://github.com/rationalis-petra/hydra/wiki)

# How to Build
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
The `--config Release` is very important - some debug flags will prevent certain parts of the lanugage from working correctly!
Note: if you want to use g++ on windows, use `conan install .. --settings compiler="gcc"`, and `cmake .. -G "MinGw Makefiles"`
```sh
(linux, osx)
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

If you'd prefer not to use conan, you'll need to get the libraries ffi (libffi) and ltdl (libtool), e.g. via a package manager, so that your compiler can link to them with `-ltdl` and `-lffi` respectively. Then run:
```sh
git clone https://www.github.com/rationalis-petra/hydra
mkdir hydra/build
cd hydra/build
cmake .. -DUseConan=OFF -G "<replace-this-with-your-target>"
cmake --build .
```

## Startup
Once you've built the program, you can execute it as you would any other binary. This will start a repl - to stop the program, type =(exit)= at any time.

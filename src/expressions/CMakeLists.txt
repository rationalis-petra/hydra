
add_library(expressions OBJECT
                        data/bool.cpp
                        data/char.cpp
                        data/cons.cpp
                        data/number.cpp
                        data/string.cpp
                        data/tuple.cpp
                        data/union.cpp
                        data/vector.cpp
                        data/parent.cpp

                        concurrency.cpp
                        mirror.cpp
                        object.cpp
                        oper.cpp
                        genericfn.cpp
                        handler.cpp
                        user_oper.cpp
                        symbol.cpp
                        values.cpp
                        module.cpp
                        foreign.cpp)


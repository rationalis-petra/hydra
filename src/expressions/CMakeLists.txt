
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

                        accessor.cpp
                        concurrency.cpp
                        foreign.cpp
                        genericfn.cpp
                        handler.cpp
                        input_output.cpp
                        mirror.cpp
                        module.cpp
                        network.cpp
                        object.cpp
                        oper.cpp
                        symbol.cpp
                        user_oper.cpp)



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

                        concurrency/thread.cpp
                        concurrency/mutex.cpp

                        io/input_output.cpp
                        io/network.cpp

                        accessor.cpp
                        foreign.cpp
                        genericfn.cpp
                        handler.cpp
                        mirror.cpp
                        module.cpp
                        object.cpp
                        oper.cpp
                        symbol.cpp
                        user_oper.cpp)


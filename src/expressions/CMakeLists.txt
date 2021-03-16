
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

                        runtime.cpp
                        lexical_scope.cpp

                        concurrency.cpp
                        object.cpp
                        user_object.cpp
                        oper.cpp
                        handler.cpp
                        user_oper.cpp
                        symbol.cpp
                        values.cpp
                        module.cpp
                        foreign.cpp)


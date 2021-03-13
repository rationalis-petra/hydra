
add_library(expressions OBJECT
                        runtime.cpp
                        lexical_scope.cpp

                        value.cpp
                        object.cpp
                        cons.cpp
                        oper.cpp
                        handler.cpp
                        user_oper.cpp
                        symbol.cpp
                        values.cpp
                        module.cpp
                        foreign.cpp)


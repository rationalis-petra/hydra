
add_library(interpreter OBJECT
                        control.cpp
                        garbage.cpp
                        roots.cpp
                        lexical_scope.cpp
                        runtime.cpp)

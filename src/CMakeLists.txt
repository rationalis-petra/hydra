
add_subdirectory(memory)
add_subdirectory(operations)
add_subdirectory(expressions)
add_subdirectory(types)

add_executable(hydra main.cpp lang.cpp)

target_link_libraries(hydra expressions
                            operations
                            types
                            memory)

target_link_libraries(hydra ffi ltdl)

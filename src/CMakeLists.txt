
add_subdirectory(memory)
add_subdirectory(operations)
add_subdirectory(expressions)
add_subdirectory(types)

add_executable(hydra main.cpp lang.cpp utils.cpp)

target_link_libraries(hydra expressions
                            operations
                            types
                            memory)



if (UseConan)
target_link_libraries(hydra ${CONAN_LIBS})
else ()
target_link_libraries(hydra ffi ltdl pthread)
endif()

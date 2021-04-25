
add_subdirectory(init)
add_subdirectory(interpreter)
add_subdirectory(operations)
add_subdirectory(expressions)
add_subdirectory(types)

add_executable(hydra main.cpp utils.cpp vals.cpp)

target_link_libraries(hydra init
                            interpreter
                            expressions
                            operations
                            types)



if (UseConan)
target_link_libraries(hydra ${CONAN_LIBS})
else ()
find_package(Boost
  1.75.0      
  # asio is header-only??
  COMPONENTS filesystem 
  REQUIRED)    

target_link_libraries(hydra ffi ltdl pthread ${Boost_LIBRARIES})
endif()



# Installation
set(install_dest "bin")
install(TARGETS hydra DESTINATION "${install_dest}")

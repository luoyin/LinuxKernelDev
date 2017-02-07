project(relationanalysis)

add_executable(relationanalysis main.cpp)

install(TARGETS relationanalysis RUNTIME DESTINATION bin)

add_subdirectory(lib)
project(relationanalysis)

add_executable(relationanalysis main.cpp)
add_executable(relation 
	relation.cc 
	lib/CBaseList.cc 
	lib/CRelationHeaderFile.cc
)

install(TARGETS relationanalysis RUNTIME DESTINATION bin)

add_subdirectory(lib)
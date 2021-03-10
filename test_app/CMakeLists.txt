# CMakeList.txt : CMake project for expresscpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (test_app "test_app.cpp")

# TODO: Add tests and install targets if needed.

target_link_libraries(test_app
PRIVATE
	${CONAN_LIBS_BOOST}
	${CONAN_SYSTEM_LIBS_BOOST}
)

target_link_directories(test_app
PRIVATE
	${CONAN_LIB_DIRS_BOOST}
)

target_include_directories(test_app
	PUBLIC
		${CMAKE_SOURCE_DIR}/include
		${CONAN_INCLUDE_DIRS_BOOST}
)

target_precompile_headers(test_app
PUBLIC "stdafx.h"
)
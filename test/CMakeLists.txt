# CMakeList.txt : CMake project for expresscpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (http_test "http_test.cpp")

# TODO: Add tests and install targets if needed.
target_precompile_headers(http_test
	PUBLIC
		<set>
		<array>
		<regex>
		<vector>
		<memory>
		<string>
		<future>
		<functional>
		<unordered_set>
		<unordered_map>
		<initializer_list>
)

target_link_libraries(http_test
PRIVATE
	${CONAN_LIBS_SOCKPP}
	${CONAN_SYSTEM_LIBS_SOCKPP}
)

target_link_directories(http_test
PRIVATE
	${CONAN_LIB_DIRS_SOCKPP}
)

target_include_directories(http_test
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS_SOCKPP}>
		$<INSTALL_INTERFACE:include>
)

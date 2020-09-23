# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("http_test")

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	file(COPY ${CMAKE_SOURCE_DIR}/conanfile.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	set(CMD)
	if(CONAN_REFERENCE)
		list(APPEND CMD "${CONAN_REFERENCE}")
	endif()
	if(CONAN_PROFILE)
		list(APPEND CMD "-pr")
		list(APPEND CMD "${CMAKE_SOURCE_DIR}/${CONAN_PROFILE}")
	endif()
	execute_process(
		COMMAND conan install . ${CMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		RESULT_VARIABLE CONAN_INSTALL_SUCCESS
	)
	if(NOT CONAN_INSTALL_SUCCESS EQUAL 0)
		message(FATAL_ERROR "Error with conan deps")
	endif()
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Include sub-projects.
add_subdirectory ("test")
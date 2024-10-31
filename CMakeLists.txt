cmake_minimum_required(VERSION 3.30)

project(constant_wrapper VERSION 1.0 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL) 
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
	
	include(colors)
	include(pedantic)
	
	if (APPLE)
		set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")
	endif() 
	
	option(CW_TESTS "Enable testing" ON)

	if (CW_TESTS)
		option(CW_COVERAGE "Enable HAMT test-coverage" ON)

		if (CW_COVERAGE)
			include(coverage)
			enable_coverage()
		else()
			message(STATUS "Test coverage measurement is OFF")
		endif()

		add_subdirectory(tests)
	else()
		message(STATUS "Tests are disabled")
	endif()
endif()

add_library(constant_wrapper INTERFACE include/constant_wrapper.hpp)
target_include_directories(constant_wrapper INTERFACE include)
target_compile_features(constant_wrapper INTERFACE cxx_std_23)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(original const.cpp)
target_link_libraries(original constant_wrapper Boost::headers)

cmake_minimum_required( VERSION 3.5.1 )
project( RaytracerWeekend LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set( TODO ON CACHE BOOL "Enable compiletime TODO messages" )

set( CMAKE_BUILD_TYPE Debug )

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME} )

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)
add_executable( ${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS} )

if("${CMAKE_VERSION}" VERSION_LESS 3.8.2)
	set_target_properties(
    ${CMAKE_PROJECT_NAME}
		PROPERTIES
			CXX_STANDARD 14
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS NO
	)
else()
  target_compile_features( ${CMAKE_PROJECT_NAME} PRIVATE cxx_std_14 )
endif()

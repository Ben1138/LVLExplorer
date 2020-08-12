cmake_minimum_required(VERSION 3.1)

#
# BUILD NOTES
#

#[[

Make sure you have LIBSWBF2_INCLUDE and LIBSWBF2_LIB set as environment 
vars pointing to your libSWBF2 headers and libraries respectively, unless
you already have them in your path.  I should get around to adding a proper
install or pkg-config setup for libSWBF2 and all dependant projects.  If you
have issues building w/CMake on Linux or Mac, drop an issue and tag
WHSnyder.

]]



project(LVLExplorer)


#SETTINGS

add_executable(lvlexplorer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
set_property(TARGET lvlexplorer PROPERTY CXX_STANDARD 17)
set_property(TARGET lvlexplorer PROPERTY CXX_STANDARD_REQUIRED ON)



#SOURCES/INCLUDES

include_directories(LVLExplorer)

file(GLOB SOURCE_FILES LVLExplorer/*.cpp)
target_sources(lvlexplorer PUBLIC ${SOURCE_FILES})

target_compile_options(lvlexplorer PUBLIC -DNDEBUG)

find_package(wxWidgets REQUIRED COMPONENTS base core html gl)

if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE})
    target_link_libraries(lvlexplorer PUBLIC ${wxWidgets_LIBRARIES})
else()
	message(ERROR "Couldn't find wxWidgets!")
endif()


set(LIBSWBF2_INCLUDES_DIR $ENV{LIBSWBF2_INCLUDE})
set(LIBSWBF2_LIBS_DIR $ENV{LIBSWBF2_LIB})

if (NOT ${LIBSWBF2_INCLUDES_DIR} EQUAL "")
	target_include_directories(lvlexplorer PUBLIC "${LIBSWBF2_INCLUDES_DIR}")
	target_include_directories(lvlexplorer PUBLIC "${LIBSWBF2_INCLUDES_DIR}/DirectX")
else()
	message(STATUS "LIBSWBF2_INCLUDE environment variable not set.  
				   	Assuming libSWBF2 headers are your include path already.")
endif()


#Redundant/harmless additions to catch underconfigured systems...
if (APPLE)
	include_directories(/usr/local/include)
	target_link_options(lvlexplorer PUBLIC -undefined dynamic_lookup)
	target_link_directories(lvlexplorer PUBLIC /usr/local/lib/)
else()
	target_link_libraries(lvlexplorer PUBLIC -ldl)
	target_link_directories(lvlexplorer PUBLIC /usr/lib/x86_64-linux-gnu/)
endif()


#LINKING

if (NOT ${LIBSWBF2_LIBS_DIR} EQUAL "")
	target_link_directories(lvlexplorer PUBLIC ${LIBSWBF2_LIBS_DIR})
else()
	message(STATUS "LIBSWBF2_LIB environment variable not set.  
				   	Assuming libSWBF2 libraries are in your link path already.")
endif()

target_link_libraries(lvlexplorer PUBLIC pthread SWBF2)
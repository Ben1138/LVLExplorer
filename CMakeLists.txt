cmake_minimum_required(VERSION 3.1)

project(LVLExplorer)


#SETTINGS

add_executable(lvlexplorer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC  -fpermissive")

set_property(TARGET lvlexplorer PROPERTY CXX_STANDARD 20)
set_property(TARGET lvlexplorer PROPERTY CXX_STANDARD_REQUIRED ON)

if (NOT ${CMAKE_BUILD_TYPE} EQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()




#SOURCES

include_directories(LVLExplorer)

if(APPLE)
	include_directories(/usr/local/include)
endif()

file(GLOB SOURCE_FILES LVLExplorer/*.cpp)
target_sources(lvlexplorer PUBLIC ${SOURCE_FILES})


find_package(wxWidgets REQUIRED COMPONENTS net gl core base)

if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE})
    target_link_libraries(lvlexplorer PUBLIC ${wxWidgets_LIBRARIES})
else()
	message(ERROR "Couldn't find wxWidgets!")
endif()

target_include_directories(lvlexplorer PUBLIC /media/will/dual/sharedprojects/libSWBF2/LibSWBF2/)
target_include_directories(lvlexplorer PUBLIC /media/will/dual/sharedprojects/libSWBF2/LibSWBF2/DirectX/)

#set(LIBSWBF2_DIR $ENV{LIBSWBF2DIR})
#message(STATUS "Env val ${LIBSWBF2_DIR}")

#if (NOT ${LIBSWBF2_DIR} EQUAL "")
#	target_include_directories(lvlexplorer PUBLIC "${LIBSWBF2_DIR}")
#endif()


#LINKING

if (APPLE)
	target_link_options(lvlexplorer PUBLIC -undefined dynamic_lookup)
	target_link_directories(lvlexplorer PUBLIC /usr/local/lib/)
else()
	target_link_libraries(lvlexplorer PUBLIC -ldl)
	target_link_directories(lvlexplorer PUBLIC /usr/lib/x86_64-linux-gnu/)
endif()

#if (NOT ${LIBSWBF2_DIR} EQUAL "")
#	target_link_directories(lvlexplorer PUBLIC "${LIBSWBF2_DIR}/build")
#endif()

target_link_directories(lvlexplorer PUBLIC /media/will/dual/sharedprojects/libSWBF2/build/LibSWBF2/)
target_link_libraries(lvlexplorer PUBLIC SWBF2)

target_link_libraries(lvlexplorer PUBLIC fmt pthread)

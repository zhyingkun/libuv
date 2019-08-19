cmake_minimum_required(VERSION 3.0)
project(demo
	VERSION 1.0.0
	# DESCRIPTION "libuv demo"
	# HOMEPAGE_URL "www.zhyingkun.com"
	LANGUAGES C CXX)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "CMakeLists.txt for ${PROJECT_NAME}")
message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

if(APPLE)
	set(CMAKE_C_FLAGS         "-std=gnu89 -Wall -Wextra")
	set(CMAKE_C_FLAGS_DEBUG   "-g")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_C_FLAGS         "-std=gnu89 -Wall -Wextra")
	set(CMAKE_C_FLAGS_DEBUG   "-g")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
elseif(WIN32)
	set(CMAKE_C_FLAGS         "-Wall")
	set(CMAKE_C_FLAGS_DEBUG   "")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

include_directories(../libuv/include)

aux_source_directory(./queue QUEUE_SRC)
source_group(src FILES ${QUEUE_SRC})
add_executable(queue ${QUEUE_SRC})
target_include_directories(queue PRIVATE ../libuv/src)
# target_link_libraries(queue libuv)
set_target_properties(queue PROPERTIES
	FOLDER "demo"
	INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# install(TARGETS ${PROJECT_NAME}
# 	RUNTIME DESTINATION bin
# 	LIBRARY DESTINATION lib
# 	ARCHIVE DESTINATION lib)

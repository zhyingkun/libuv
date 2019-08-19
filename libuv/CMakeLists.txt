cmake_minimum_required(VERSION 3.0)
project(libuv
	VERSION 1.31.0
	# DESCRIPTION "libuv dynamic library"
	# HOMEPAGE_URL "www.zhyingkun.com"
	LANGUAGES C CXX)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "CMakeLists.txt for ${PROJECT_NAME}")
message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

if(APPLE)
	set(CMAKE_C_FLAGS         "-std=gnu89 -Wall -Wextra -fvisibility=hidden -Wstrict-prototypes -Wno-unused-parameter -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_DARWIN_UNLIMITED_SELECT=1 -D_DARWIN_USE_64_BIT_INODE=1")
	set(CMAKE_C_FLAGS_DEBUG   "-g")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_C_FLAGS         "-std=gnu89 -Wall -Wextra -fvisibility=hidden -Wstrict-prototypes -Wno-unused-parameter -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112")
	set(CMAKE_C_FLAGS_DEBUG   "-g")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
elseif(WIN32)
	set(CMAKE_C_FLAGS         "/W4 -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0600")
	set(CMAKE_C_FLAGS_DEBUG   "")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

include_directories(./include)
file(GLOB UV_PUBLIC_HEADERS "./include/*.h")
source_group(include FILES ${UV_PUBLIC_HEADERS})
file(GLOB UV_PUBLIC_UV_HEADERS "./include/uv/*.h")
source_group(include\\uv FILES ${UV_PUBLIC_UV_HEADERS})
include_directories(./src)
file(GLOB UV_PRIVATE_HEADERS "./src/*.h")
source_group(src FILES ${UV_PRIVATE_HEADERS})
if(APPLE)
	include_directories(./src/unix)
	file(GLOB UV_HEADERS_UNIX "./src/unix/*.h")
	source_group(unix FILES ${UV_HEADERS_UNIX})
	# include_directories(./src/unix/macosx)
	# file(GLOB UV_HEADERS_UNIX_MACOSX "./src/unix/macosx/*.h")
	# source_group(macosx FILES ${UV_HEADERS_UNIX_MACOSX})
	list(APPEND UV_PRIVATE_HEADERS ${UV_HEADERS_UNIX})
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	include_directories(./src/unix)
	file(GLOB UV_HEADERS_UNIX "./src/unix/*.h")
	source_group(unix FILES ${UV_HEADERS_UNIX})
	include_directories(./src/unix/linux)
	file(GLOB UV_HEADERS_UNIX_LINUX "./src/unix/linux/*.h")
	source_group(linux FILES ${UV_HEADERS_UNIX_LINUX})
	list(APPEND UV_PRIVATE_HEADERS ${UV_HEADERS_UNIX} ${UV_HEADERS_UNIX_LINUX})
elseif(WIN32)
	include_directories(./src/win)
	file(GLOB UV_HEADERS_WIN "./src/win/*.h")
	source_group(win FILES ${UV_HEADERS_WIN})
	list(APPEND UV_PRIVATE_HEADERS ${UV_HEADERS_WIN})
endif()

aux_source_directory(./src UV_SOURCES)
source_group(src FILES ${UV_SOURCES})
if(APPLE)
	aux_source_directory(./src/unix UV_SOURCES_UNIX)
	source_group(unix FILES ${UV_SOURCES_UNIX})
	aux_source_directory(./src/unix/macosx UV_SOURCES_UNIX_MACOSX)
	source_group(macosx FILES ${UV_SOURCES_UNIX_MACOSX})
	list(APPEND UV_SOURCES ${UV_SOURCES_UNIX} ${UV_SOURCES_UNIX_MACOSX})
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	aux_source_directory(./src/unix UV_SOURCES_UNIX)
	source_group(unix FILES ${UV_SOURCES_UNIX})
	aux_source_directory(./src/unix/linux UV_SOURCES_UNIX_LINUX)
	source_group(linux FILES ${UV_SOURCES_UNIX_LINUX})
	list(APPEND UV_SOURCES ${UV_SOURCES_UNIX} ${UV_SOURCES_UNIX_LINUX})
elseif(WIN32)
	aux_source_directory(./src/win UV_SOURCES_WIN)
	source_group(win FILES ${UV_SOURCES_WIN})
	list(APPEND UV_SOURCES ${UV_SOURCES_WIN})
endif()

add_library(${PROJECT_NAME} SHARED 
	${UV_SOURCES} ${UV_PUBLIC_HEADERS} ${UV_PUBLIC_UV_HEADERS} ${UV_PRIVATE_HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_UV_SHARED=1) # for win make dll
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "uv"
	VERSION ${PROJECT_VERSION}
	SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
	POSITION_INDEPENDENT_CODE ON)

if(APPLE)
	target_link_libraries(${PROJECT_NAME} pthread)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${PROJECT_NAME} pthread dl rt)
elseif(WIN32)
	target_link_libraries(${PROJECT_NAME} advapi32 iphlpapi psapi shell32 user32 userenv ws2_32)
endif()

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES ${UV_PUBLIC_HEADERS} DESTINATION include/libuv)
install(FILES ${UV_PUBLIC_UV_HEADERS} DESTINATION include/libuv/uv)

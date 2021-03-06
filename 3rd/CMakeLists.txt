cmake_minimum_required(VERSION 2.8)

add_definitions(-DUNICODE -D_UNICODE
	-D_CRT_SECURE_NO_DEPRECATE
	-D_CRT_NONSTDC_NO_DEPRECATE
	-Dx86_64
	)

set(libuv_dir ${CMAKE_CURRENT_SOURCE_DIR}/libuv)

# src文件
if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Od /Oy- /MD ")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /Oy- /MTd")	

	set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
	foreach(CompilerFlag ${CompilerFlags})
	  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()

	file(GLOB platform
		${libuv_dir}/src/win/*.*
		)

	source_group(src\\win FILES ${platform} )

	# include相关的文件
	file(GLOB libuvinclude
		${libuv_dir}/include/tree.h
		${libuv_dir}/include/uv.h
		${libuv_dir}/include/uv-errno.h
		${libuv_dir}/include/uv-threadpool.h
		${libuv_dir}/include/uv-version.h
		${libuv_dir}/include/uv-win.h
		)

else()
add_definitions(-DUNICODE -D_GNU_SOURCE -Dansi	)
	file(GLOB platform
		${libuv_dir}/src/unix/linux-core.c
		${libuv_dir}/src/unix/linux-inotify.c
		${libuv_dir}/src/unix/linux-syscalls.c
		${libuv_dir}/src/unix/linux-syscalls.h
		${libuv_dir}/src/unix/async.c
		${libuv_dir}/src/unix/atomic-ops.h
		${libuv_dir}/src/unix/core.c
		${libuv_dir}/src/unix/dl.c
		${libuv_dir}/src/unix/fs.c
		${libuv_dir}/src/unix/getaddrinfo.c
		${libuv_dir}/src/unix/getnameinfo.c
		${libuv_dir}/src/unix/internal.h
		${libuv_dir}/src/unix/loop.c
		${libuv_dir}/src/unix/loop-watcher.c
		${libuv_dir}/src/unix/pipe.c
		${libuv_dir}/src/unix/poll.c
		${libuv_dir}/src/unix/process.c
		${libuv_dir}/src/unix/signal.c
		${libuv_dir}/src/unix/spinlock.h
		${libuv_dir}/src/unix/stream.c
		${libuv_dir}/src/unix/tcp.c
		${libuv_dir}/src/unix/thread.c
		${libuv_dir}/src/unix/timer.c
		${libuv_dir}/src/unix/tty.c
		${libuv_dir}/src/unix/udp.c
		)

	source_group(src\\unix FILES ${platform} )

	file(GLOB libuvinclude
		${libuv_dir}/include/tree.h
		${libuv_dir}/include/uv.h
		${libuv_dir}/include/uv-errno.h
		${libuv_dir}/include/uv-threadpool.h
		${libuv_dir}/include/uv-version.h
		${libuv_dir}/include/uv-unix.h
		)

endif()

source_group(include FILES ${libuvinclude} )


file(GLOB libuvsrc
	${libuv_dir}/src/*.*
		)
source_group(src FILES ${libuvsrc} )

add_library(libuv STATIC ${libuvsrc} ${libuvinclude} ${platform})
set_target_properties(libuv PROPERTIES FOLDER 3rd)
if(WIN32)
	target_compile_definitions(libuv PUBLIC NDIS_IF_MAX_STRING_SIZE=256)
	
else()
	target_link_libraries(libuv pthread)

endif()

include_directories(
	${libuv_dir}/src
	${libuv_dir}/include)
# Created by madmaxoft for static linkage
cmake_minimum_required(VERSION 2.8)
project(luasocket CXX)




set(SRCS
	# socket.core:
	src/luasocket.c
	src/timeout.c
	src/buffer.c
	src/io.c
	src/auxiliar.c
	src/options.c
	src/inet.c
	src/except.c
	src/select.c
	src/tcp.c
	src/udp.c
	src/compat.c

	# mime.core:
	src/mime.c

	# static registering:
	src/static_register.cpp
)




if(WIN32)
	list(APPEND SRCS
		src/wsocket.c
	)
else()
	list(APPEND SRCS
		src/usocket.c
	)
endif()





set(LUASOCKETMODULES
	http.lua
	url.lua
	tp.lua
	ftp.lua
	headers.lua
	smtp.lua
)
set(LUAMODULES
	mime.lua
	socket.lua
	ltn12.lua
)





# Store the Lua files as C++11 string constants:
set(GenLuaFiles)
foreach(LuaFile IN LISTS LUAMODULES LUASOCKETMODULES)
	string(REPLACE ".lua" "" LuaFileBare ${LuaFile})
	file(READ src/${LuaFile} LuaFileContents)
	set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${LuaFile}.cpp)
	file(WRITE ${outfile} "extern \"C\" const char LuaSrc_${LuaFileBare}[] = R\"%%%%(" ${LuaFileContents} ")%%%%\";")
	list(APPEND GenLuaFiles ${outfile})
endforeach()

add_library(luasocket STATIC
	${SRCS}
	${GenLuaFiles}
)

target_link_libraries(luasocket
	lua
)

if(WIN32)
	target_link_libraries(luasocket
		ws2_32
	)
endif()

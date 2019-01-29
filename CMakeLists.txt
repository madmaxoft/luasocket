cmake_minimum_required(VERSION 2.8...4.0)

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
	src/http.lua
	src/url.lua
	src/tp.lua
	src/ftp.lua
	src/headers.lua
	src/smtp.lua
)

set(LUAMODULES
	src/mime.lua
	src/socket.lua
	src/ltn12.lua
)





# Store the Lua files as C++11 string constants:
set(GenLuaFiles)
foreach(LuaFile IN LISTS LUAMODULES LUASOCKETMODULES)
	string(REPLACE ".lua" "" LuaFileBare ${LuaFile})
	string(REPLACE "src/" "" LuaFileBare ${LuaFileBare})
	file(READ ${LuaFile} LuaFileContents)
	set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${LuaFile}.cpp)
	file(WRITE ${outfile} "extern \"C\" const char LuaSrc_${LuaFileBare}[] = R\"%%%%(" ${LuaFileContents} ")%%%%\";")
	list(APPEND GenLuaFiles ${outfile})
endforeach()

source_group("lua" FILES ${LUAMODULES})
source_group("lua/socket" FILES ${LUASOCKETMODULES})





# Static library:
add_library(luasocket-static STATIC
	${SRCS}
	${GenLuaFiles}
	${LUAMODULES}
	${LUASOCKETMODULES}
)

target_link_libraries(luasocket-static
	lua-static
)

if(WIN32)
	target_link_libraries(luasocket-static
		ws2_32
	)
endif()





# Dynamic library:
add_library(luasocket SHARED
	${SRCS}
	${GenLuaFiles}
	${LUAMODULES}
	${LUASOCKETMODULES}
)

target_link_libraries(luasocket
	lua
)

if(WIN32)
	target_link_libraries(luasocket
		ws2_32
	)
endif()

cmake_minimum_required(VERSION 3.1)

project(blogsoft)

# Remove for release
#add_compile_definitions(_DEBUG)

set (CMAKE_CXX_STANDARD 11)

add_executable(blogsoft Main.cpp BlogConfig.cpp EditLog.cpp Article.cpp Index.cpp Tags.cpp RSS.cpp)

if(MSVC)
  target_compile_options(blogsoft PRIVATE /W4 /Od)
else(MSVC)
  target_compile_options(blogsoft PRIVATE -Wall -Wextra -pedantic -O0 -fno-lto)
endif(MSVC)




install(TARGETS blogsoft RUNTIME DESTINATION bin)

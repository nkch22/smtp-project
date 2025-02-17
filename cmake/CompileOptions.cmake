# Common compile options for C++

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# https://clang.llvm.org/docs/DiagnosticsReference.html
#add_compile_options(-Wall -Wextra -Wpedantic -Werror -g)

# Turn warnings into errors
#add_compile_options(-Wno-language-extension-token)

#add_compile_options(-Wno-error=unused-command-line-argument)

# libc++
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

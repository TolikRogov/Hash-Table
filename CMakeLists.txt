cmake_minimum_required(VERSION 3.5)

project(HashTable LANGUAGES CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ASM_NASM_FLAGS "-f elf64")

set(DEBUG OFF)
set(BASE OFF)
set(FSANITIZER OFF)

if (${FSANITIZER})
    set(FSANITIZER_FLAGS -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
else()
    set(FSANITIZER_FLAGS)
endif()

if (${DEBUG})
    add_compile_definitions(DEBUG)
endif()

if (${BASE})
    add_compile_definitions(BASE)
endif()

set(DEBUG_FLAGS ${FSANITIZER_FLAGS} -std=c++17 -O3 -z noexecstack -mavx2 -ggdb3 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fPIC -fPIE -fno-omit-frame-pointer -fPIE -Werror=vla -Wno-write-strings)

add_executable(${PROJECT_NAME})
add_subdirectory(source)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Debug>:${DEBUG_FLAGS}>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:${DEBUG_FLAGS}>>
)

set_source_files_properties(source/crc32Asm.asm PROPERTIES
    COMPILE_FLAGS "${CMAKE_ASM_NASM_FLAGS}"
)

target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:${DEBUG_FLAGS}>)

cmake_minimum_required(VERSION 3.4.1)

file(GLOB LOCAL_FILES
    "src/main/cpp/*.h"
    "src/main/cpp/*.cpp"
)

add_library(native-lib SHARED ${LOCAL_FILES})

target_link_libraries(native-lib log stdc++ android EGL GLESv2 dl)
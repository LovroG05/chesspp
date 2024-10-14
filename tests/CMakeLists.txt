set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Users/admin/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic")


find_package(Catch2 CONFIG REQUIRED)
#target_link_libraries(main PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
file(GLOB_RECURSE SRC_FILES ../src/*.cpp)
add_executable(tests VerifyMoveTest.cpp ${SRC_FILES})
target_include_directories(tests PUBLIC ../include/)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
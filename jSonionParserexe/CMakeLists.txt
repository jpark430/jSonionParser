# CMakeList.txt : CMake project for jSonionParser, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (jSonionParserexe "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET jSonionParserexe PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(jSonionParserexe PRIVATE jSonionParserlib)

add_custom_command(TARGET jSonionParserexe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/jSonionfiles"
        "$<TARGET_FILE_DIR:jSonionParserexe>/jSonionfiles"
)
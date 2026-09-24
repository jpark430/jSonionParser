# CMakeList.txt : CMake project for jSonionParser, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(jSonionParserlib STATIC "jSonionParser.cpp" "jSonionParser.h" "jSonion.h" "jSonion.cpp" "jSonionSerializer.h" "jSonionSerializer.cpp" "jSonionExceptions.cpp" "jSonionExceptions.h" "jSonionStuff.h" "jSonionStuff.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET jSonionParserlib PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_include_directories(jSonionParserlib PUBLIC .)
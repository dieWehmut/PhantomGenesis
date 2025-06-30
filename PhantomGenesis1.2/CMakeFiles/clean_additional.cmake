# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\PhantomGenesis_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PhantomGenesis_autogen.dir\\ParseCache.txt"
  "PhantomGenesis_autogen"
  )
endif()

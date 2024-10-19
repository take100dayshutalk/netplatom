# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\mainobject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mainobject_autogen.dir\\ParseCache.txt"
  "databasepool\\CMakeFiles\\databasepool_autogen.dir\\AutogenUsed.txt"
  "databasepool\\CMakeFiles\\databasepool_autogen.dir\\ParseCache.txt"
  "databasepool\\databasepool_autogen"
  "mainobject_autogen"
  "server\\CMakeFiles\\server_autogen.dir\\AutogenUsed.txt"
  "server\\CMakeFiles\\server_autogen.dir\\ParseCache.txt"
  "server\\server_autogen"
  )
endif()

# Copyright 2017-2018 the DCSS authors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Source files.
set(LIB_SRC
  ${SOURCE_DIR}/bit_map.cpp
  ${SOURCE_DIR}/cmds.cpp
  ${SOURCE_DIR}/dcss_conf.cpp
  ${SOURCE_DIR}/dcss_network.cpp
  ${SOURCE_DIR}/dcss_node_com.cpp
  ${SOURCE_DIR}/shell.cpp
  ${SOURCE_DIR}/uint160.cpp

  ${SOURCE_DIR}/dht/address.cpp

  CACHE
  INTERNAL
  ""
  FORCE
)

# Generate config.h (with compile-time information).
configure_file(${SOURCE_DIR}/config.in ${GENERATE_DIR}/config.h @ONLY)

###########
# Libraries
###########

# Dependencies.
find_package(Readline         REQUIRED)
find_package(JsonRpcCppClient REQUIRED)
find_package(EasyLogging      REQUIRED)
find_package(QUADIRON         REQUIRED)

get_property(QUADIRON_INCLUDE_DIRS
  TARGET   QUADIRON::static
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES
)

set(OBJECT_SYS_INCLUDES
  ${GENERATE_DIR}
  ${JsonRpcCppClient_INCLUDE_DIRS}
  ${EasyLogging_INCLUDE_DIRS}
  ${Readline_INCLUDE_DIRS}
  ${QUADIRON_INCLUDE_DIRS}
)

set(OBJECT_INCLUDES
  ${SOURCE_DIR}
)

# Build an Object Library (can be reused for both static and dynamic libs).
add_library(${OBJECT_LIB} OBJECT ${LIB_SRC})
add_coverage(${OBJECT_LIB})
set_property(TARGET ${OBJECT_LIB} PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(${OBJECT_LIB} SYSTEM PUBLIC ${OBJECT_SYS_INCLUDES})
target_include_directories(${OBJECT_LIB}        PUBLIC ${OBJECT_INCLUDES})
target_compile_definitions(${OBJECT_LIB}
  PUBLIC HAVE_READLINE=1
)
add_dependencies(${OBJECT_LIB}
  ${JSON_RPC_STUB}
)

# Dynamic library.
add_library(${SHARED_LIB} SHARED $<TARGET_OBJECTS:${OBJECT_LIB}>)
# Static library.
add_library(${STATIC_LIB} STATIC $<TARGET_OBJECTS:${OBJECT_LIB}>)

foreach(lib ${SHARED_LIB} ${STATIC_LIB})
  set_target_properties(${lib} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
  target_include_directories(${lib} SYSTEM PUBLIC ${OBJECT_SYS_INCLUDES})
  target_include_directories(${lib}        PUBLIC ${OBJECT_INCLUDES})
  target_compile_definitions(${lib}
    PUBLIC $<TARGET_PROPERTY:${OBJECT_LIB},COMPILE_DEFINITIONS>
  )

  target_link_libraries(${lib}
    ${Readline_LIBRARIES}
    ${JsonRpcCppClient_LIBRARIES}
    ${EasyLogging_LIBRARIES}
    QUADIRON::static
  )
endforeach()

############
# Executable
############

set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})

add_executable(${EXECUTABLE_NAME}
  ${SOURCE_DIR}/main.cpp
)
add_coverage(${EXECUTABLE_NAME})

# Link librairies.
target_link_libraries(${EXECUTABLE_NAME}
  ${STATIC_LIB}
)

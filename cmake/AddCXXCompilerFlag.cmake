# - Adds a compiler flag if it is supported by the compiler
#
# This function checks that the supplied compiler flag is supported and then
# adds it to the corresponding compiler flags
#
#  add_cxx_compiler_flag(<FLAG> [<VARIANT>])
#
# - Example
#
# include(AddCXXCompilerFlag)
# add_cxx_compiler_flag(-Wall)
# add_cxx_compiler_flag(-no-strict-aliasing RELEASE)
# Requires CMake 2.6+

if(__add_cxx_compiler_flag)
  return()
endif()
set(__add_cxx_compiler_flag INCLUDED)

include(CheckCXXCompilerFlag)

function(add_cxx_compiler_flag FLAG)
  string(TOUPPER "HAVE_CXX_FLAG_${FLAG}" SANITIZED_FLAG)
  string(REPLACE "+" "X" SANITIZED_FLAG ${SANITIZED_FLAG})
  string(REGEX REPLACE "[^A-Za-z_0-9]" "_" SANITIZED_FLAG ${SANITIZED_FLAG})
  string(REGEX REPLACE "_+" "_" SANITIZED_FLAG ${SANITIZED_FLAG})
  check_cxx_compiler_flag(${SANITIZED_FLAG} NO_DIAGNOSTICS_PRODUCED)
  if(${NO_DIAGNOSTICS_PRODUCED})
    set(VARIANT ${ARGV1})
    if(ARGV1)
      string(REGEX REPLACE "[^A-Za-z_0-9]" "_" VARIANT "${VARIANT}")
      string(TOUPPER "_${VARIANT}" VARIANT)
    endif()
    set(CMAKE_CXX_FLAGS${VARIANT} "${CMAKE_CXX_FLAGS${VARIANT}} ${FLAG}" PARENT_SCOPE)
  endif()
endfunction()


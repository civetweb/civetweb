# Determine if we should print to the output
if (CIVETWEB_ENABLE_THIRD_PARTY_OUTPUT)
  set(THIRD_PARTY_LOGGING 0)
else()
  set(THIRD_PARTY_LOGGING 1)
endif()

# We use the check unit testing framework for our C unit tests
include(ExternalProject)
ExternalProject_Add(check
  DEPENDS c-library
  URL "https://downloads.sourceforge.net/project/check/check/${CIVETWEB_CHECK_VERSION}/check-${CIVETWEB_CHECK_VERSION}.tar.gz"
  URL_MD5 ${CIVETWEB_CHECK_MD5_HASH}
  PREFIX "${CMAKE_BINARY_DIR}/third_party"
  BUILD_IN_SOURCE 1
  PATCH_COMMAND ${CMAKE_COMMAND}
    -DSOURCE_DIR=<SOURCE_DIR>
    -DBINARY_DIR=<BINARY_DIR>
    -DINSTALL_DIR=<INSTALL_DIR>
    -DVERSION=${CIVETWEB_CHECK_VERSION}
    -P ${CMAKE_SOURCE_DIR}/cmake/check/patch.cmake
  CMAKE_ARGS
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
    "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
  LOG_DOWNLOAD ${THIRD_PARTY_LOGGING}
  LOG_UPDATE ${THIRD_PARTY_LOGGING}
  LOG_CONFIGURE ${THIRD_PARTY_LOGGING}
  LOG_BUILD ${THIRD_PARTY_LOGGING}
  LOG_TEST ${THIRD_PARTY_LOGGING}
  LOG_INSTALL ${THIRD_PARTY_LOGGING})
ExternalProject_Get_Property(check INSTALL_DIR)
set(CHECK_INSTALL_DIR ${INSTALL_DIR})
unset(INSTALL_DIR)
link_directories("${CHECK_INSTALL_DIR}/lib")
include_directories("${CHECK_INSTALL_DIR}/include")
if (WIN32 AND MINGW)
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};${CHECK_INSTALL_DIR}/lib/libcheck.a")
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};${CHECK_INSTALL_DIR}/lib/libcompat.a")
elseif (WIN32)
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};${CHECK_INSTALL_DIR}/lib/check.lib")
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};${CHECK_INSTALL_DIR}/lib/compat.lib")
else()
  set(CHECK_LIBRARIES "${CHECK_INSTALL_DIR}/lib/libcheck.a")
endif()
find_package(LibM)
if (LIBM_FOUND)
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};LIBM::LIBM")
endif()
find_package(LibRt)
if (LIBRT_FOUND)
  set(CHECK_LIBRARIES "${CHECK_LIBRARIES};LIBRT::LIBRT")
endif()

# Build the C unit tests
add_library(public-c-unit-tests STATIC public.c)
if (BUILD_SHARED_LIBS)
  target_compile_definitions(public-c-unit-tests PRIVATE CIVETWEB_DLL_IMPORTS)
endif()
target_include_directories(
  public-c-unit-tests PUBLIC
  ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(public-c-unit-tests c-library ${CHECK_LIBRARIES})
add_dependencies(public-c-unit-tests check)
add_library(private-c-unit-tests STATIC private.c)
target_include_directories(
  private-c-unit-tests PUBLIC
  ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(private-c-unit-tests ${CHECK_LIBRARIES})
add_dependencies(private-c-unit-tests check)
add_executable(civetweb-c-unit-test main.c)
target_link_libraries(civetweb-c-unit-test public-c-unit-tests private-c-unit-tests ${CHECK_LIBRARIES})
add_dependencies(civetweb-c-unit-test check)

# A macro for adding tests
macro(civetweb_add_test suite test_case)
  set(test "test-${suite}-${test_case}")
  string(TOLOWER "${test}" test)
  string(REGEX REPLACE "[^-A-Za-z0-9]" "-" test "${test}")
  add_test(
    NAME ${test}
    COMMAND civetweb-c-unit-test "--suite=${suite}" "--test-case=${test_case}")
  string(REPLACE ";" "\\;" test_path "$ENV{PATH}")
  set_tests_properties(${test} PROPERTIES
    ENVIRONMENT "PATH=${test_path}\\;$<TARGET_FILE_DIR:c-library>")
endmacro(civetweb_add_test)

# Public API tests
civetweb_add_test(Public Cookies)

# Private API tests
civetweb_add_test(Private "HTTP Message")

# Add the coverage command(s)
if (${CMAKE_BUILD_TYPE} MATCHES "[Cc]overage")
  find_program(GCOV_EXECUTABLE gcov)
  find_program(LCOV_EXECUTABLE lcov)
  find_program(GENHTML_EXECUTABLE genhtml)
  find_program(CTEST_EXECUTABLE ctest)
  if (GCOV_EXECUTABLE AND LCOV_EXECUTABLE AND GENHTML_EXECUTABLE AND CTEST_EXECUTABLE AND HAVE_C_FLAG_COVERAGE)
    add_custom_command(
      OUTPUT ${CMAKE_BINARY_DIR}/lcov/index.html
      COMMAND ${LCOV_EXECUTABLE} -q -z -d .
      COMMAND ${LCOV_EXECUTABLE} -q --no-external -c -b "${CMAKE_SOURCE_DIR}" -d . -o before.lcov -i
      COMMAND ${CTEST_EXECUTABLE} --force-new-ctest-process
      COMMAND ${LCOV_EXECUTABLE} -q --no-external -c -b "${CMAKE_SOURCE_DIR}" -d . -o after.lcov
      COMMAND ${LCOV_EXECUTABLE} -q -a before.lcov -a after.lcov --output-file final.lcov
      COMMAND ${LCOV_EXECUTABLE} -q -r final.lcov "'${CMAKE_SOURCE_DIR}/test/*'" -o final.lcov
      COMMAND ${GENHTML_EXECUTABLE} final.lcov -o lcov --demangle-cpp --sort -p "${CMAKE_SOURCE_DIR}" -t benchmark
      DEPENDS civetweb-c-unit-test
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Running LCOV"
    )
    add_custom_target(coverage
      DEPENDS ${CMAKE_BINARY_DIR}/lcov/index.html
      COMMENT "LCOV report at lcov/index.html"
    )
    message(STATUS "Coverage command added")
  else()
    if (HAVE_C_FLAG_COVERAGE)
      set(C_FLAG_COVERAGE_MESSAGE supported)
    else()
      set(C_FLAG_COVERAGE_MESSAGE unavailable)
    endif()
    message(WARNING
      "Coverage command not available:\n"
      "  gcov: ${GCOV_EXECUTABLE}\n"
      "  lcov: ${LCOV_EXECUTABLE}\n"
      "  genhtml: ${GENHTML_EXECUTABLE}\n"
      "  ctest: ${CTEST_EXECUTABLE}\n"
      "  --coverage flag: ${C_FLAG_COVERAGE_MESSAGE}")
  endif()
endif()

include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(victronSolarMonitor_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(victronSolarMonitor_setup_options)
  option(victronSolarMonitor_ENABLE_HARDENING "Enable hardening" ON)
  option(victronSolarMonitor_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    victronSolarMonitor_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    victronSolarMonitor_ENABLE_HARDENING
    OFF)

  victronSolarMonitor_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR victronSolarMonitor_PACKAGING_MAINTAINER_MODE)
    option(victronSolarMonitor_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(victronSolarMonitor_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(victronSolarMonitor_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(victronSolarMonitor_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(victronSolarMonitor_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(victronSolarMonitor_ENABLE_PCH "Enable precompiled headers" OFF)
    option(victronSolarMonitor_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(victronSolarMonitor_ENABLE_IPO "Enable IPO/LTO" ON)
    option(victronSolarMonitor_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(victronSolarMonitor_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(victronSolarMonitor_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(victronSolarMonitor_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(victronSolarMonitor_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(victronSolarMonitor_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(victronSolarMonitor_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(victronSolarMonitor_ENABLE_PCH "Enable precompiled headers" OFF)
    option(victronSolarMonitor_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      victronSolarMonitor_ENABLE_IPO
      victronSolarMonitor_WARNINGS_AS_ERRORS
      victronSolarMonitor_ENABLE_USER_LINKER
      victronSolarMonitor_ENABLE_SANITIZER_ADDRESS
      victronSolarMonitor_ENABLE_SANITIZER_LEAK
      victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED
      victronSolarMonitor_ENABLE_SANITIZER_THREAD
      victronSolarMonitor_ENABLE_SANITIZER_MEMORY
      victronSolarMonitor_ENABLE_UNITY_BUILD
      victronSolarMonitor_ENABLE_CLANG_TIDY
      victronSolarMonitor_ENABLE_CPPCHECK
      victronSolarMonitor_ENABLE_COVERAGE
      victronSolarMonitor_ENABLE_PCH
      victronSolarMonitor_ENABLE_CACHE)
  endif()

  victronSolarMonitor_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (victronSolarMonitor_ENABLE_SANITIZER_ADDRESS OR victronSolarMonitor_ENABLE_SANITIZER_THREAD OR victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(victronSolarMonitor_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(victronSolarMonitor_global_options)
  if(victronSolarMonitor_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    victronSolarMonitor_enable_ipo()
  endif()

  victronSolarMonitor_supports_sanitizers()

  if(victronSolarMonitor_ENABLE_HARDENING AND victronSolarMonitor_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED
       OR victronSolarMonitor_ENABLE_SANITIZER_ADDRESS
       OR victronSolarMonitor_ENABLE_SANITIZER_THREAD
       OR victronSolarMonitor_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${victronSolarMonitor_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED}")
    victronSolarMonitor_enable_hardening(victronSolarMonitor_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(victronSolarMonitor_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(victronSolarMonitor_warnings INTERFACE)
  add_library(victronSolarMonitor_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  victronSolarMonitor_set_project_warnings(
    victronSolarMonitor_warnings
    ${victronSolarMonitor_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(victronSolarMonitor_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    victronSolarMonitor_configure_linker(victronSolarMonitor_options)
  endif()

  include(cmake/Sanitizers.cmake)
  victronSolarMonitor_enable_sanitizers(
    victronSolarMonitor_options
    ${victronSolarMonitor_ENABLE_SANITIZER_ADDRESS}
    ${victronSolarMonitor_ENABLE_SANITIZER_LEAK}
    ${victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED}
    ${victronSolarMonitor_ENABLE_SANITIZER_THREAD}
    ${victronSolarMonitor_ENABLE_SANITIZER_MEMORY})

  set_target_properties(victronSolarMonitor_options PROPERTIES UNITY_BUILD ${victronSolarMonitor_ENABLE_UNITY_BUILD})

  if(victronSolarMonitor_ENABLE_PCH)
    target_precompile_headers(
      victronSolarMonitor_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(victronSolarMonitor_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    victronSolarMonitor_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(victronSolarMonitor_ENABLE_CLANG_TIDY)
    victronSolarMonitor_enable_clang_tidy(victronSolarMonitor_options ${victronSolarMonitor_WARNINGS_AS_ERRORS})
  endif()

  if(victronSolarMonitor_ENABLE_CPPCHECK)
    victronSolarMonitor_enable_cppcheck(${victronSolarMonitor_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(victronSolarMonitor_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    victronSolarMonitor_enable_coverage(victronSolarMonitor_options)
  endif()

  if(victronSolarMonitor_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(victronSolarMonitor_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(victronSolarMonitor_ENABLE_HARDENING AND NOT victronSolarMonitor_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR victronSolarMonitor_ENABLE_SANITIZER_UNDEFINED
       OR victronSolarMonitor_ENABLE_SANITIZER_ADDRESS
       OR victronSolarMonitor_ENABLE_SANITIZER_THREAD
       OR victronSolarMonitor_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    victronSolarMonitor_enable_hardening(victronSolarMonitor_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()

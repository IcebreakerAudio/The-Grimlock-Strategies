# ============================================================================
# Compiler and Build Configuration Defaults
# ============================================================================

# C++23 compiler check function (to be called after project() command)
function(check_cxx23_support)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-std=c++23 COMPILER_SUPPORTS_CXX23)
    if(NOT COMPILER_SUPPORTS_CXX23 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(WARNING "Compiler may not fully support C++23. Build may fail.")
    endif()
endfunction()

# IDE Organization
# Adds all the module sources so they appear correctly in the IDE
# Must be set before JUCE is added as a sub-dir (or any targets are made)
# https://github.com/juce-framework/JUCE/commit/6b1b4cf7f6b1008db44411f2c8887d71a3348889
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Creates a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

# ============================================================================
# Runtime Library Configuration
# ============================================================================

# Static runtime please
# See https://github.com/sudara/pamplejuce/issues/111
if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")
endif ()

# ============================================================================
# Compiler Diagnostics
# ============================================================================

# Color our warnings and errors
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif ()

# ============================================================================
# Target-Specific Compiler Options (requires SharedCode target to exist)
# ============================================================================

# This function should be called after SharedCode target is created
function(apply_shared_code_defaults)
    # Optimization flags
    if (MSVC)
        # fast math and better simd support in RELEASE
        # https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170#fast
        target_compile_options(SharedCode INTERFACE $<$<CONFIG:RELEASE>:/fp:fast>)
        
        # Tell MSVC to properly report what c++ version is being used
        target_compile_options(SharedCode INTERFACE /Zc:__cplusplus)
    else ()
        # See the implications here:
        # https://stackoverflow.com/q/45685487
        target_compile_options(SharedCode INTERFACE $<$<CONFIG:RELEASE>:-Ofast>)
        target_compile_options(SharedCode INTERFACE $<$<CONFIG:RelWithDebInfo>:-Ofast>)
    endif ()

    # C++23, please
    target_compile_features(SharedCode INTERFACE cxx_std_23)
endfunction()
#
# F E T C H   L E D G E R   B E N C H M A R K S
#
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(fetch-chain)

# CMake configuration
include(${FETCH_ROOT_CMAKE_DIR}/BuildTools.cmake)

# Compiler Configuration
setup_compiler()

# ------------------------------------------------------------------------------
# Benchmark Targets
# ------------------------------------------------------------------------------

add_fetch_gbench(chain-benchmarks fetch-chain .)

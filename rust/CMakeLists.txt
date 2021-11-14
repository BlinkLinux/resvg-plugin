
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL DEBUG)
  set(RUST_TARGET_DIR target/debug)
  set(RUST_BUILD_FLAG)
else()
  set(RUST_TARGET_DIR target/release)
  set(RUST_BUILD_FLAG --release)
endif()

set(RESVG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resvg)
set(RESVG_LIB "${RESVG_DIR}/${RUST_TARGET_DIR}/libresvg.so")
add_custom_command(
    OUTPUT "${RESVG_LIB}"
    COMMAND cargo build ${RUST_BUILD_FLAG} --workspace
    DEPENDS ${RESVG_DIR}/src/lib.rs
    ${RESVG_DIR}/c-api/lib.rs
    WORKING_DIRECTORY "${RESVG_DIR}"
)
add_custom_target(resvg-static-generator DEPENDS "${RESVG_LIB}")
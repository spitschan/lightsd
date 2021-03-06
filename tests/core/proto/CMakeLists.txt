INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_CORE_LIBRARY(
    test_core_proto STATIC
    ${LIGHTSD_SOURCE_DIR}/core/jsonrpc.c
    ${LIGHTSD_SOURCE_DIR}/core/stats.c
    ${LIGHTSD_SOURCE_DIR}/core/utils.c
    ${LIGHTSD_SOURCE_DIR}/lifx/bulb.c
    ${LIGHTSD_SOURCE_DIR}/lifx/tagging.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests_shims.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests_utils.c
)

FUNCTION(ADD_PROTO_TEST TEST_SOURCE)
    ADD_TEST_FROM_C_SOURCES(${TEST_SOURCE} test_core_proto)
ENDFUNCTION()

FILE(GLOB TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "test_*.c")
FOREACH(TEST ${TESTS})
    ADD_PROTO_TEST(${TEST})
ENDFOREACH()

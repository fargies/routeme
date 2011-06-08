
if (TESTS)

    set(test_SRCS
        cunit_main.c
        test_rule.c test_manager.c test_config.c)

    add_executable(tester ${test_SRCS})
    target_link_libraries(tester rme ${CUNIT_LIBRARIES})

    set(TESTS test_config test_manager test_rule)

    foreach(test ${TESTS})
        add_test(${test} ${CMAKE_CURRENT_BUILD_DIR}/tester ${test})
    endforeach()

    include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/src
        ${CMAKE_BINARY_DIR}
        ${GLIB_INCLUDE_DIRS}
        ${GOBJECT_INCLUDE_DIRS})

endif()


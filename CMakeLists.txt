set(DUMMY_SRCS
    dummy.cpp options.cpp
    # XXX These should be in a library.
    ${P4C_SOURCE_DIR}/test/gtest/helpers.cpp)

add_executable(p4dummy ${DUMMY_SRCS})
target_link_libraries(p4dummy ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
add_dependencies(p4dummy frontend midend gtestp4c)

add_custom_target(
  linkdummy_out COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/p4dummy
                        ${P4C_BINARY_DIR}/p4dummy
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${P4C_BINARY_DIR}/p4include
          ${CMAKE_CURRENT_BINARY_DIR}/p4include
)

add_dependencies(p4c_driver linkdummy_out)

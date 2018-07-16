cmake_minimum_required(VERSION 3.2)

set(BINARY_NAME package_manager)

add_executable(${BINARY_NAME} main.cpp)

target_include_directories(${BINARY_NAME} PRIVATE ../include)

set_target_properties(
  ${BINARY_NAME}
  PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install(TARGETS ${BINARY_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT vladimir.englisbox@gmail.com)

include(CPack)
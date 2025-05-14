file(GLOB SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp"
    "include/Komma-runner/*.hpp"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(Komma-runner SHARED ${SOURCES})
target_link_libraries(Komma-runner
    LINK_PUBLIC sc-memory
    LINK_PUBLIC sc-config
)

target_include_directories(Komma-runner
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(Komma "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(Komma LINK_PRIVATE Komma-runner)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(Komma-runner)
    target_clangformat_setup(Komma)
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()

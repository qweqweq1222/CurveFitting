set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_view_zoom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/choosefiledialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/choosesplinedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/savedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointsSelecter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointsStorage.cpp
)

set(SOURCES_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/lib_functions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_view_zoom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/choosefiledialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/choosesplinedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/savedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointsSelecter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointsStorage.cpp
)

target_sources(interface PRIVATE ${SOURCES})
target_sources(Spline PRIVATE ${SOURCES_LIB})
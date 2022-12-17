set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AkimaSplineGSL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LinearGSL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubicSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SteffenSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Generator.cpp
)

set(SOURCES_GUI
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AkimaSplineGSL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubicSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SteffenSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LinearGSL.cpp
)

set(SOURCES_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AkimaSplineGSL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubicSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SteffenSpline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LinearGSL.cpp
)
target_sources(interface PRIVATE ${SOURCES_GUI})
target_sources(Spline PRIVATE ${SOURCES_LIB})
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)

include_directories(
  ./include
  ./forms
)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(MOC_SOURCE
  ./include/mainwindow.h
  ./include/graphics_view_zoom.h
  ./include/choosefiledialog.h
  ./include/choosesplinedialog.h
  ./include/savedialog.h
)

set(UI_SOURCE
  ./forms/mainwindow.ui
  ./forms/choosefiledialog.ui
  ./forms/choosesplinedialog.ui
  ./forms/savedialog.ui
)

qt5_wrap_cpp(MOC_HEADERS ${MOC_SOURCE})
qt5_wrap_ui(UI_HEADERS ${UI_SOURCE})

find_package(Qt5 COMPONENTS Widgets Gui Charts REQUIRED)
add_executable(interface ${MOC_HEADERS} ${UI_HEADERS})
  
add_library(Spline ${MOC_HEADERS} ${UI_HEADERS})
add_library(CurveFit::Spline ALIAS Spline) 

include(GNUInstallDirs)
file(GLOB INCLUDE_GUI "./include/*.h")
file(GLOB INCLUDE_CURVEFIT "../include/*.h")
set( ALL_HEADERS
  ${MOC_SOURCE} 
  ${UI_SOURCE}
  ${INCLUDE_GUI}
  ${INCLUDE_CURVEFIT} 
)
set_target_properties(Spline PROPERTIES PUBLIC_HEADER "${ALL_HEADERS}")
install( 
  TARGETS Spline
  EXPORT CurveFitTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Spline
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Spline.pc.in Spline.pc @ONLY)

install(
  EXPORT CurveFitTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CurveFit
  NAMESPACE CurveFit::
  FILE CurveFitTargets.cmake # Not sure if this is still needed
  )

include(CMakePackageConfigHelpers)
configure_package_config_file( 
  "Config.cmake.in" 
  "CurveFitConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CurveFit
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
)
  

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/CurveFitConfigVersion.cmake
  VERSION 1.0.0
  COMPATIBILITY SameMajorVersion
  )

### Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/CurveFitConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CurveFitConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CurveFit"
)


find_package(GSL REQUIRED)
find_package(TBB REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(interface PRIVATE Qt5::Widgets Qt5::Gui Qt5::Charts)
target_link_libraries(interface PRIVATE GSL::gsl GSL::gslcblas)
target_link_libraries(interface PRIVATE TBB::tbb)
target_link_libraries(interface PRIVATE Eigen3::Eigen)

target_link_libraries(Spline Qt5::Widgets Qt5::Gui Qt5::Charts)
target_link_libraries(Spline GSL::gsl GSL::gslcblas)
target_link_libraries(Spline TBB::tbb)
target_link_libraries(Spline Eigen3::Eigen)

set_target_properties(
    PROPERTIES
    CXX_STANDARD 17
)
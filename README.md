# CurveFitting
git clone ...<br />
cd CurveFitting <br />
mkdir build && cd build<br />
sudo make install<br />

find_package(CurveFit COMPONENTS Spline REQUIRED)<br />
find_package(Threads REQUIRED)<br />
find_package(GSL REQUIRED)<br />
find_package(Qt5 COMPONENTS Widgets Gui Charts REQUIRED)<br />
find_package(TBB REQUIRED)<br />
find_package(Eigen3 3.3 REQUIRED no_module)<br />

target_link_libraries(${CMAKE_PROJECT_NAME} CurveFit::Spline)

#include <Spline/lib_functions.h>

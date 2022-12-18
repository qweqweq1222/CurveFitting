# CurveFitting
git clone ...
mkdir build && cd build
sudo make install

find_package(CurveFit COMPONENTS Spline REQUIRED)<br />
find_package(Threads REQUIRED)<br />
find_package(GSL REQUIRED)<br />
find_package(Qt5 COMPONENTS Widgets Gui Charts REQUIRED)<br />
find_package(TBB REQUIRED)<br />
find_package(Eigen3 3.3 REQUIRED no_module)<br />

target_link_libraries(${CMAKE_PROJECT_NAME} CurveFit::Spline)

# CurveFitting
git clone ...
mkdir build && cd build
sudo make install

find_package(CurveFit COMPONENTS Spline REQUIRED)
find_package(Threads REQUIRED)
find_package(GSL REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui Charts REQUIRED)
find_package(TBB REQUIRED)
find_package(Eigen3 3.3 REQUIRED no_module)

target_link_libraries(${CMAKE_PROJECT_NAME} CurveFit::Spline)

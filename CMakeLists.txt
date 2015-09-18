cmake_minimum_required(VERSION 2.8)
project( readvideo )


# ---[ Find Boost ]
# Cmake has its own "cmake-3.2/Modules/FindBoost.cmake". So you don't has to get one by yourself.
# Set BOOST_ROOT as environment variable before run cmake.
add_definitions(-DBOOST_ALL_NO_LIB)

# 使用静态boost库，且这些库是依赖shared C运行时库的。
set( Boost_USE_STATIC_LIBS ON )         # only find static libs
set( Boost_USE_MULTITHREADED  ON )
set( Boost_USE_STATIC_RUNTIME  OFF )
find_package( Boost 1.46  REQUIRED  COMPONENTS  filesystem  system )    # 注意：这里要写具体使用了哪些Boost的component

include_directories(${Boost_INCLUDE_DIRS})



# 使用静态链接
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#set(OpenCV_STATIC ON)

# 使用动态链接
set(OpenCV_STATIC OFF)

# 需要系统中有OpenCV_DIR这个变量，该变量路径下需要有OpenCVConfig.cmake, OpenCVConfig-version.cmake
# 例如：可以在cmd中 set OpenCV_DIR=F:\3rdParty\opencv-2.4.8-win\build
find_package( OpenCV REQUIRED )

add_executable( video2image video2image.cpp )
target_link_libraries( video2image ${OpenCV_LIBS} ${Boost_LIBRARIES})
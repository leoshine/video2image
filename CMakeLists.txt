cmake_minimum_required(VERSION 2.8)
project( readvideo )


# ---[ Find Boost ]
# Cmake has its own "cmake-3.2/Modules/FindBoost.cmake". So you don't has to get one by yourself.
# Set BOOST_ROOT as environment variable before run cmake.
add_definitions(-DBOOST_ALL_NO_LIB)

# ʹ�þ�̬boost�⣬����Щ��������shared C����ʱ��ġ�
set( Boost_USE_STATIC_LIBS ON )         # only find static libs
set( Boost_USE_MULTITHREADED  ON )
set( Boost_USE_STATIC_RUNTIME  OFF )
find_package( Boost 1.46  REQUIRED  COMPONENTS  filesystem  system )    # ע�⣺����Ҫд����ʹ������ЩBoost��component

include_directories(${Boost_INCLUDE_DIRS})



# ʹ�þ�̬����
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#set(OpenCV_STATIC ON)

# ʹ�ö�̬����
set(OpenCV_STATIC OFF)

# ��Ҫϵͳ����OpenCV_DIR����������ñ���·������Ҫ��OpenCVConfig.cmake, OpenCVConfig-version.cmake
# ���磺������cmd�� set OpenCV_DIR=F:\3rdParty\opencv-2.4.8-win\build
find_package( OpenCV REQUIRED )

add_executable( video2image video2image.cpp )
target_link_libraries( video2image ${OpenCV_LIBS} ${Boost_LIBRARIES})
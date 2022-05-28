PROJECT(rpc)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-g -Wall -Wno-pmf-conversions -std=c++11")
SET(TARGET_NAME test_client)
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/libuv)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/uvpp)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/config)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/transport)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)

ADD_EXECUTABLE(${TARGET_NAME} 
	main/main.cpp
	main/server.cpp
        src/FancyMsgChanLibInterfaceDll.cpp
        src/DynamicModuleLoader.cpp	
	)

TARGET_LINK_LIBRARIES(${TARGET_NAME} 
	libconfig++.a
	libuv_a.a
	#FancyJingMsgChannel
	pthread
	dl)


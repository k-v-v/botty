cmake_minimum_required(VERSION 3.9)
project(BoostHttp)

set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD 17)

SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")



find_package(Boost COMPONENTS system thread log filesystem REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(botty src/main.cpp src/ExchangeConnection.cpp src/ExchangeConnection.hpp src/order.hpp src/JsonParser.cpp src/JsonParser.hpp src/matrix.cpp src/matrix.hpp src/OrderResponse.hpp src/OptimalOrders.cpp src/OptimalOrders.hpp src/balance.hpp)

target_link_libraries(botty
        ${Boost_LIBRARIES}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        -lpthread)
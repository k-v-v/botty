cmake_minimum_required(VERSION 3.9)
project(BoostHttp)
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")


include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(botty main.cpp ExchangeConnection.cpp ExchangeConnection.hpp order.hpp JsonParser.cpp JsonParser.hpp matrix.cpp matrix.hpp OrderResponse.hpp OptimalOrders.cpp OptimalOrders.hpp)


target_link_libraries(botty
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        -lpthread)
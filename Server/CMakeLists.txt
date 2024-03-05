project(server)


set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_DEBUG_LIBS        OFF)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS  ON)


add_executable(server   server.cpp 
                        device.cpp
                        device.h)


target_link_libraries(server avformat avcodec avutil avfilter avdevice swscale Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree)





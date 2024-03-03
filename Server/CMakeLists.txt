project(server)



# Добавьте исполняемый файл и исходные файлы проекта
add_executable(server server.cpp)

# Укажите библиотеки, которые необходимо связать с исполняемым файлом
target_link_libraries(server avformat avcodec avutil avdevice swscale)
project(server)

# Добавьте исполняемый файл и исходные файлы проекта
add_executable(server server.cpp)

# Укажите библиотеки, которые необходимо связать с исполняемым файлом
target_link_libraries(server avformat avcodec avutil avfilter avdevice swscale)

# Укажите путь к файлам заголовков и библиотек SDL
set(SDL2_INCLUDE_DIRS /usr/include/SDL2)
set(SDL2_LIBRARIES /usr/lib/x86_64-linux-gnu/libSDL2.so /usr/lib/x86_64-linux-gnu/libSDL2main.a)

# Подключите файлы заголовков и библиотек SDL к вашему проекту
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(server ${SDL2_LIBRARIES})

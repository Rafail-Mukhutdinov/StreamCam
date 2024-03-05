#include "device.h"


// Объявляем метод main
int main(int argc, char *argv[])
{
  avdevice_register_all();
    // Создаем контекст для входного потока
  AVFormatContext *input_context = NULL;
    // Создаем пакет для хранения данных
  AVPacket *packet = NULL;
  device dev("/dev/video0");
  // Вызываем функцию, которая выводит данные в консоль в виде текста
  dev.output_data_as_text(input_context, packet);
  // Возвращаем 0 в случае успеха
  return 0;
}



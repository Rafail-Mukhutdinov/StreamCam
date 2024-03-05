#include "device.h"



int main(int argc, char *argv[])
{
  const char* name_divice = "/dev/video0";
  device dev(name_divice);

  avdevice_register_all();

  // Создаем контекст для входного потока
  AVFormatContext *input_context = nullptr;

  // Создаем пакет для хранения данных
  AVPacket *packet = nullptr;  

  // Вызываем функцию, которая выводит данные в консоль в виде текста
  dev.output_data_as_text(input_context, packet);


  return 0;
}
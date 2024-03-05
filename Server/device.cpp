#include "device.h"


void device::readAndPrintPacketData(AVFormatContext* context, AVPacket* packet)
{
    // Читаем данные из входного потока и выводим их в консоль
  while (av_read_frame(context, packet) >= 0) {
    // Выводим информацию о пакете в формате JSON
    printf("{\n");
    // Получаем тип медиа из потока
    enum AVMediaType media_type = context->streams[packet->stream_index]->codecpar->codec_type;
    // Передаем его в функцию
    printf("  \"type\": \"%s\",\n", av_get_media_type_string(media_type));
    printf("  \"size\": %d,\n", packet->size);
    // Используем макрос для вывода значения типа int64_t
    printf("  \"time\": %" PRId64 ",\n", packet->pts);
    //printf("  \"data\": \"%s\"\n", packet->data);
    printf("}\n");
    // Освобождаем память от пакета
    av_packet_unref(packet);
  }
}

bool device::openInputDevice(AVFormatContext** context)
{
    // Открываем входное устройство с помощью FFmpeg
  if (avformat_open_input(context, name_devace, NULL, NULL) < 0) {
    // Если не удалось, выводим ошибку и завершаем программу
    fprintf(stderr, "Could not open input device\n");
    return false;
  }
  return true;
}

// Определяем функцию, которая выводит данные в консоль в виде текста
void device::output_data_as_text(AVFormatContext* in_context, AVPacket* in_packet)
{

    this->openInputDevice(&in_context);

  // Выделяем память для пакета
  in_packet = av_packet_alloc();
  if (!in_packet) {
    fprintf(stderr, "Could not allocate packet\n");
    return;
  }

  this->readAndPrintPacketData(in_context, in_packet);

}


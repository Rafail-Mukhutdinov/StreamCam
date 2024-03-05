#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <QTimer>
extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

static int clamp(int x)
{
    return std::min(255, std::max(x, 0));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    QVBoxLayout layout(&window);
    QLabel label;
    QPushButton button1("Увеличить качество");
    QPushButton button2("Уменьшить качество");
    layout.addWidget(&label);
    layout.addWidget(&button1);
    layout.addWidget(&button2);
    window.show();

    const auto Width = 640; // Новое разрешение по ширине
    const auto Height = 480; // Новое разрешение по высоте
    avdevice_register_all();

    AVFormatContext *formatContext = nullptr;
    auto fileName = "/dev/video0";
    auto format = "v4l2";
    auto inputFormat = av_find_input_format(format);
    if (!inputFormat)
    {
        qDebug() << "Unknown input format: " << format;
        return 1;
    }

    AVDictionary *format_opts = nullptr;
    av_dict_set(&format_opts, "framerate", "1000", 0);;
    av_dict_set(&format_opts, "video_size", "640x480", 0); // Установка нового разрешения
    auto err = avformat_open_input(&formatContext, fileName, inputFormat, &format_opts);
    if (err != 0)
    {
        qDebug() << "Could not open input " << fileName;
        return 2;
    }

    av_dump_format(formatContext, 0, fileName, 0);
    av_dict_free(&format_opts);

    int quality = 1; // 1 - high quality, 2 - low quality

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
            AVPacket packet;
            err = av_read_frame(formatContext, &packet);
            if (err)
            {
                timer.stop();
                return;
            }

            QImage image(Width, Height, QImage::Format_RGBA8888);
            uint8_t *pixels = image.bits();
            int pitch = image.bytesPerLine();
            for (int j = 0; j < Height; ++j)
                for (int i = 0; i < Width; ++i)
                {
                    auto y = packet.data[j * Width * 2 + i * 2];
                    auto u = packet.data[j * Width * 2 + (i & 0xfffe) * 2 + 1];
                    auto v = packet.data[j * Width * 2 + (i | 0x1) * 2 + 1];
                    auto c = y - 16;
                    auto d = u - 128;
                    auto e = v - 128;
                    auto r = clamp((298 * c + 409 * e + 128) >> 8);
                    auto g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
                    auto b = clamp((298 * c + 516 * d + 128) >> 8);
                    pixels[i * 4 + j * pitch] = r;
                    pixels[i * 4 + 1 + j * pitch] = g;
                    pixels[i * 4 + 2 + j * pitch] = b;
                    pixels[i * 4 + 3 + j * pitch] = 0xff;
                }
            av_packet_unref(&packet);

            QPixmap pixmap = QPixmap::fromImage(image);
            if (quality == 2) {
                pixmap = pixmap.scaled(Width/2, Height/2, Qt::IgnoreAspectRatio, Qt::FastTransformation);
                pixmap = pixmap.scaled(Width, Height, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            }
            label.setPixmap(pixmap);
        });

    QObject::connect(&button1, &QPushButton::clicked, [&] {
            quality = 1;
        });

    QObject::connect(&button2, &QPushButton::clicked, [&] {
            quality = 2;
        });

    timer.start(1000 / 30); // 30 FPS

    return app.exec();
}

#ifndef BLUR_IMAGE_H
#define BLUR_IMAGE_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QImage>
#include <QDebug>

const static int radius = 25;

class Worker : public QObject
{
    Q_OBJECT

public slots:
    void blur( QImage &origin)
    {
        QImage newImage = origin;

        int kernel [radius][radius];
        for (int a = 0; a < radius; a++) {
            for(int b = 0; b < radius; b++)
                kernel[a][b] = 1;
        }
        int kernelSize = radius;
        int sumKernel = radius * radius;
        int r, g, b;
        QColor color;

        for(int x = kernelSize / 2; x < newImage.width() - (kernelSize / 2); x++){
            for(int y = kernelSize / 2; y < newImage.height() - (kernelSize / 2); y++){

                r = 0;
                g = 0;
                b = 0;


                for(int i = -kernelSize / 2; i <= kernelSize / 2; i++) {
                    for(int j = -kernelSize / 2; j <= kernelSize / 2; j++ ){
                     color = QColor(origin.pixel(x + i, y + j));
                     r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                     g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                     b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    }
                }

                r = qBound(0, r/sumKernel, 255);
                g = qBound(0, g/sumKernel, 255);
                b = qBound(0, b/sumKernel, 255);

                newImage.setPixel(x, y, qRgb(r, g, b));

            }
        }
        qDebug() << "call";

        emit resultReady(newImage);
    }


signals:
    void resultReady(QImage &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::blur);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }

signals:
    void operate(QImage &result);
    void handleResults(QImage &result);
};

#endif // BLUR_IMAGE_H

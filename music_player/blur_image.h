#ifndef BLUR_IMAGE_H
#define BLUR_IMAGE_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QImage>
#include <QDebug>

const static int radius = 9; //半径
const static int count = 9; //模糊次数

class Worker : public QObject
{
    Q_OBJECT

public slots:
    void blur( QImage &origin)
    {
        QImage newImage = origin;
        qint64 kernel [radius][radius] = {
            {1,    6,      720,     3200,    5000,     3200,    720,     6,      1},
            {6,    1900,   23900,   107000,  176000,   107000,  23900,   1900,   6},
            {720,  23900,  291000,  3551000, 2153000,  3551000, 291000,  23900,  720},
            {3200, 107000, 1306000, 5854000, 9653000,  5854000, 1306000, 107000, 700},
            {5000, 176000, 2153000, 9653000, 15915000, 9653000, 2153000, 176000, 5000},
            {3200, 107000, 1306000, 5854000, 9653000,  5854000, 1306000, 107000, 700},
            {720,  23900,  291000,  3551000, 2153000,  3551000, 291000,  23900,  720},
            {6,    1900,   23900,   107000,  176000,   107000,  23900,   1900,   6},
            {1,    6,      720,     3200,    5000,     3200,    720,     6,      1}
        };
        qint64 sumKernel = 0;
        for(int a = 0; a < radius; a++) {
            for(int b = 0; b < radius; b++)
                sumKernel = sumKernel + kernel[a][b];
        }
        int kernelSize = radius;
        qint64 r, g, b;
        QColor color;

        for(int z = 0; z < count; z++) {
            for(int x = kernelSize / 2; x < newImage.width() - (kernelSize / 2); x++){
                for(int y = kernelSize / 2; y < newImage.height() - (kernelSize / 2); y++){

                    r = 0;
                    g = 0;
                    b = 0;


                    for(int i = -kernelSize / 2; i <= kernelSize / 2; i++) {
                        for(int j = -kernelSize / 2; j <= kernelSize / 2; j++ ){
                         color = QColor(newImage.pixel(x + i, y + j));
                         r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                         g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                         b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                        }
                    }
                    r = r/sumKernel;
                    g = g/sumKernel;
                    b = b/sumKernel;

                    newImage.setPixel(x, y, qRgb(r, g, b));
                }
            }
        }
        qDebug() << "blur";
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

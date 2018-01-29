#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QSize>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget* parent);
    ~TitleBar();

protected:
    void mousePressEvent(QMouseEvent* event);

public slots:
    void ButtonMinClicked();
    void ButtonCloseClicked();

private:
    Ui::TitleBar* ui;
};

#endif // TITLEBAR_H

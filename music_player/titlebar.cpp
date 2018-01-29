#include "titlebar.h"
#include "ui_titlebar.h"

#include <QMouseEvent>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleBar::TitleBar(QWidget *parent)
    :ui(new Ui::TitleBar)
    ,QWidget(parent)
{
    ui->setupUi(this);

    connect(ui->minButton, SIGNAL(clicked()), this, SLOT(ButtonMinClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(ButtonCloseClicked()));
}

TitleBar::~TitleBar()
{
    delete ui;
}

void TitleBar::ButtonMinClicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void TitleBar::ButtonCloseClicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->close();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if(ReleaseCapture()) {
        QWidget* pWindow = this->window();
        if(pWindow->isTopLevel())
            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    }
    event->ignore();
#else
#endif
}






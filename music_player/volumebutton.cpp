#include "volumebutton.h"

#include <QtWidgets>
#include <QtWinExtras>

VolumeButton::VolumeButton(QWidget *parent) :
    QToolButton(parent)
{
    setIcon(QIcon(":/icon/volume.png"));
    setStyleSheet("VolumeButton{border-style: outset;}");
    setPopupMode(QToolButton::InstantPopup);

    QWidget *popup = new QWidget(this);

    slider = new QSlider(Qt::Horizontal, popup);
    slider->setRange(0, 100);
    slider->setValue(50);
    slider->setStyleSheet(tr("QSlider::groove:horizontal { background: rgb(230, 230, 230);height: 3px;padding-left:-1px;padding-right:-1px;}"
                             "QSlider::sub-page:horizontal {background: rgb(255, 0, 0);border-radius: 2px;}"
                             "QSliderr::add-page:horizontal {background: rgbrgb(230, 230, 230);border: 0px solid #777;}"
                             "QSlider::handle:horizontal { width: 18px;margin-top: -8px; margin-bottom: -7px;border-radius: 9px;"
                             "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
                             "stop:0.25 #ff0000, stop:0.3 rgba(255, 255, 255), stop:0.87 rgba(250, 250, 250),stop:0.9 rgba(220, 220, 220));}"));
    connect(slider, &QAbstractSlider::valueChanged, this, &VolumeButton::volumeChanged);

    label = new QLabel(popup);
    label->setAlignment(Qt::AlignCenter);
    label->setNum(50);
    label->setMinimumWidth(label->sizeHint().width());

    typedef void(QLabel::*IntSlot)(int);
    connect(slider, &QAbstractSlider::valueChanged, label, static_cast<IntSlot>(&QLabel::setNum));

    QBoxLayout *popupLayout = new QHBoxLayout(popup);
    popupLayout->setMargin(2);
    popupLayout->addWidget(slider);
    popupLayout->addWidget(label);

    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popup);

    menu = new QMenu(this);
    menu->addAction(action);
    setMenu(menu);
}

void VolumeButton::increaseVolume()
{
    slider->triggerAction(QSlider::SliderPageStepAdd);
}

void VolumeButton::descreaseVolume()
{
    slider->triggerAction(QSlider::SliderPageStepSub);
}

int VolumeButton::volume() const
{
    return slider->value();
}

void VolumeButton::setVolume(int volume)
{
    slider->setValue(volume);
}

void VolumeButton::stylize()
{
    if (QtWin::isCompositionEnabled()) {
        QtWin::enableBlurBehindWindow(menu);
        QString css("QMenu { border: 1px solid %1; border-radius: 2px; background: transparent; }");
        menu->setStyleSheet(css.arg(QtWin::realColorizationColor().name()));
    } else {
        QtWin::disableBlurBehindWindow(menu);
        QString css("QMenu { border: 1px solid black; background: %1; }");
        menu->setStyleSheet(css.arg(QtWin::realColorizationColor().name()));
    }
}

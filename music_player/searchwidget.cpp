#include "searchwidget.h"
#include "qqmusic_api.h"
#include "ui_search.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QWidget>
#include <QColor>
#include <QBrush>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QStringList>
#include <QGraphicsProxyWidget>

int widgets = 0; //计widget的个数，

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    historyKey = QStringList();

    api = new QQMusicAPI(this);
    api->getHotWord();
    connect(api, &QQMusicAPI::hotWordList, this, &SearchWidget::setHotLabel);
    connect(api, &QQMusicAPI::searchList, this, &SearchWidget::slotData);

    ui->historyWidget->resize(320, 330);
    //这里存在一个问题，无法正确返回ui文件里部件的宽和高
    ui->historyWidget->setColumnCount(2);
    ui->historyWidget->setColumnWidth(0, ui->historyWidget->width() * 0.9);
    ui->historyWidget->setColumnWidth(1, ui->historyWidget->width() * 0.1);
    ui->historyWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->historyWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->historyWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->historyWidget->setShowGrid(false);
    ui->historyWidget->setCornerButtonEnabled(false);
    ui->historyWidget->horizontalHeader()->hide();
    ui->historyWidget->verticalHeader()->hide();

    connect(ui->searchButton, &QPushButton::clicked, this, &SearchWidget::search);
    connect(ui->historyWidget, &QTableWidget::cellClicked, this, &SearchWidget::historySearch);
    connect(ui->clearButton, &QPushButton::clicked, this, &SearchWidget::clearHistory);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &SearchWidget::deleteWidget);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::signalItme(int row, int column)
{
    Q_UNUSED(column);

    emit currentItem(names.at(row),
                     singer.at(row),
                     album.at(row),
                     urls.at(row),
                     imageUrls.at(row),
                     lrcUrls.at(row),
                     timeList.at(row));
}

void SearchWidget::slotData(QStringList song,
                            QStringList sing,
                            QStringList ablum,
                            QStringList song_url,
                            QStringList cover_url,
                            QStringList lrc_url,
                            QList <int> time)
{
    names = song;
    singer = sing;
    album = ablum;
    urls = song_url;
    imageUrls = cover_url;
    lrcUrls = lrc_url;
    timeList = time;

    QFont bigfont("Microsoft YaHei UI", 10);
    QFont smallfont("Microsoft YaHei UI", 8);

    for(int row = 0; row < names.size(); row++) {
        widget->setRowHeight(row, 50);

        QTableWidgetItem* item_0 = new QTableWidgetItem(names.at(row));
        item_0->setFont(bigfont);
        widget->setItem(row, 1, item_0);

        QTableWidgetItem* item_1 = new QTableWidgetItem(singer.at(row));
        item_1->setFont(smallfont);
        widget->setItem(row, 2, item_1);

        QTableWidgetItem* item_2 = new QTableWidgetItem(ablum.at(row));
        item_2->setFont(smallfont);
        widget->setItem(row, 3, item_2);

        QTableWidgetItem* item_3 = new QTableWidgetItem(QString::number(timeList.at(row)));
        item_3->setFont(smallfont);
        widget->setItem(row, 4, item_3);

        QTableWidgetItem* item_4 = new QTableWidgetItem();
        item_4->setIcon(QIcon(":/icon/collect"));
        widget->setItem(row, 0, item_4);
    }
}

void SearchWidget::search()
{
    if(!ui->searchEdit->text().isEmpty()) {
        names.clear();
        singer.clear();
        album.clear();
        urls.clear();
        imageUrls.clear();
        lrcUrls.clear();
        timeList.clear();
        if(widgets > 0) {
            widget->deleteLater();
            widgets--;
        }

        widget = new QTableWidget(20, 5, this);
        widgets ++;
        widget->resize(this->width() - 80, this->height() - 100);
        widget->move(40, 90);
        widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        widget->setSelectionBehavior(QAbstractItemView::SelectRows);
        widget->setAlternatingRowColors(true);
        widget->setShowGrid(false);
        widget->setCornerButtonEnabled(false);
        widget->horizontalHeader()->hide();
        widget->verticalHeader()->hide();
        widget->setColumnWidth(0, widget->width() * 0.05);
        widget->setColumnWidth(1, widget->width() * 0.4);
        widget->setColumnWidth(2, widget->width() * 0.2);
        widget->setColumnWidth(3, widget->width() * 0.3);
        widget->setColumnWidth(4, widget->width() * 0.05);
        widget->setStyleSheet(tr("selection-background-color: rgb(200, 200, 200);"
                                 "border-width: 0px;"
                                 "border-style: solid;"
                                 "background-color: rgb(240, 240, 240);"));

        widget->show();

        disconnect(widget, &QTableWidget::cellDoubleClicked, this, &SearchWidget::signalItme);
        widget->clear();
        connect(widget, &QTableWidget::cellDoubleClicked, this, &SearchWidget::signalItme);

        api->search(ui->searchEdit->text(), 1);

        historyKey.insert(0, ui->searchEdit->text());
        ui->historyWidget->insertRow(0);
        ui->historyWidget->setItem(0, 0, new QTableWidgetItem(ui->searchEdit->text()));
        ui->historyWidget->setItem(0, 1, new QTableWidgetItem(QIcon(tr(":/icon/delete_3.png")), QString()));

        if(historyKey.size() > 10) {
            historyKey.removeAt(historyKey.size() - 1);
            ui->historyWidget->removeRow(historyKey.size() - 1);
        }
    }
}

void SearchWidget::setHotLabel(QStringList wordList)
{
    int w = 20;
    int h = 10;

    foreach(QString word, wordList) {
        QPushButton* button = new QPushButton(word, ui->hotWidget);
        button->setMinimumHeight(30);
        button->setMaximumHeight(30);

        button->setStyleSheet(tr("border: 1px solid rgb(200, 200, 200);"
                                 "border-radius: 15px;"
                                 "color: rgb(70, 70, 70);"
                                 "font: 10pt Microsoft YaHei UI;"));
        button->adjustSize();
        if(w + button->width() < ui->hotWidget->width()) {
            button->move(w, h);
            w = w + button->width() + 20;
        } else {
            w = 20;
            h = h + 45;
            button->move(w, h);
            w = w + button->width() + 20;
        }
        button->show();
        connect(button, &QPushButton::clicked, this, &SearchWidget::setLineEdit);
    }
}

void SearchWidget::setLineEdit()
{
    QPushButton* button = qobject_cast <QPushButton*> (sender());
    QString searchKey = button->text();
    searchKey.remove(0, 1);
    searchKey.remove(searchKey.size() - 1, 1);
    ui->searchEdit->setText(searchKey);

    search();
}

void SearchWidget::historySearch(int row, int column)
{
    if(column == 1) {
        ui->historyWidget->removeRow(row);
        historyKey.removeAt(row);
        qDebug() << 1;
    } else {

        ui->searchEdit->setText(ui->historyWidget->item(row, column)->text());
        ui->historyWidget->removeRow(row);
        qDebug() << 1;
    }
    search();
}

void SearchWidget::clearHistory()
{
    historyKey.clear();
    disconnect(ui->historyWidget, &QTableWidget::cellClicked, this, &SearchWidget::historySearch);
    ui->historyWidget->clear();
    connect(ui->historyWidget, &QTableWidget::cellClicked, this, &SearchWidget::historySearch);
}

void SearchWidget::deleteWidget(QString text)
{
    if(text.isEmpty()) {
        if(widgets > 0) {
            disconnect(widget, &QTableWidget::cellDoubleClicked, this, &SearchWidget::signalItme);
            widget->clear();
            widget->deleteLater();
            widgets--;
        }
    }
}

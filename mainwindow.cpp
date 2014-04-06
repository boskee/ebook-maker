#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QImage>
#include <QListWidgetItem>
#include <QGraphicsPixmapItem>
#include <poppler-qt5.h>

#include <RectItem.h>
#include <PageItem.h>

#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scenes(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImageFile();
}

void MainWindow::loadImageFile()
{
    QString path;
    path = "/home/voitek/Projects/ebook-maker/WireframeOverview.pdf";
    Poppler::Document *document = Poppler::Document::load(path);

    if (!document || document->isLocked()) {
      // ... error message ....
      delete document;
      //qDebug() << " ZLY PDF";
    }

    // Access page of the PDF file
    Poppler::Page* pdfPage = document->page(1);  // Document starts at page 0
    if (pdfPage == 0) {
      // ... error message ...
        //qDebug() << " I CHUJ";
    }

    qDebug() << pdfPage->pageSizeF();

    // Generate a QImage of the rendered page
    QImage image = pdfPage->renderToImage(physicalDpiX(), physicalDpiY()); // QImage image = pdfPage->renderToImage(xres, yres, x, y, width, height);
    if (image.isNull()) {
      // ... error message ...
        //qDebug() << " I DRUGI CHUJ";
    }

    for(int i = 0; i < document->numPages(); i++)
    {
        Poppler::Page *page = document->page(i);
        QImage image = page->renderToImage(physicalDpiX(), physicalDpiY());
        QListWidgetItem *item;
        QPixmap thumbnailPixmap = QPixmap::fromImage(image);
        item = new QListWidgetItem(QIcon(thumbnailPixmap), "Page " + i);
        item->setData(0, i);
        ui->listWidget->addItem(item);

        PageItem *pPixMapItem2 = new PageItem(thumbnailPixmap);
        pPixMapItem2->setPos(0, 0);
        pPixMapItem2->setAcceptDrops(true);

        this->scenes[i] = new QGraphicsScene(0, 0, pPixMapItem2->pixmap().width(), pPixMapItem2->pixmap().height());
        this->scenes[i]->addItem(pPixMapItem2);
        delete page;
    }

    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setScene(this->scenes[0]);
    delete pdfPage;
    delete document;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_testButton_clicked()
{
    int i = 0;
    RectItem *item = new RectItem();
    item->setPos(::sin((i * 6.28) / 10.0) * 150,
                 ::cos((i * 6.28) / 10.0) * 150);

    ui->graphicsView->scene()->addItem(item);
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    int sceneNo = current->data(0).toInt();
    ui->graphicsView->setScene(this->scenes[sceneNo]);
}

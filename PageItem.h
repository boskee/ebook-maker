#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>

class PageItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    PageItem(const QPixmap &pixmap);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    QPixmap mainImage;
    QPoint mousePos;

signals:

public slots:

};

#endif // PAGEITEM_H

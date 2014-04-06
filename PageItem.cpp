
#include <QtGui>

#include "PageItem.h"
#include <QGraphicsPixmapItem>

/*PageItem::PageItem(QObject *parent) :
    QGraphicsPixmapItem(parent)
{
}*/

PageItem::PageItem(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap){
    setAcceptDrops(true);
}

void PageItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    update();
}

void PageItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->proposedAction() == Qt::MoveAction) {
        qDebug() << "KURWA MAC!!";
        event->acceptProposedAction();
    }
}

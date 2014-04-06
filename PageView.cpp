#include "PageView.h"

//Qt includes
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

/**
* Sets up the subclassed QGraphicsView
*/
PageView::PageView(QWidget* parent) : QGraphicsView(parent) {

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Use ScrollHand Drag Mode to enable Panning
    setDragMode(ScrollHandDrag);
}

/**
  * Zoom the view in and out.
  */
void PageView::wheelEvent(QWheelEvent* event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

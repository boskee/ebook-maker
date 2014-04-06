#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>

class PageView : public QGraphicsView
{
public:
    PageView(QWidget* parent = NULL);

protected:

    //Take over the interaction
    virtual void wheelEvent(QWheelEvent* event);
};

#endif // PAGEVIEW_H

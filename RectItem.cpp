/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QDrag>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

#include "RectItem.h"

RectItem::RectItem(QGraphicsItem * parent):
    QGraphicsItem(parent),
    color(qrand() % 256, qrand() % 256, qrand() % 256, 200),
    _pen(),
    _dragStart(0,0),
    _gridSpace(10),
    _width(250),
    _height(100),
    _cornerDragStart(0,0),
    _XcornerGrabBuffer(7),
    _YcornerGrabBuffer(7),
    _drawingWidth(  _width -   _XcornerGrabBuffer),
    _drawingHeight( _height -  _YcornerGrabBuffer),
    _drawingOrigenX( _XcornerGrabBuffer),
    _drawingOrigenY( _YcornerGrabBuffer),
    _isDragged(false),
    _isHighlighted(false),
    _isHovered(false),
    _isSelected(false),
    _showBoxStyle(kWhenSelected),
    _drawBoxLineStyle(kDrawDotted),
    _boxStyle(kTransparent),
    _hoverLineThickness(3)
{
    setParentItem(parent);
    _corners[0] = NULL;
    _corners[1] = NULL;
    _corners[2] = NULL;
    _corners[3] = NULL;

    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);
}

RectItem::~RectItem()
{
    delete _corners[0];
    delete _corners[1];
    delete _corners[2];
    delete _corners[3];
}


void RectItem::setHoverLineThickness(int t )
{
    _hoverLineThickness = t;
}


void RectItem::setShowBoxLineStyle(ShowBoxStyle s )
{
    _showBoxStyle = s;
}

void RectItem::setDrawBoxLineStyle( DrawBoxLineStyle s)
{
    _drawBoxLineStyle = s;
}

void RectItem::setBoxStyle( BoxStyle s)
{
    _boxStyle = s;
}

void RectItem::setHighlighted(bool on)
{
    _isHighlighted = on;
}

void RectItem::increaseWidth(int inc)
{
    adjustDrawingSize(inc, 0);
}

QString RectItem::getPositionAsString()
{
    QString v= QString::number( this->pos().x() ) + "," + QString::number(this->pos().y() );
    return v;
}

QPointF RectItem::getSize()
{
    QPointF size;
    size.setX(_width);
    size.setY(_height);

    return size;
}

QString RectItem::getSizeAsString()
{
    return ( QString::number( _width) + "," + QString::number(_height));
}


void RectItem::setSize(QPoint size)
{
    _width = size.x();
    _height = size.y();

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;
}

/**
 *  To allow the user to grab the corners to re-size, we need to get a hover
 *  indication. But if the mouse pointer points to the left, then when the mouse
 *  tip is to the left but just outsize the box, we will not get the hover.
 *  So the solution is to tell the graphics scene the box is larger than
 *  what the painter actually paints in. This way when the user gets the mouse
 *  within a few pixels of what appears to be the edge of the box, we get
 *  the hover indication.

 *  So the cornerGrabBuffer is a few pixel wide buffer zone around the outside
 *  edge of the box.
 *
 */
void RectItem::adjustDrawingSize(int x, int y)
{

    QPoint newSize  (_width + x, _height+y);

    // call the virtual setSize() so that children can capture this and edit size before applying
    setSize (newSize);

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;

}

/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending item is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool RectItem::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    //    qDebug() << " QEvent == " + QString::number(event->type());

    CornerGrabber * corner = dynamic_cast<CornerGrabber *>(watched);
    if ( corner == NULL) return false; // not expected to get here

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        // this is not one of the mouse events we are interrested in
        return false;
    }


    switch (event->type() )
    {
        // if the mouse went down, record the x,y coords of the press, record it inside the corner object
    case QEvent::GraphicsSceneMousePress:
        {
            corner->setMouseState(CornerGrabber::kMouseDown);
            corner->mouseDownX = mevent->pos().x();
            corner->mouseDownY = mevent->pos().y();
        }
        break;

    case QEvent::GraphicsSceneMouseRelease:
        {
            corner->setMouseState(CornerGrabber::kMouseReleased);
            //graphicHasChanged();
        }
        break;

    case QEvent::GraphicsSceneMouseMove:
        {
            corner->setMouseState(CornerGrabber::kMouseMoving );
        }
        break;

    default:
        // we dont care about the rest of the events
        return false;
        break;
    }


    if ( corner->getMouseState() == CornerGrabber::kMouseMoving )
    {

        qreal x = mevent->pos().x(), y = mevent->pos().y();

        // depending on which corner has been grabbed, we want to move the position
        // of the item as it grows/shrinks accordingly. so we need to eitehr add
        // or subtract the offsets based on which corner this is.

        int XaxisSign = 0;
        int YaxisSign = 0;
        switch( corner->getCorner() )
        {
        case 0:
            {
                XaxisSign = +1;
                YaxisSign = +1;
            }
            break;

        case 1:
            {
                XaxisSign = -1;
                YaxisSign = +1;
            }
            break;

        case 2:
            {
                XaxisSign = -1;
                YaxisSign = -1;
            }
            break;

        case 3:
            {
                XaxisSign = +1;
                YaxisSign = -1;
            }
            break;

        }

        // if the mouse is being dragged, calculate a new size and also re-position
        // the box to give the appearance of dragging the corner out/in to resize the box

        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int newWidth = _width + ( XaxisSign * xMoved);
        if ( newWidth < 40 ) newWidth  = 40;

        int newHeight = _height + (YaxisSign * yMoved) ;
        if ( newHeight < 40 ) newHeight = 40;

        int deltaWidth  =   newWidth - _width ;
        int deltaHeight =   newHeight - _height ;

        adjustDrawingSize(  deltaWidth ,   deltaHeight);

        deltaWidth *= (-1);
        deltaHeight *= (-1);

        if ( corner->getCorner() == 0 )
        {
            int newXpos = this->pos().x() + deltaWidth;
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(newXpos, newYpos);
        }
        else   if ( corner->getCorner() == 1 )
        {
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(this->pos().x(), newYpos);
        }
        else   if ( corner->getCorner() == 3 )
        {
            int newXpos = this->pos().x() + deltaWidth;
            this->setPos(newXpos,this->pos().y());
        }

        setCornerPositions();

        this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}

// for supporting keys
void RectItem::keyPressEvent ( QKeyEvent * event )
{
    event->setAccepted(true);

    if (this->isSelected())
    {
        if (event->key () == Qt::Key_Delete)
        {
            qDebug() << "REMOVE!!";
        }
        else if (event->key() == Qt::Key_Left)
        {
            QPointF location = this->pos();
            if (event->modifiers() & Qt::ShiftModifier)
            {
                location.setX(static_cast<int>(location.x()) - 10);
            }
            else
            {
                location.setX(static_cast<int>(location.x()) - 1);
            }
            this->setPos(location);
            return;
        }
        else if (event->key() == Qt::Key_Right)
        {
            QPointF location = this->pos();
            if (event->modifiers() & Qt::ShiftModifier)
            {
                location.setX(static_cast<int>(location.x()) + 10);
            }
            else
            {
                location.setX(static_cast<int>(location.x()) + 1);
            }
            this->setPos(location);
            return;
        }
        else if (event->key() == Qt::Key_Up)
        {
            QPointF location = this->pos();
            if (event->modifiers() & Qt::ShiftModifier)
            {
                location.setY(static_cast<int>(location.y()) - 10);
            }
            else
            {
                location.setY(static_cast<int>(location.y()) - 1);
            }
            this->setPos(location);
            return;
        }
        else if (event->key() == Qt::Key_Down)
        {
            QPointF location = this->pos();
            if (event->modifiers() & Qt::ShiftModifier)
            {
                location.setY(static_cast<int>(location.y()) + 10);
            }
            else
            {
                location.setY(static_cast<int>(location.y()) + 1);
            }
            this->setPos(location);
            return;
        }
    }

    QGraphicsItem::keyPressEvent(event);
}

// for supporting moving the box across the scene
void RectItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    setCursor(Qt::ArrowCursor);
    _isDragged = false;

    //graphicHasChanged();

    QGraphicsItem::mouseReleaseEvent(event);
}


// for supporting moving the box across the scene
void RectItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    setCursor(Qt::ClosedHandCursor);
    _dragStart = event->pos();
    _isDragged = true;

    QGraphicsItem::mousePressEvent(event);
}


// for supporting moving the box across the scene
void RectItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    if (_isDragged)
    {
        QPointF newPos = event->pos() ;
        QPointF location = this->pos();
        location += (newPos - _dragStart);

        setCursor(Qt::ClosedHandCursor);
        this->setPos(location);
    }

    QGraphicsItem::mouseMoveEvent(event);
}

// remove the corner grabbers

void RectItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _isHovered = false;

    _corners[0]->setParentItem(NULL);
    _corners[1]->setParentItem(NULL);
    _corners[2]->setParentItem(NULL);
    _corners[3]->setParentItem(NULL);

    delete _corners[0];
    _corners[0] = NULL;

    delete _corners[1];
    _corners[1] = NULL;

    delete _corners[2];
    _corners[2] = NULL;

    delete _corners[3];
    _corners[3] = NULL;
    setCursor(Qt::ArrowCursor);

    update();
}


// create the corner grabbers

void RectItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _isHovered = true;

    _corners[0] = new CornerGrabber(this, 0, true);
    _corners[1] = new CornerGrabber(this, 1, true);
    _corners[2] = new CornerGrabber(this, 2, true);
    _corners[3] = new CornerGrabber(this, 3, true);

    _corners[0]->setPaintStyle(CornerGrabber::kBox);
    _corners[1]->setPaintStyle(CornerGrabber::kBox);
    _corners[2]->setPaintStyle(CornerGrabber::kBox);
    _corners[3]->setPaintStyle(CornerGrabber::kBox);

    _corners[0]->installSceneEventFilter(this);
    _corners[1]->installSceneEventFilter(this);
    _corners[2]->installSceneEventFilter(this);
    _corners[3]->installSceneEventFilter(this);

    setCornerPositions();

    update();
}

void RectItem::setCornerPositions()
{
    _corners[0]->setPos(_drawingOrigenX, _drawingOrigenY);
    _corners[1]->setPos(_drawingWidth,  _drawingOrigenY);
    _corners[2]->setPos(_drawingWidth , _drawingHeight);
    _corners[3]->setPos(_drawingOrigenX, _drawingHeight);

}

QRectF RectItem::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}


QPointF RectItem::getVisibleCenter()
{
    QRectF area = getUsableArea();

    double centerX = (area.x() + area.width())/2;
    double centerY = (area.y() + area.height())/2;

    return QPointF(centerX, centerY);
}

QRectF RectItem::getUsableArea()
{
    int x0 = _drawingOrigenX + 2;
    int y0 = _drawingOrigenY + 2;
    int width = _drawingWidth -2 ;
    int height = _drawingHeight -2;

    return QRectF(x0,y0, width, height);
}


QPointF RectItem::getSideCenterPointInSceneCoord(int side)
{
    switch (side)
    {
    case 0:
        return mapToScene( QPointF ( (_drawingOrigenX +  _drawingWidth)/2, _drawingOrigenY ));
        break;

    case 1:
        return  mapToScene( QPointF ( _drawingWidth, (_drawingOrigenY + _drawingHeight)/2));
        break;

    case 2:
        return  mapToScene( QPointF ( (_drawingOrigenX +  _drawingWidth)/2,  _drawingHeight));
        break;

    case 3:
        return  mapToScene( QPointF ( _drawingOrigenX, (_drawingOrigenY + _drawingHeight)/2));
        break;
    }

    return QPointF();
}

void RectItem::paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if ( _isHighlighted )
        _pen.setColor(Qt::red);
    else
        _pen.setColor(Qt::black);

    _pen.setWidth(1);

    if ( _drawBoxLineStyle == kDrawSolid )
        _pen.setStyle( Qt::SolidLine );
    else
        _pen.setStyle( Qt::DotLine );

    painter->setPen(_pen);

    painter->setBrush(_isHovered ? color.light(110) : color);


    // draw outter layer box
    // if shadowed box, draw inside, else draw on the outter edge
    QRectF rect2;

    QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
    QPointF bottomRight2 ( _drawingWidth, _drawingHeight);

    rect2 = QRectF (topLeft2, bottomRight2);

    painter->drawRect(rect2);
}

void RectItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    _pen.setColor(Qt::black);
    _pen.setWidth(1);

    if (this->isSelected()) {
        _pen.setStyle( Qt::DotLine );
    } else {
        _pen.setStyle( Qt::SolidLine );
    }

    painter->setPen(_pen);

    painter->setBrush(_isHovered ? color.light(110) : color);

    // draw outter layer box
    // if shadowed box, draw inside, else draw on the outter edge
    QRectF rect2;

    QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
    QPointF bottomRight2 ( _drawingWidth, _drawingHeight);

    rect2 = QRectF (topLeft2, bottomRight2);

    painter->drawRect(rect2);

    /*if  ( (_showBoxStyle == kAlways) || (( _showBoxStyle == kWhenSelected) && ( _isHovered == true)))
    {
        paintWithVisibleBox(painter,0,0);
    }*/
}


void RectItem::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void RectItem::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

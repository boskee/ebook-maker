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

#ifndef RECTITEM_H
#define RECTITEM_H

#include <QGraphicsItem>
#include "cornergrabber.h"

class CornerGrabber;
class IAttribute;

 class RectItem : public QGraphicsItem
 {
 public:
     RectItem(QGraphicsItem *parent = 0);
     ~RectItem();

     virtual void setSize(QPoint size); ///< set the size of this object
     void    increaseWidth(int inc);
     QString getPositionAsString();
     QString getSizeAsString();
     QPointF getSize();

     void    setHighlighted(bool);

     enum BoxStyle {kSolidWithShadow, kSolidNoShadow, kTransparent};
     enum ShowBoxStyle { kAlways, kWhenSelected};
     enum DrawBoxLineStyle { kDrawSolid, kDrawDotted};

     void setShowBoxLineStyle(ShowBoxStyle s );///< show box allways, or only when selected
     void setDrawBoxLineStyle( DrawBoxLineStyle s);///< if drawing box, draw solid or dotted line
     void setBoxStyle (BoxStyle s); ///< set box style
     void setHoverLineThickness(int t);///< set line thickness when hovered
     QRectF getUsableArea();///< returns a rect relative to the SelecableBoxGraphic that represents the inside margin, or usable area of the box.
     QPointF getVisibleCenter();///< returns a point which is appears to be the center of the box (i.e. does not include drop shadow), may not be the center of boundingRect()
     QPointF getSideCenterPointInSceneCoord(int side); ///< returns the center point on a given side, for anchoring a transition line in the middle of a side

     /*QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
*/

 protected:
     virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event

     /*
     virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
     virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
     void mousePressEvent(QGraphicsSceneMouseEvent *event);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
     void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
     void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

     QColor  _outterborderColor; ///< the hover event handlers will toggle this between red and black
     QPen    _outterborderPen; ///< the pen is used to paint the red/black border
     QPointF _location;

     bool mouseOver;*/

 private:
     //virtual void graphicHasChanged() = 0; ///< subclass must implement this, used to record user changes back to the data model

     virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView

     virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
     virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

     virtual void keyPressEvent ( QKeyEvent * event );

     virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
     virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
     virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

     virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
     virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
     virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;

     void setCornerPositions();
     void adjustDrawingSize(int x, int y);


     void paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);


     QColor color;

     QPen     _pen ;

     QPointF _dragStart;
     int     _gridSpace;
     qreal   _width;
     qreal   _height;

     QPointF _cornerDragStart;

     int _XcornerGrabBuffer;
     int _YcornerGrabBuffer;
     qreal   _drawingWidth;
     qreal   _drawingHeight;
     qreal   _drawingOrigenX;
     qreal   _drawingOrigenY;

     CornerGrabber*  _corners[4];// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box

     bool    _isHighlighted; ///< highlighting is used to indicate visually a group of objects that are related when one of the group is selected
     bool    _isHovered;
     bool    _isSelected;
     bool    _isDragged;

     ShowBoxStyle     _showBoxStyle;
     DrawBoxLineStyle _drawBoxLineStyle;
     BoxStyle         _boxStyle;
     int              _hoverLineThickness;
 };

 #endif // RECTITEM_H

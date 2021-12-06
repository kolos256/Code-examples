#include "myimglabel.h"

MyImgLabel::MyImgLabel(QWidget *parent) :
    MainCursor1(10,10),CursorMove(false),
    QLabel(parent)
{
    emit SignalCursorChanged(MainCursor1);
    update();
}

void MyImgLabel::paintEvent ( QPaintEvent * event )
{

    //painter.setBrush(Qt::red);
#if defined (DBG)
    qDebug()<<"width(): "<<width();
#endif
    //painter.setBrush(QBrush(Qt::red));

#if 0
    if((MainCursor1.x()>0)&&(MainCursor1.x()<width())&&((MainCursor1.y()>0))&&(MainCursor1.y()<height()))
    {
        //qDebug()<<"drawEllipse";
        painter.drawEllipse(MainCursor1,5,5);
        //painter.drawEllipse(10,10,20,20);
    }
#endif
    //painter.drawEllipse(MainCursor1,5,5);

#if 0
    painter.setFont(QFont("times",12));
    painter.setPen( QPen( cursor2olor, CursorPosAccuracy-1 ) );

    if(HLine2Visiable)
    {
        if(HLine2YPos>height()){HLine2YPos=height();}
        Cursor|=0x02;
        painter.drawLine(0,HLine2YPos,width(),HLine2YPos);
    }
    painter.setPen( QPen( Qt::red, CursorPosAccuracy-1 ) );
    if(HLine1Visiable)
    {
        if(HLine1YPos>height()){HLine1YPos=height();}
        Cursor|=0x01;
        painter.drawLine(0,HLine1YPos,width(),HLine1YPos);
    }
    painter.setPen( QPen( Qt::red, CursorPosAccuracy-1 ) );
    if(VLine2Visiable)
    {
        if(VLine2XPos>width()){VLine2XPos=width();}
        Cursor|=0x08;
        painter.drawLine(VLine2XPos,0,VLine2XPos,height());
    }
    painter.setPen( QPen( cursor2olor, CursorPosAccuracy-1 ) );
    if(VLine1Visiable)
    {
        if(VLine1XPos>width()){VLine1XPos=width();}
        Cursor|=0x04;
        painter.drawLine(VLine1XPos,0,VLine1XPos,height());
    }


    //painter.setPen( QPen( Qt::black, 3 ) );
    //painter.drawPoint( QPoint( 11,11 ) );
    //painter.drawLine(10,10,500,500);
    //painter.drawLine(HLine1);
#endif
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::red));
    //if((MainCursor1.x()>0)&&(MainCursor1.x()<width())&&((MainCursor1.y()>0))&&(MainCursor1.y()<height()))
    {
        //qDebug()<<"drawEllipse";
        painter.drawEllipse(MainCursor1,5,5);
        //painter.drawEllipse(10,10,20,20);
    }


}

void MyImgLabel::mouseReleaseEvent ( QMouseEvent * event )
{
    #if defined (DBG)
    int x=event->pos().x();
    int y=event->pos().y();
    qDebug()<<"MyImgLabel::mouseReleaseEvent: "<<x<<","<<y;
    #endif

    emit SignalCursorChanged(MainCursor1);
    CursorMove=false;
    update();
}
void MyImgLabel::mousePressEvent ( QMouseEvent * event )
{
    #if defined (DBG)
    int x=event->pos().x();
    int y=event->pos().y();
    qDebug()<<"MyImgLabel::mousePressEvent: "<<x<<","<<y;
    #endif
    MainCursor1=event->pos();
    emit SignalCursorChanged(MainCursor1);
    CursorMove=true;
    update();
}


void MyImgLabel::mouseMoveEvent ( QMouseEvent * event )
{
   #if defined (DBG)
   int x=event->pos().x();
   int y=event->pos().y();
   qDebug()<<"MyImgLabel::mouseMoveEvent: "<<x<<","<<y;
   qDebug()<<"MainCursor1: "<<MainCursor1.x()<<","<<MainCursor1.y();
    #endif
   //update(QRegion(MainCursor1.x()-20,MainCursor1.y()-20,40,40));
   if(CursorMove)
   {
       //MainCursor1=(event->pos());
       MainCursor1.setX(qMax(qMin(event->pos().x(),width()),0));
       MainCursor1.setY(qMax(qMin(event->pos().y(),height()),0));
       emit SignalCursorChanged(MainCursor1);
   }
   update();
}

void MyImgLabel::OnMoveCursor(QPoint &CursorPos)
{
#if 0
    qDebug()<<"MyImgLabel::OnMoveCursor(QPoint &CursorPos)";
    qDebug()<<CursorPos;
#endif
    MainCursor1=CursorPos;
    update();
}

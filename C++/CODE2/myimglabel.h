#ifndef MYIMGLABEL_H
#define MYIMGLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QPainter>

class MyImgLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyImgLabel(QWidget *parent = 0);

protected:
    void paintEvent ( QPaintEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
private:
    bool CursorMove;
    QPoint MainCursor1;
signals:
    void SignalCursorChanged(QPoint &CursorPos);
public slots:
    void OnMoveCursor(QPoint &CursorPos);

};

#endif // MYIMGLABEL_H

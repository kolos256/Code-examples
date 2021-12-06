#include "mainwindow.h"
#include <QApplication>
#include <QLinkedList>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include "pointdatalist.h"

int main(int argc, char *argv[])
{
    //PointDataList::AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY,quint16 FRQ,quint16 PWR,quint16 TIME,quint16 WaveLength)
    //MainPointDataList.AddPointTail(QString("TR23"),QString("Параорбитальные"),QString("Процедуру проводить через закрытые веки"),QString("SLIDE001"),QString("Магнитная насадка 40мТл"),300,150,100,200,200,650);
    QApplication a(argc, argv);
    MainWindow w;
    QLinkedList<int>::iterator tmpit;
    QLinkedList<int> TestList1;

#if 0
    qDebug()<<"QDir::current(): "<<QDir::current();

    qDebug()<<"TST1"<<QFileInfo("TEST1/TEST2/TREST3.csv").fileName().length();

    qDebug()<<"TST2"<<QString("test1").length();
#endif
#if 0
    tmpit=TestList1.begin();
    TestList1.append(55);
    tmpit=TestList1.begin();
    TestList1.append(66);
    tmpit++;
#endif
#if 0
    tmpit=TestList1.begin();
    *tmpit=44;
    qDebug()<<"*tmpit: "<<*tmpit;
    qDebug()<<"size: "<<TestList1.size();
#endif
#if 0
    TestList1.append(1);
    TestList1.append(2);
    TestList1.append(3);
#endif
#if 0
    TestList1.append(55);
    tmpit=TestList1.begin();
    TestList1.append(66);
    TestList1.append(77);
    TestList1.erase(tmpit);
    qDebug()<<"*tmpit: "<<*tmpit;
#endif
#if 0
    for(tmpit = TestList1.begin(); tmpit != TestList1.end(); ++tmpit)
    {
        qDebug()<<"*tmpit: "<<*tmpit;
    }
#endif

    w.show();
    return a.exec();
}

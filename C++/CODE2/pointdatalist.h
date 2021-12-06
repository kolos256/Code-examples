#ifndef POINTDATALIST_H
#define POINTDATALIST_H

#include <QApplication>
#include <QDebug>
#include <QLinkedList>
#include <QFile>

#if 0
/*****************************************************************************************************************************************************************************************************
 *
 * Point                                                                            |        PointName     |      BAPName         |      Description     |      SlidePath       |     NozzleName
 * +-----------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 * |PrevPointSh|NextPointSh |   PointFrq |  PointPwr  |  PointTime |PointWaveLength |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text
 * +-----------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 * |    2b     |     2b     |     2b     |     2b     |     2b     |       2b       |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb
 * +-----------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 *
 *****************************************************************************************************************************************************************************************************/

class PointDataList
{
public:
    PointDataList();
    typedef quint16 PointDataListElemSize;

    struct PointHeader{
        quint16 PrevPointSh;
        quint16 NextPointSh;
        quint16 FRQ;
        quint16 PWR;
        quint16 TIME;
        quint16 WaveLength;
        quint16 X;
        quint16 Y;
    };

    //void AddPoint(const char* PointName,const char* BAPName,const char* Description,const char* SlidePath,const char* NozzleName, quint16 CursorX,quint16 CursorY);
    //void AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY);
    void AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY,quint16 FRQ,quint16 PWR,quint16 TIME,quint16 WaveLength);
    void* NextPointAddr();
private:
    qint32 WriteString(quint8 *MemAddr,qint32 MemSize,const QString &Str);
    quint8 *ListHead;
    quint8 *CurPoint;
    quint8 *ListTail;
};
#endif


/************************************************************************************************************************************************************************************************************************
 *
 * Point                                                                                         |        PointName     |      BAPName         |      Description     |      SlidePath       |     NozzleName
 * +-----------+------------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 * |PrevPointSh|NextPointSh | SessionNbr |   PointFrq |  PointPwr  |  PointTime |PointWaveLength |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text
 * +-----------+------------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 * |    2b     |     2b     |     2b     |     2b     |     2b     |     2b     |       2b       |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb
 * +-----------+------------+------------+------------+------------+------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+-------
 *
 ***********************************************************************************************************************************************************************************************************************/

class PointDataList
{
public:
    PointDataList();
    ~PointDataList();
    struct PointData{
        //quint16 PrevPointSh;
        //quint16 NextPointSh;
        quint16 SessionNbr;
        quint16 FRQ;
        quint16 PWR;
        quint16 TIME;
        quint16 WaveLength;
        quint16 X;
        quint16 Y;
        QString PointName;
        QString Description;
        QString SlidePath;
        QString NozzleName;
        QString BAPName;
        QString Description1;
        void Print();
    };


    //void AddPoint(const char* PointName,const char* BAPName,const char* Description,const char* SlidePath,const char* NozzleName, quint16 CursorX,quint16 CursorY);
    //void AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY);
#if 0
    void AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY,quint16 FRQ,quint16 PWR,quint16 TIME,quint16 WaveLength);
#endif

#if 1
    void AddPoint(void);
#else
    void AddPoint(const PointData &Data);
#endif
    bool MoveNextPoint();
    bool MovePrevPoint();
    PointDataList::PointData* GetCurrentPointData(void);
    bool SetCurrentPointData(const PointData& Data);
    bool RemoveCurrent();
    bool IsFirstPoint();
    bool IsLastPoint();
    int ChekcPoints();
    quint32 GetCurPointId();
    quint32 GetPoints();
    void Savetofilefmt1(QFile &File);
    quint8 LoadFromfilefmt1( QFile &File);
    void WriteCountStr8(QString &Str, QFile &File);
    quint8 ReadCountStr8(QString &Str, QFile &File);
    void SetName(const QString &Name);
    const QString &GetName() const;
    //void* NextPointAddr();
private:

    struct Fmt1FileHeader{
        quint16 PrevPointSh;
        quint16 NextPointSh;
        quint16 SessionNbr;
        quint16 FRQ;
        quint16 PWR;
        quint16 TIME;
        quint16 WaveLength;
        quint16 X;
        quint16 Y;
    };
    void setDefaultData(PointData &Data);
    QLinkedList<PointData*> PointsList;
    QLinkedList<PointData*>::iterator it;
    qint32 CurPointId;
    QString Name;
};

extern PointDataList MainPointDataList;

#endif // POINTDATALIST_H

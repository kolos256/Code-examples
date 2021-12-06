#include "pointdatalist.h"

#include "mainwindow.h"

#if 0
extern const quint16 WAVELENGTHTBL1[];
extern const quint16 FRQTBL1[];
extern const quint16 TIMETBL1[];
//850нм
extern const quint16 POWERTBL1[];
//670нм
extern const quint16 POWERTBL2[];
#endif

#if 0
PointDataList::PointDataList()
{
    CurPoint=ListHead=NULL;
}
void* PointDataList::NextPointAddr()
{
    if(CurPoint==NULL)
    {
        return NULL;
    }
    else
    {
        return (*(reinterpret_cast<quint16*>(CurPoint))+CurPoint);
    }
}
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

qint32 PointDataList::WriteString(quint8 *MemAddr,qint32 MemSize,const QString &Str)
{
    if(MemSize<(Str.size()+1))
    {
        return 0;
    }
    *MemAddr=Str.size();
    memcpy((MemAddr+1),Str.toLocal8Bit().constData(),Str.size());
    return (Str.size()+1);
}

void PointDataList::AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY,quint16 FRQ,quint16 PWR,quint16 TIME,quint16 WaveLength)
{
    PointHeader *TMPHEAD;
    quint8 *NewPoint;
    qint32 TMP1,TMPSize=sizeof(PointHeader)+PointName.size()+BAPName.size()+Description.size()+SlidePath.size()+NozzleName.size();
    Q_UNUSED(TMPHEAD);
    NewPoint = new quint8[TMPSize];
    TMPHEAD = reinterpret_cast<PointHeader*>(NewPoint);
    TMPHEAD->FRQ=FRQ;
    TMPHEAD->PWR=PWR;
    TMPHEAD->TIME=TIME;
    TMPHEAD->WaveLength=WaveLength;

    TMPSize-=sizeof(PointHeader);
    NewPoint+=sizeof(PointHeader);
    //Save PointName
    if((TMP1=WriteString(NewPoint,TMPSize,PointName))==0)
    {
        qDebug()<<"Write PointName error"<<endl;
        return;
    }
#if 1
    //Save BAPName
    TMPSize-=TMP1;
    NewPoint+=TMP1;
    if((TMP1=WriteString(NewPoint,TMPSize,BAPName))==0)
    {
        qDebug()<<"Write BAPName error"<<endl;
        return;
    }

    //Description
    TMPSize-=TMP1;
    NewPoint+=TMP1;
    if((TMP1=WriteString(NewPoint,TMPSize,Description))==0)
    {
        qDebug()<<"Write Description error"<<endl;
        return;
    }
#endif
    Q_UNUSED(PointName);
    Q_UNUSED(BAPName);
    Q_UNUSED(Description);
    Q_UNUSED(SlidePath);
    Q_UNUSED(NozzleName);
    Q_UNUSED(CursorX);
    Q_UNUSED(CursorY);
}
#endif
PointDataList::PointDataList()
{
    PointData *NewData;
//#define DBG
    #if defined(DBG)
    qDebug()<<"PointDataList::PointDataList()";
    #endif
#if defined(DBG)&&0
    QLinkedList<PointData*>::iterator tmpit;
    tmpit=PointsList.begin();
    qDebug()<<"tst1"<<(tmpit==PointsList.begin());
    qDebug()<<"tst2"<<(tmpit==PointsList.end());
#endif
    //PointsList.begin();
    //it=PointsList.begin();
    CurPointId=1;
    NewData=new PointData;
    setDefaultData(*NewData);
    PointsList.append(NewData);
    //PointsList.append(new PointData);
    it=PointsList.begin();
    //*it=new PointData;
#if defined(DBG)
    QLinkedList<PointData*>::iterator tmpit;
    tmpit=PointsList.begin();
    qDebug()<<"tst1"<<(it!=PointsList.begin());
    qDebug()<<"tst2"<<(it!=PointsList.end());
#endif

#undef DBG
}

PointDataList::~PointDataList()
{
    qDebug()<<"PointDataList::~PointDataList()"<<endl;
    QLinkedList<PointData*>::iterator tmpit;
    for(tmpit = PointsList.begin(); tmpit != PointsList.end(); ++tmpit)
    {
        delete *tmpit;
    }
}
#if 0
void PointDataList::AddPointTail(const QString &PointName,const QString &BAPName,const QString &Description,const QString &SlidePath,const QString &NozzleName, quint16 CursorX,quint16 CursorY,quint16 FRQ,quint16 PWR,quint16 TIME,quint16 WaveLength)
{
    PointData *NEWPOINT;
    NEWPOINT= new PointData;
    setDefaultData(*NEWPOINT);
    NEWPOINT->PointName=PointName;
    NEWPOINT->NozzleName=NozzleName;
    NEWPOINT->SlidePath=SlidePath;
    NEWPOINT->Description=Description;
    NEWPOINT->BAPName=BAPName;
    NEWPOINT->X=CursorX;
    NEWPOINT->Y=CursorY;
    NEWPOINT->FRQ=FRQ;
    NEWPOINT->PWR=PWR;
    NEWPOINT->TIME=TIME;
    NEWPOINT->WaveLength=WaveLength;
    PointsList.append(NEWPOINT);
    //qDebug()<<PointsList.end();
}
#endif

bool PointDataList::MoveNextPoint()
{
//#define DBG
    #if defined(DBG)
    qDebug()<<"MoveNextPoint(): ";
    qDebug()<<"tst1"<<(it!=PointsList.begin());
    qDebug()<<"tst2"<<(it!=PointsList.end());
    #endif
    if((it+1)!=PointsList.end())
    {
        it++;
        CurPointId++;
        #if defined(DBG)
        qDebug()<<"CurPointId1: "<<CurPointId<<endl;
        #endif
        return true;
    }
    else
    {
        #if defined(DBG)
        qDebug()<<"CurPointId2: "<<CurPointId<<endl;
        #endif
        return false;
    }
#undef DBG
}

bool PointDataList::MovePrevPoint()
{
//#define DBG
    #if defined(DBG)
    qDebug()<<"MovePrevPoint(): ";
    qDebug()<<"tst1"<<(it!=PointsList.begin());
    qDebug()<<"tst2"<<(it!=PointsList.end());
    #endif
    if(it!=PointsList.begin())
    {
       CurPointId--;
       it--;
        #if defined(DBG)
        qDebug()<<"CurPointId1: "<<CurPointId<<endl;
        #endif
        return true;
    }
    else
    {
        #if defined(DBG)
        qDebug()<<"CurPointId2: "<<CurPointId<<endl;
        #endif
        return false;
    }
#undef DBG
}

PointDataList::PointData * PointDataList::GetCurrentPointData(void)
{
#if 0
    if(PointsList.begin()==PointsList.end())
    {
        return NULL;
    }
    else
    {
        return *it;
    }
#endif
    //memcpy(*it,&Data,sizeof(PointData));
    return *it;
}

bool PointDataList::SetCurrentPointData(const PointData &Data)
{
#if 0
    if(PointsList.begin()==PointsList.end())
    {
        return false;
    }
    else
    {
        memcpy(*it,&Data,sizeof(PointData));
        return true;
    }
#endif
    //memcpy(*it,&Data,sizeof(PointData));
    (*it)->PointName=Data.PointName;
    (*it)->BAPName=Data.BAPName;
    (*it)->Description=Data.Description;
    (*it)->NozzleName=Data.NozzleName;
    (*it)->SlidePath=Data.SlidePath;
    (*it)->Description1=Data.Description1;

    (*it)->SessionNbr=Data.SessionNbr;
    (*it)->FRQ=Data.FRQ;
    (*it)->PWR=Data.PWR;
    (*it)->TIME=Data.TIME;
    (*it)->WaveLength=Data.WaveLength;
    (*it)->X=Data.X;
    (*it)->Y=Data.Y;

#if 0
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
#endif

    //        Data.PointName=ui->PointNamelineEdit->text();
    //        Data.Description=ui->DescriptionLineEdit->text();
    //        Data.NozzleName=ui->NozzleNameLineEdit->text();
    //        Data.SlidePath=CurImgFilePath;


    return true;

}

bool PointDataList::RemoveCurrent(void)
{
    QLinkedList<PointData*>::iterator tmpit;
    if(PointsList.size()>1)
    {

        if(it==PointsList.begin())
        {
            tmpit=it;
            delete *tmpit;
            PointsList.erase(tmpit);
            it=(PointsList.begin());
        }
        else
        {
            tmpit=it;
            it--;
            delete *tmpit;
            PointsList.erase(tmpit);
            CurPointId--;
        }
        return true;
    }
    else
    {
        return false;
    }
}
#if 1
void PointDataList::AddPoint(void)
{
//#define DBG
    PointData *NewPoint = new PointData;
    setDefaultData(*NewPoint);
    //PointsList.insert(PointsList.end(),new PointData); //append(new PointData);
    //PointsList.push_front(new PointData);
    PointsList.append(NewPoint);
    #if 0
    if(CurPointId==0)
    {
        //it=PointsList.begin();
        CurPointId=1;
    }
    #endif
    #if 0
    if(PointsList.size()==1)
    {
        it=PointsList.begin();
        CurPointId=1;
    }
    #endif
    #if defined(DBG)
    qDebug()<<"Points: "<<PointsList.size()<<endl;
    #endif
    #if defined(DBG)
    QLinkedList<PointData*>::iterator tmpit;
    tmpit=PointsList.begin();
    qDebug()<<"tst1"<<(it!=PointsList.begin());
    qDebug()<<"tst2"<<(it!=PointsList.end());
    #endif
#undef DBG
}
#endif
#if 0
void PointDataList::AddPoint(const PointData &Data)
{
#define DBG
    PointData *NewData = new PointData;
    memcpy(NewData,&Data,sizeof(PointData));
    append(NewData);
    //PointsList.insert(PointsList.end(),new PointData); //append(new PointData);
    //PointsList.push_front(new PointData);
    #if 0
    if(CurPointId==0)
    {
        //it=PointsList.begin();
        CurPointId=1;
    }
    #endif
    #if 0
    if(PointsList.size()==1)
    {
        it=PointsList.begin();
        CurPointId=1;
    }
    #endif
    #if defined(DBG)
    qDebug()<<"Points: "<<PointsList.size()<<endl;
    #endif
    #if defined(DBG)
    QLinkedList<PointData*>::iterator tmpit;
    tmpit=PointsList.begin();
    qDebug()<<"tst1"<<(it!=PointsList.begin());
    qDebug()<<"tst2"<<(it!=PointsList.end());
    #endif
#undef DBG
}
#endif

quint32 PointDataList::GetCurPointId()
{
    return CurPointId;
}

quint32 PointDataList::GetPoints()
{
    return PointsList.size();
}

bool PointDataList::IsFirstPoint()
{
    return (it==PointsList.begin());
}

bool PointDataList::IsLastPoint()
{
    return ((it+1)==PointsList.end());
}

int PointDataList::ChekcPoints()
{
#if 0
    qDebug()<<"ChekcPoints: ";
#endif
    QLinkedList<PointData*>::iterator tmpit;
    int i=1;
    for(tmpit = PointsList.begin(); tmpit != PointsList.end(); ++tmpit)
    {
#if 0
        (*tmpit)->Print();
#endif
        if((*tmpit)->PointName=="")
        {
           return i;
        }
        if((*tmpit)->BAPName=="")
        {
            return i;
        }
        if((*tmpit)->Description=="")
        {
            return i;
        }
        if((*tmpit)->NozzleName=="")
        {
            return i;
        }
        if((*tmpit)->SlidePath=="")
        {
            return i;
        }
        if((*tmpit)->Description1=="")
        {
            return i;
        }

        i++;
        //qDebug()<<"*tmpit: "<<*tmpit;
    }
    return 0;
}

#if 0

inline int WAVELENGTHTOID(int Wavelength)
{
    return POSINTBL16(WAVELENGTHTBL1,sizeof(WAVELENGTHTBL1)/sizeof(WAVELENGTHTBL1[0]),Wavelength);
}
int PWRTOID(int Wavelength,int Power)
{
    if(Wavelength==850)
    {//850nm
        return POSINTBL16(POWERTBL1,sizeof(POWERTBL1)/sizeof(POWERTBL1[0]),Power);
    }
    else
    {//670nm
        return POSINTBL16(POWERTBL2,sizeof(POWERTBL2)/sizeof(POWERTBL2[0]),Power);
    }
}

inline int FRQTOID(int FRQ)
{
    return POSINTBL16(FRQTBL1,sizeof(FRQTBL1)/sizeof(FRQTBL1[0]),FRQ);
}

inline int TIMETOID(int TIME)

#endif

void PointDataList::setDefaultData(PointData &Data)
{
    Data.PointName="";
    Data.NozzleName="";
    Data.SlidePath="";
    Data.Description="";
    Data.BAPName="";
    Data.Description1="";
    Data.X=10;
    Data.Y=10;
    Data.SessionNbr=1;
    Data.FRQ=0;
    //Data.FRQ=FRQTBL1[0];
    //Data.PWR=POWERTBL2[0];//670nm
    //Data.TIME=TIMETBL1[0];
    Data.PWR=1;
    Data.TIME=1;
    Data.WaveLength=WAVELENGTHTBL1[0];

}






/********************************************************************************************************************************************************************************************************************************************************************
 *
 * Point                                                                                                              |      PointName       |      BAPName         |      Description1    |      SlidePath       |     NozzleName      |      Description2
 * +-----------+------------+------------+------------+------------+----------------+----------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+------- +------------+-------
 * |PrevPointSh|NextPointSh |   PointFrq |  PointPwr  |  PointTime |PointWaveLength |     PointX     |     PointY     |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text   |   size     |  text  |   size     |  text
 * +-----------+------------+------------+------------+------------+----------------+----------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+------- +------------+-------
 * |    2b     |     2b     |     2b     |     2b     |     2b     |       2b       |       2b       |       2b       |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb   |    1b      | sizeb  |    1b      | sizeb
 * +-----------+------------+------------+------------+------------+----------------+----------------+----------------+------------+-------  +------------+-------  +------------+-------  +------------+-------  +------------+------- +------------+-------
 *
 ********************************************************************************************************************************************************************************************************************************************************************/
#if 0
struct Fmt1FileHeader{
    quint16 PrevPointSh;
    quint16 NextPointSh;
    quint16 FRQ;
    quint16 PWR;
    quint16 TIME;
    quint16 WaveLength;
    quint16 X;
    quint16 Y;
};
#endif
void PointDataList::Savetofilefmt1( QFile &File)
{
    QLinkedList<PointData*>::iterator tmpit;
    Fmt1FileHeader Header;
    quint16 PrevElemSize,CurElemSize;
    QString IMGNAME;
#if 0
    Data.PointName="";
    Data.NozzleName="";
    PaData.Slideth="";
    Data.Description="";
    Data.BAPName="";
    Data.X=10;
    Data.Y=10;
    Data.FRQ=FRQTBL1[0];
    Data.PWR=POWERTBL2[0];//670nm
    Data.TIME=TIMETBL1[0];
    Data.WaveLength=WAVELENGTHTBL1[0];

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

#endif
    //Write header
    File.write("\xA5" "m02",4);
    WriteCountStr8(Name,File);
    PrevElemSize=0;
    for(tmpit = PointsList.begin(); tmpit != PointsList.end(); ++tmpit)
    {
        IMGNAME=QFileInfo((*tmpit)->SlidePath).fileName();
        #if defined(DBG)
        qDebug()<<"IMGNAME: "<<IMGNAME<<endl;
        #endif
        if(tmpit+1!=PointsList.end())
        {
            CurElemSize=sizeof(Fmt1FileHeader)+((*tmpit)->PointName.size()+2)+((*tmpit)->NozzleName.size()+2)+
                            (IMGNAME.size()+2)+((*tmpit)->Description.size()+2)+((*tmpit)->BAPName.size()+2)+((*tmpit)->Description1.size()+2);
        }
        else
        {
            CurElemSize=0;
        }
        Header.PrevPointSh=PrevElemSize;
        Header.NextPointSh=CurElemSize;
        Header.SessionNbr=(*tmpit)->SessionNbr;
        Header.FRQ=(*tmpit)->FRQ;
        Header.PWR=(*tmpit)->PWR;
        Header.TIME=(*tmpit)->TIME;
        Header.WaveLength=(*tmpit)->WaveLength;
        Header.X=(*tmpit)->X;
        Header.Y=(*tmpit)->Y;
        File.write(reinterpret_cast<const char*>(&Header),sizeof(Fmt1FileHeader));
        WriteCountStr8(((*tmpit)->PointName),File);
        WriteCountStr8(((*tmpit)->BAPName),File);
        WriteCountStr8(((*tmpit)->Description),File);
        WriteCountStr8((IMGNAME),File);
        WriteCountStr8(((*tmpit)->NozzleName),File);
        WriteCountStr8(((*tmpit)->Description1),File);
        PrevElemSize=CurElemSize;
        File.flush();
    }
}


quint8 PointDataList::LoadFromfilefmt1( QFile &File)
{
    static QString TMPSTR1;
    quint32 TMP1;
    PointData *NewData;
    Fmt1FileHeader Header;
#if defined(DBG)
    qDebug()<<"LoadFromfilefmt1:";
#endif
    //#define DBG
    //qDebug()<<"PointDataList::PointDataList()";
    QLinkedList<PointData*>::iterator tmpit;
    for(tmpit = PointsList.begin(); tmpit != PointsList.end(); ++tmpit)
    {
        delete *tmpit;
    }
    PointsList.clear();
    CurPointId=1;
    //NewData=new PointData;
    //setDefaultData(*NewData);
    //PointsList.append(NewData);
    //PointsList.append(new PointData);
    if(!File.seek(0))
    {
        #if defined(DBG)
        qDebug()<<"err1:";
        #endif
        return 1;
    }
    if(File.read(reinterpret_cast<char*>(&TMP1),4)!=4)
    {
        #if defined(DBG)
        qDebug()<<"err2:";
        #endif
        return 2;
    }
    //qDebug()<<"TMP1:"<<TMP1;
    if(TMP1!=0x32306DA5)
    {
        #if defined(DBG)
        qDebug()<<"err3:";
        #endif
        return 3;
    }
    if(ReadCountStr8(Name,File))
    {
        #if defined(DBG)
        qDebug()<<"err4:";
        #endif
        return 4;
    }
    #if defined(DBG)
    qDebug()<<"TSTNAME: "<<Name<<endl;
    #endif
    do
    {
        NewData=new PointData;
        if(File.read(reinterpret_cast<char*>(&Header),sizeof(Fmt1FileHeader))!=sizeof(Fmt1FileHeader))
        {
            #if defined(DBG)
            qDebug()<<"err5:";
            #endif
            return 5;
        }
        //qDebug()<<"Header.NextPointSh: "<<Header.NextPointSh<<endl;
        //Header.PrevPointSh=PrevElemSize;
        //Header.NextPointSh=CurElemSize;

        NewData->SessionNbr=Header.SessionNbr;
        NewData->FRQ=Header.FRQ;
        NewData->PWR=Header.PWR;
        NewData->TIME=Header.TIME;
        NewData->WaveLength=Header.WaveLength;
        NewData->X=Header.X;
        NewData->Y=Header.Y;
#if 0
        WriteCountStr8(((*tmpit)->PointName),File);
        WriteCountStr8(((*tmpit)->BAPName),File);
        WriteCountStr8(((*tmpit)->Description),File);
        WriteCountStr8((IMGNAME),File);
        WriteCountStr8(((*tmpit)->NozzleName),File);
        WriteCountStr8(((*tmpit)->Description1),File);
#endif

        if(ReadCountStr8((NewData->PointName),File))
        {
            #if defined(DBG)
            qDebug()<<"err6:";
            #endif
            return 6;
        }
        if(ReadCountStr8((NewData->BAPName),File))
        {
            #if defined(DBG)
            qDebug()<<"err7:";
            #endif
            return 7;
        }
        if(ReadCountStr8((NewData->Description),File))
        {
            #if defined(DBG)
            qDebug()<<"err8:";
            #endif
            return 8;
        }
        if(ReadCountStr8((NewData->SlidePath),File))
        {
            #if defined(DBG)
            qDebug()<<"err9:";
            #endif
            return 9;
        }
#if 0
        if(ReadCountStr8(TMPSTR1,File))
        {
            qDebug()<<"err9:";
            return 9;
        }
#endif
        //qDebug()<<"TMPSTR1: "<<TMPSTR1<<"LTH: "<<TMPSTR1.length()<<endl;
        //NewData->SlidePath=(QDir::currentPath()+"/IMGS/"+TMPSTR1);

        //NewData->SlidePath=(QString("./IMGS/"+TMPSTR1));

#if 0
        NewData->SlidePath=QString("./IMGS/"+NewData->SlidePath);
#else
        NewData->SlidePath=QString("./FILES/IMGS/"+NewData->SlidePath);
#endif
        //qDebug()<<"NewData->SlidePath: "<<NewData->SlidePath<<"Test2";
        if(ReadCountStr8((NewData->NozzleName),File))
        {
            #if defined(DBG)
            qDebug()<<"err10:";
            #endif
            return 10;
        }
        if(ReadCountStr8((NewData->Description1),File))
        {
            #if defined(DBG)
            qDebug()<<"err11:";
            #endif
            return 11;
        }
        //NewData->Print();
        PointsList.append(NewData);

#if 0
        Header.FRQ=(*tmpit)->FRQ;
        Header.PWR=(*tmpit)->PWR;
        Header.TIME=(*tmpit)->TIME;
        Header.WaveLength=(*tmpit)->WaveLength;
        Header.X=(*tmpit)->X;
        Header.Y=(*tmpit)->Y;
#endif
    }
    while(Header.NextPointSh!=0);
    it=PointsList.begin();
    return 0;
#if 0
    QLinkedList<PointData*>::iterator tmpit;
    Fmt1FileHeader Header;
    quint16 PrevElemSize,CurElemSize;
    QString IMGNAME;

    //Write header
    File.write("\xA5" "m02",4);
    File.read(,4)!=4
    WriteCountStr8(Name,File);
    PrevElemSize=0;
    for(tmpit = PointsList.begin(); tmpit != PointsList.end(); ++tmpit)
    {
        IMGNAME=QFileInfo((*tmpit)->SlidePath).fileName();
        if(tmpit+1!=PointsList.end())
        {
            CurElemSize=sizeof(Fmt1FileHeader)+((*tmpit)->PointName.size()+2)+((*tmpit)->NozzleName.size()+2)+
                            (IMGNAME.size()+2)+((*tmpit)->Description.size()+2)+((*tmpit)->BAPName.size()+2)+((*tmpit)->Description1.size()+2);
        }
        else
        {
            CurElemSize=0;
        }
        Header.PrevPointSh=PrevElemSize;
        Header.NextPointSh=CurElemSize;
        Header.FRQ=(*tmpit)->FRQ;
        Header.PWR=(*tmpit)->PWR;
        Header.TIME=(*tmpit)->TIME;
        Header.WaveLength=(*tmpit)->WaveLength;
        Header.X=(*tmpit)->X;
        Header.Y=(*tmpit)->Y;
        File.write(reinterpret_cast<const char*>(&Header),sizeof(Fmt1FileHeader));
        WriteCountStr8(((*tmpit)->PointName),File);
        WriteCountStr8(((*tmpit)->BAPName),File);
        WriteCountStr8(((*tmpit)->Description),File);
        WriteCountStr8((IMGNAME),File);
        WriteCountStr8(((*tmpit)->NozzleName),File);
        WriteCountStr8(((*tmpit)->Description1),File);
        PrevElemSize=CurElemSize;
        File.flush();
    }
#endif
}


void PointDataList::SetName(const QString &Name)
{
    PointDataList::Name=Name;
}
const QString &PointDataList::GetName() const
{
    //return const_cast<QString>(Name);
    return Name;
}

#if 0
void PointDataList::WriteCountStr8(QString &Str, QFile &File)
{
    quint8 counter=Str.size();
    //Write counter
    File.write(reinterpret_cast<const char*>(&counter),1);
    //Write string
    //File.write(Str.toLocal8Bit().toStdString().c_str(),counter);
    File.write(Str.toLocal8Bit().data(),counter);
    //File.flush();
}
#endif

void PointDataList::WriteCountStr8(QString &Str, QFile &File)
{
    quint8 counter=Str.size()+1;
    //Write counter
    File.write(reinterpret_cast<const char*>(&counter),1);
    //Write string
    //File.write(Str.toLocal8Bit().toStdString().c_str(),counter);
    File.write(Str.toLocal8Bit().data(),(counter-1));
    File.write("",1);
    //File.flush();
}

quint8 PointDataList::ReadCountStr8(QString &Str, QFile &File)
{
    quint8 StrBuffer[256],Size;
    if(File.read(reinterpret_cast<char*>(&Size),1)!=1)
    {
        #if defined(DBG)
        qDebug()<<"ReadCountStr8 err1";
        #endif
        return 1;
    }
    if((Size)>sizeof(StrBuffer))
    {
        #if defined(DBG)
        qDebug()<<"ReadCountStr8 err2";
        #endif
        return 2;
    }
    if(File.read(reinterpret_cast<char*>(StrBuffer),(Size))!=(Size))
    {
        #if defined(DBG)
        qDebug()<<"ReadCountStr8 err3";
        #endif
        return 3;
    }
    //Str.clear();
    Str=QString::fromLocal8Bit(reinterpret_cast<char*>(StrBuffer),(Size-1));
    return 0;
}

void PointDataList::PointData::Print()
{
#if 0
    qDebug()<<"PointName: "<<PointName;
    qDebug()<<"NozzleName: "<<NozzleName;
    qDebug()<<"SlidePath: "<<SlidePath;
    qDebug()<<"Description: "<<Description;
    qDebug()<<"BAPName: "<<BAPName;
    qDebug()<<"X: "<<X;
    qDebug()<<"Y: "<<Y;
    qDebug()<<"FRQ: "<<FRQ;
    qDebug()<<"PWR: "<<PWR;
    qDebug()<<"TIME: "<<TIME;
    qDebug()<<"WaveLength: "<<WaveLength;
#endif

    qDebug()<<"PointName: "<<PointName<<endl;
    qDebug()<<"NozzleName: "<<NozzleName<<endl;
    qDebug()<<"SlidePath: "<<SlidePath<<endl;
    qDebug()<<"Description: "<<Description<<endl;
    qDebug()<<"BAPName: "<<BAPName<<endl;
    qDebug()<<"X: "<<X<<endl;
    qDebug()<<"Y: "<<Y<<endl;
    qDebug()<<"FRQ: "<<FRQ<<endl;
    qDebug()<<"PWR: "<<PWR<<endl;
    qDebug()<<"TIME: "<<TIME<<endl;
    qDebug()<<"WaveLength: "<<WaveLength<<endl;

}

PointDataList MainPointDataList;

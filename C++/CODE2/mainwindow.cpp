#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointdatalist.h"

const quint16 WAVELENGTHTBL1[]={670,850};
const quint16 FRQTBL1[]={0,1,2,4,5,8,12,80,100,130,200,500,800,1000,1500,2000};
const quint16 TIMETBL1[]={2,3,4,6,8,12,15,25,30,50,65,100,130,200,250,400};
//850нм
const quint16 POWERTBL1[]={10,20,30,40,50,60,70,80,90,100,110,120,140,160,180,200};
//670нм
const quint16 POWERTBL2[]={4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64};


int TIMETEXTTOVALUE(const QString &Text)
{
    if(Text=="Непрерывный")
    {
        return 0;
    }
    else
    {
        return Text.toInt();
    }
}

int POSINTBL16(const quint16 *TBL16,size_t TBL16_SIZE,quint16 VALUE)
{
    size_t i=0;
    while((*TBL16!=VALUE)&&(i<TBL16_SIZE))
    {
        TBL16++;
        i++;
    }
    if(*TBL16==VALUE)
    {
        return i;
    }
    else
    {
        return -1;
    }
}

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
{
    return POSINTBL16(TIMETBL1,sizeof(TIMETBL1)/sizeof(TIMETBL1[0]),TIME);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),CurImgFilePath(""),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //MainPointDataList.AddPoint();

    //void OnCursorChanget(QPoint &CursorPos);
    connect(ui->ImgLabel,SIGNAL(SignalCursorChanged(QPoint&)),this,SLOT(OnCursorChanged(QPoint&)));
    connect(this,SIGNAL(SignalMoveCursor(QPoint&)),ui->ImgLabel,SLOT(OnMoveCursor(QPoint&)));
    connect(ui->WavelengthComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnWavelengthcurrentIndexChanged(int)));

    //void OnWavelengthcurrentIndexChanged(int index)
    //void OnMoveCursor(QPoint &CursorPos);
    //connect(ImgLabel,SIGNAL(OnCursorDataChanget(unsigned char,int,int,int,int)),this,SIGNAL(OnCursorDataChanget(unsigned char,int,int,int,int)));
    LoadPointData();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnCursorChanged(QPoint &CursorPos)
{
    ui->ImgXcursorLineEdit->setText(QString::number(CursorPos.x()));
    ui->ImgYcursorLineEdit->setText(QString::number(CursorPos.y()));
}
/***********************************************************
 *
 * Read Data from gui elements and save to MainPointDataList
 *
 ***********************************************************/
void MainWindow::SavePointData()
{
    PointDataList::PointData Data;
    Data.BAPName=ui->BAPNameLineEdit->text();
    Data.PointName=ui->PointNamelineEdit->text();
    Data.Description=ui->DescriptionLineEdit->text();
    Data.NozzleName=ui->NozzleNameLineEdit->text();

    Data.Description1=ui->Description1LineEdit->text();

    Data.SlidePath=CurImgFilePath;
#if 0
    Data.FRQ=ui->FrqComboBox->currentText().toInt();
#else
    Data.FRQ=ui->FrqSpinBox->value();
#endif

#if 0
    Data.PWR=ui->PowerComboBox->currentText().toInt();
#else
    Data.PWR=ui->PowerSpinBox->value();
#endif
    //Data.TIME=ui->TimeComboBox->currentText().toInt();
#if 0
    Data.TIME=TIMETEXTTOVALUE(ui->TimeComboBox->currentText());
#else
    Data.TIME=ui->TimeSpinBox->value();
#endif
    Data.WaveLength=ui->WavelengthComboBox->currentText().toInt();
    Data.X=ui->ImgXcursorLineEdit->text().toInt();
    Data.Y=ui->ImgYcursorLineEdit->text().toInt();

    Data.SessionNbr=ui->SassionSpinBox->value();

    qDebug()<<"SavePointData: ";
#if 0
    Data.Print();
#endif
    MainPointDataList.SetCurrentPointData(Data);
#if 0
    Data->SlidePath=SlidePath;
    Data->Description=Description;

    Data->X=CursorX;
    Data->Y=CursorY;
    Data->FRQ=FRQ;
    Data->PWR=PWR;
    Data->TIME=TIME;
    Data->WaveLength=WaveLength;
#endif
}
/*****************************************************
 *
 * Check how many points if MainPointDataList
 * and disable next prev remove buttons if required
 *
 *****************************************************/
void MainWindow::UpdateButtonsState()
{
    if(MainPointDataList.IsFirstPoint())
    {
        ui->PrevPointButton->setEnabled(false);
    }
    else
    {
        ui->PrevPointButton->setEnabled(true);
    }

    if(MainPointDataList.GetPoints()>1)
    {
        ui->RemovePointButton->setEnabled(true);
    }
    else
    {
        ui->RemovePointButton->setEnabled(false);
    }

    if(MainPointDataList.IsLastPoint())
    {
        ui->NextPointButton->setEnabled(false);
    }
    else
    {
        ui->NextPointButton->setEnabled(true);
    }
}

/*******************************************************
 *
 * Update amount of points
 *
 *******************************************************/

void MainWindow::UpdatePointNumber()
{
    ui->PointsLabel->setText( QString("Точка")+QString::number(MainPointDataList.GetCurPointId())+QString("/")+QString::number(MainPointDataList.GetPoints()) );
}

/***********************************************************
 *
 * Read Data from gui elements and save to MainPointDataList
 *
 ***********************************************************/

void MainWindow::LoadPointData()
{
    static QPixmap ImgPixmap;
    static QImage IMG1;
    PointDataList::PointData *Data=MainPointDataList.GetCurrentPointData();
    static QPoint CursorPos;
#if defined(DBG)
    qDebug()<<"LoadPointData: ";
#endif
    //Data->Print();
    //CurImgFilePath=Data->SlidePath;

    ui->BAPNameLineEdit->setText(Data->BAPName);
    ui->PointNamelineEdit->setText(Data->PointName);
    ui->DescriptionLineEdit->setText(Data->Description);
    ui->NozzleNameLineEdit->setText(Data->NozzleName);

    ui->Description1LineEdit->setText(Data->Description1);

    ui->SassionSpinBox->setValue(Data->SessionNbr);


#if 0
    qDebug()<<"TestCurImgFilePath1: "<<CurImgFilePath<<" Test1"<<endl;
    qDebug()<<"Test1";
#endif

    if(Data->SlidePath=="")
    {
        CurImgFilePath="./RSC/defaultimg1.jpg";
        //ImgPixmap=QPixmap::fromImage(QImage(QDir::currentPath()+"/RSC/defaultimg1.jpg"));
    }
    else
    {
        CurImgFilePath=Data->SlidePath;
        //CurImgFilePath="./IMGS/002.jpg";
    }
#if defined(DBG)
    qDebug()<<"TestCurImgFilePath1: "<<CurImgFilePath<<" LTH "<<CurImgFilePath.length()<<" Test1:"<< Data->SlidePath<<" LTH "<<Data->SlidePath.length()<<endl;
#endif

    //ui->ImgLabel->setPixmap(QPixmap::fromImage(IMG1));!!!

    //ui->ImgLabel->setPixmap(QPixmap::fromImage(QImage(QString("./IMGS/002.jpg"))));
    ui->ImgLabel->setPixmap(QPixmap::fromImage(QImage(CurImgFilePath)));

    ui->WavelengthComboBox->setCurrentIndex(WAVELENGTHTOID(Data->WaveLength));
#if 0
    ui->PowerComboBox->setCurrentIndex(PWRTOID(Data->WaveLength,Data->PWR));
#else
    ui->PowerSpinBox->setValue(Data->PWR);
#endif
#if 0
    ui->FrqComboBox->setCurrentIndex(FRQTOID(Data->FRQ));
#else
    ui->FrqSpinBox->setValue(Data->FRQ);
#endif
#if 0
    ui->TimeComboBox->setCurrentIndex(TIMETOID(Data->TIME));
#else
    ui->TimeSpinBox->setValue(Data->TIME);
#endif

    ui->ImgXcursorLineEdit->setText(QString::number(Data->X));
    ui->ImgYcursorLineEdit->setText(QString::number(Data->Y));
    //Data.X=ui->ImgXcursorLineEdit->text().toInt();
    //Data.Y=ui->ImgYcursorLineEdit->text().toInt();
    CursorPos=QPoint(Data->X,Data->Y);
    SignalMoveCursor(CursorPos);

}


int MainWindow::CheckPointData()
{
    if(ui->BAPNameLineEdit->text()=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введите название БАТ!!!"));
        return 1;
    }
    if(ui->PointNamelineEdit->text()=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введите Наименование точки!!!"));
        return 2;
    }
    if(ui->DescriptionLineEdit->text()=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введите Примечание!!!"));
        return 3;
    }
    if(ui->NozzleNameLineEdit->text()=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введите название насадки!!!"));
        return 4;
    }
    if(CurImgFilePath=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Выберите изображение!!!"));
        return 4;
    }


    return 0;
}

void MainWindow::on_AddPoinButton_released()
{
    //if(CheckPointData()==0)
    {
        MainPointDataList.AddPoint();
    }
    UpdatePointNumber();
    UpdateButtonsState();
}

void MainWindow::on_PrevPointButton_released()
{
    //Точка 1/1

    //if(CheckPointData()==0)
    {
        SavePointData();
        if(MainPointDataList.MovePrevPoint())
        {
            LoadPointData();
        }
    }
    UpdatePointNumber();
    UpdateButtonsState();
}

void MainWindow::on_NextPointButton_released()
{
    //if(CheckPointData()==0)
    {
        SavePointData();
        if(MainPointDataList.MoveNextPoint())
        {
            LoadPointData();
        }
    }
    UpdatePointNumber();
    UpdateButtonsState();
}
#if 0
//850нм
const quint16 POWERTBL1[]={10,20,30,40,50,60,70,80,90,100,110,120,140,160,180,200};
//670нм
const quint16 POWERTBL2[]={4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64};
#endif

void MainWindow::OnWavelengthcurrentIndexChanged(int index)
{
    int i;
#if 0
    //qDebug()<<"ui->WavelengthComboBox->size(): "<<ui->WavelengthComboBox->size();
    /*for(i=0;i<ui->WavelengthComboBox->count();i++)
    {

    }*/

    while(ui->PowerComboBox->count()>0)
    {
        ui->PowerComboBox->removeItem(ui->PowerComboBox->count()-1);
    }
    //ui->WavelengthComboBox->size()
    //ui->WavelengthComboBox->removeItem();
#if 1
    if(index==0)
    {
        for(i=0;i<sizeof(POWERTBL2)/sizeof(POWERTBL2[0]);i++)
        {
            ui->PowerComboBox->addItem(QString::number(POWERTBL2[i]));
        }
    }
    else
    {
        for(i=0;i<sizeof(POWERTBL1)/sizeof(POWERTBL1[0]);i++)
        {
            ui->PowerComboBox->addItem(QString::number(POWERTBL1[i]));
        }
    }
#endif
#endif
}


void MainWindow::on_RemovePointButton_released()
{
    MainPointDataList.RemoveCurrent();
    LoadPointData();
    UpdatePointNumber();
    UpdateButtonsState();
}

void MainWindow::on_SelectImgButton_released()
{
#if 1
    //QString FileName;
    //QImage ImgFromFile;
    QFileDialog dlg(this, "Select Image");
    //dlg.setHistory(QStringList(""));
    //qDebug()<<"history: "<<dlg.history();
    //dlg.setFilter(QString(""));
    //dlg.setFileMode(QFileDialog::ExistingFile);
    //dlg.setOption(QFileDialog::DontUseNativeDialog, true);
    CurImgFilePath=dlg.getOpenFileName(this,tr("Open Image"),"./FILES/IMGS/", tr("Image Files (*.jpg *.bmp)"));
#if defined (DBG)
    qDebug()<<"CurImgFilePath1: "<<CurImgFilePath;
#endif
    //ImgFromFile=QImage(FileName);
    if(CurImgFilePath!="")
    {
        //Ui::MainWindow::ImgLabel ImgLabel->
        ui->ImgLabel->setPixmap(QPixmap::fromImage(QImage(CurImgFilePath)));
        ui->ImgLabel->setMaximumSize(ui->ImgLabel->pixmap()->size());
        ui->ImgLabel->setMinimumSize(ui->ImgLabel->pixmap()->size());
        ui->ImgLabel->show();
    }
    //ui-> QImage(FileName)
    //QFileDialog::getOpenFileName(this,tr("Open Image"), QDir::currentPath()+"/IMGS/", tr("Image Files (*.jpg *.bmp)"));
#endif
}

void MainWindow::on_SaveScenarioButton_released()
{
    QFile ScenarioFile;


    SavePointData();
#if 0
    if(ui->ScenarioNameLineEdit->text()=="")
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введите название сценария!!!"));
        return;
    }

    if(MainPointDataList.ChekcPoints()!=0)
    {
        QMessageBox::critical(this, tr("Ошибка"),("Введены значения не для всех точек!!!"));
        return;
    }
#endif
    //ui->ScenarioNumberSpinBox-> ScenarioNumberSpinBox
    //ScenarioFile.setFileName("tst1.m02");
    ScenarioFile.setFileName(QString("./FILES/SCN/SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+".m02");
    if(!ScenarioFile.open(QFile::ReadWrite))
    {
        QMessageBox::critical(this, tr("Error"),("Не удалось создать файл: "+ScenarioFile.fileName()));
        ScenarioFile.close();
        return;
    }
    MainPointDataList.SetName(ui->ScenarioNameLineEdit->text());
    MainPointDataList.Savetofilefmt1(ScenarioFile);
    ScenarioFile.flush();
    ScenarioFile.close();
}

void MainWindow::on_LoadScenarioButton_released()
{
    //QFile ScenarioFile(QDir::currentPath()+QString("/SCN/")+QString("SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+QString(".m02"));
    QFile ScenarioFile;
#if defined (DBG)
    qDebug()<<"on_LoadScenarioButton_released";
    //QString ScnFilePath=QDir::currentPath()+QString("/SCN/")+QString("SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+".m02";
    //ScenarioFile.open()
    qDebug()<<QDir::currentPath();
#endif
    if(!QFile::exists(QString("./FILES/SCN/SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+QString(".m02")))
    {
        QMessageBox::critical(this, tr("Error"),("Файл: "+QString("SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+QString(".m02")+QString("не найден")));
        return;
    }
    ScenarioFile.setFileName((QString("./FILES/SCN/SCN")+QString::number(ui->ScenarioNumberSpinBox->value())+QString(".m02")));
    if(!ScenarioFile.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"),("Не удалось открыть файл: "+ScenarioFile.fileName()));
        ScenarioFile.close();
        return;
    }
    MainPointDataList.LoadFromfilefmt1(ScenarioFile);
    ui->ScenarioNameLineEdit->setText(MainPointDataList.GetName());
#if defined(DBG)
    qDebug()<<"-------------------1----------------";
#endif
    LoadPointData();
    ScenarioFile.close();
    UpdateButtonsState();
    UpdatePointNumber();
    //ScenarioFile.s
}

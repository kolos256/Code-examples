#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

extern const quint16 WAVELENGTHTBL1[];
extern const quint16 FRQTBL1[];
extern const quint16 TIMETBL1[];
//850нм
extern const quint16 POWERTBL1[];
//670нм
extern const quint16 POWERTBL2[];


extern int POSINTBL16(const quint16 *TBL16,size_t TBL16_SIZE,quint16 VALUE);
extern inline int WAVELENGTHTOID(int Wavelength);
extern int PWRTOID(int Wavelength,int Power);
extern inline int FRQTOID(int FRQ);
extern inline int TIMETOID(int TIME);
extern int TIMETEXTTOVALUE(const QString &Text);


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QString CurImgFilePath;
    void SavePointData();
    int CheckPointData();
    void LoadPointData();
    void UpdateButtonsState();
    void UpdatePointNumber();
private slots:
    void on_AddPoinButton_released();

    void on_PrevPointButton_released();

    void on_NextPointButton_released();

    void on_RemovePointButton_released();

    void on_SelectImgButton_released();

    void OnCursorChanged(QPoint &CursorPos);

    void OnWavelengthcurrentIndexChanged(int index);

    void on_SaveScenarioButton_released();

    void on_LoadScenarioButton_released();

signals:
    void SignalMoveCursor(QPoint& CursorPos);

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

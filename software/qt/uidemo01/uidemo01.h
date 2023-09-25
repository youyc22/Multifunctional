#ifndef UIDEMO01_H
#define UIDEMO01_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <QTimer>//定时器头文件
#include <QDateTime>//时间类，获取系统时间
#include <windowone.h>
#include <windowtwo.h>
namespace Ui {
class UIDemo01;

}

class UIDemo01 : public QDialog
{
    Q_OBJECT

public:
    explicit UIDemo01(QWidget *parent = 0);
    ~UIDemo01();
public:
    void addNewSelfDefined(QString str,int num);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::UIDemo01 *ui;
    QSerialPort *serialPort;
    QTimer *timer_calendar;
    QTimer *countdownTimer;
    QTimer *gestureTimer;
    QTimer *countdownTimer1;
    windowOne *windowone;
    windowTwo *windowtwo;
    int com6;
    int com7;
    int com8;
    int com9;
    int com11;
    int com15;
    int com16;
    int com17;
    int countdownValue;
    int countdownValued;
    int gesturejingTime;
    int gesturedongTime;

private slots:
    void initForm();
    void buttonClick();
    void onWindowOneButtonPressed();
    void onWindowOneButtonPressed1();
    void onWindowTwoButtonPressed();
    void onWindowTwoButtonPressed1();

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_closechuankou_clicked();
    void on_openchuankou_clicked();
private slots:
    void comstate_update();
    void on_pushButton_clickedggg(QTreeWidgetItem *item, int column);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_comboBox_6_currentIndexChanged(int index);
    void on_comboBox_7_currentIndexChanged(int index);
    void updateCountdown();
    void updatejingtime();
    void sendjingclose();
    void updateCountdownd();
    void updatedongtime();
    void senddongclose();
    void on_comboBox_17_currentIndexChanged(int index);
    void on_comboBox_15_currentIndexChanged(int index);
    void on_comboBox_9_currentIndexChanged(int index);
    void on_comboBox_8_currentIndexChanged(int index);
    void on_comboBox_11_currentIndexChanged(int index);
    void on_comboBox_16_currentIndexChanged(int index);
};

#endif // UIDEMO01_H

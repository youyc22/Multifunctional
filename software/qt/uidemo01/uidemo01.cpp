#pragma execution_character_set("utf-8")

#include "uidemo01.h"
#include "ui_uidemo01.h"
#include "iconhelper.h"
#include <QtWidgets>
#include "windowone.h"
#include "windowtwo.h"
#include "huishou.h"
#include <QSettings>

UIDemo01::UIDemo01(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UIDemo01)
{
  ui->setupUi(this);
  windowone = new windowOne(this);
  windowtwo = new windowTwo(this);
  timer_calendar = new QTimer(this);//new个对象
  connect(timer_calendar,SIGNAL(timeout()),this,SLOT(comstate_update()));//timeout超时事件
  timer_calendar->start(100);//每隔一秒调用一次槽函数
    //QTreeWidget *treeWidget=ui->treeWidget;
  connect(ui->treeWidget,&QTreeWidget::itemClicked,this,&UIDemo01::on_pushButton_clickedggg);

  QStringList serialNamePort;
     serialPort = new QSerialPort(this);
    //​/* 搜索所有可用串口 */
  foreach (const QSerialPortInfo &inf0, QSerialPortInfo::availablePorts())
    {
        serialNamePort<<inf0.portName();
    }
  ui->comboBox->addItems(serialNamePort);

  QScrollArea *scrollArea = new QScrollArea(ui->stackedWidget->widget(0));
     scrollArea->setGeometry(0, 0, 1200, 1000);
     scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // 创建滚动内容界面
  QWidget *scrollContent = new QWidget(scrollArea);
     scrollContent->setGeometry(0, 0, 1200, 1500);  // 实际界面参数
     scrollContent->setStyleSheet("background-color: #ECECEC;");
        // 添加滚动内容界面到滚动区域
     scrollArea->setWidget(scrollContent);

        // 在滚动内容界面上添加标签等内容
  QLabel *label_a = new QLabel(scrollContent);
     label_a->setText("Scrollable Content");
     label_a->move(0, 300);

        // 将滚动区域添加到第一个页面的布局中
  QVBoxLayout *layout = new QVBoxLayout(ui->stackedWidget->widget(0));
     layout->addWidget(scrollArea);

  QScrollArea *scrollArea1 = new QScrollArea(ui->stackedWidget->widget(1));
     scrollArea1->setGeometry(0, 0, 1200, 1000);
     scrollArea1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     scrollArea1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            // 创建滚动内容界面
  QWidget *scrollContent1 = new QWidget(scrollArea1);
     scrollContent1->setGeometry(0, 0, 1200, 1500);  // 实际界面参数
     scrollContent1->setStyleSheet("background-color: #ECECEC;");
            // 添加滚动内容界面到滚动区域
     scrollArea1->setWidget(scrollContent1);

            // 在滚动内容界面上添加标签等内容
  //QLabel *label1 = new QLabel(scrollContent1);
    // label1->setText("Scrollable Content");
     //label1->move(0, 300);

            // 将滚动区域添加到第一个页面的布局中
  QVBoxLayout *layout1 = new QVBoxLayout(ui->stackedWidget->widget(1));
     layout1->addWidget(scrollArea1);

       //ui->page3->setStyleSheet("background-color: #ECECEC;");

  setWindowFlags(Qt::FramelessWindowHint);
       //设置关于窗体为圆角
  QBitmap bmp(this->size());
    bmp.fill();
  QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),20,20);
    setMask(bmp);
  this->initForm();

  QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
        //Read
        settings->beginGroup("JING");
        com6 = settings->value("OK",-1).toInt();
        com7 = settings->value("THUMB",-1).toInt();
        com8 = settings->value("ERSHUN",-1).toInt();
        com9 = settings->value("SHIGOU",-1).toInt();
        com11 = settings->value("ERNI",-1).toInt();
        com15= settings->value("SHINI",-1).toInt();
        com16= settings->value("ERGOU",-1).toInt();
        com17= settings->value("SHISHUN",-1).toInt();
        settings->endGroup();
        //Write
        settings->beginGroup("MAIN");
        settings->setValue("NAME","YYC");
        settings->endGroup();

        settings->beginGroup("OTHER");
        //settings->setValue("CLASS","无23");
        QString s=settings->value("CLASS","").toString();
        qDebug()<<s;
        settings->endGroup();
  countdownTimer = new QTimer(this);
  countdownTimer1=new QTimer(this);
  gestureTimer=new QTimer(this);
  countdownValue=5;
  countdownValued=5;
  gesturejingTime=3;
  connect(windowone, &windowOne::buttonPressed, this, &UIDemo01::onWindowOneButtonPressed);
  connect(windowone, &windowOne::buttonPressed1, this, &UIDemo01::onWindowOneButtonPressed1);
  connect(windowone, &windowOne::closewindow,this,&UIDemo01::sendjingclose);
  connect(countdownTimer,&QTimer::timeout,this,&UIDemo01::updateCountdown);

  connect(windowtwo, &windowTwo::buttonPressed,this,&UIDemo01::onWindowTwoButtonPressed);
  connect(windowtwo, &windowTwo::buttonPressed1, this, &UIDemo01::onWindowTwoButtonPressed1);
  connect(windowtwo, &windowTwo::closewindow,this,&UIDemo01::senddongclose);
  connect(countdownTimer1,&QTimer::timeout,this,&UIDemo01::updateCountdownd);

  connect(gestureTimer,&QTimer::timeout,this,&UIDemo01::updatejingtime);
  ui->comboBox_6->setCurrentIndex(com6);
  ui->comboBox_7->setCurrentIndex(com7);
  ui->comboBox_8->setCurrentIndex(com8);
  ui->comboBox_9->setCurrentIndex(com9);
  ui->comboBox_11->setCurrentIndex(com11);
  ui->comboBox_15->setCurrentIndex(com15);
  ui->comboBox_16->setCurrentIndex(com16);
  ui->comboBox_17->setCurrentIndex(com17);
}

UIDemo01::~UIDemo01()
{
    delete ui;
}

bool UIDemo01::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btnMenu_Max_clicked();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void UIDemo01::initForm()
{
    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf256), 30);
    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));
    //IconHelper::Instance()->setIcon(ui->label_8, QChar(0xf058));

    //ui->widgetMenu->setVisible(false);
    ui->widgetTitle->installEventFilter(this);
    ui->widgetTitle->setProperty("form", "title");
    ui->widgetTop->setProperty("nav", "top");
    ui->labTitle->setText("多功能蓝牙手套");
    ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
    this->setWindowTitle(ui->labTitle->text());

    ui->stackedWidget->setStyleSheet("QLabel{font:60pt;}");
    ui->widgetTitle->setFixedHeight(100);
    //单独设置指示器大小
    int addWidth = 20;
    int addHeight = 10;
    int rbtnWidth = 15;
    int ckWidth = 13;
    int scrWidth = 12;
    int borderWidth = 3;

    QStringList qss;
    qss.append(QString("QComboBox::drop-down,QDateEdit::drop-down,QTimeEdit::drop-down,QDateTimeEdit::drop-down{width:%1px;}").arg(addWidth));
    qss.append(QString("QComboBox::down-arrow,QDateEdit[calendarPopup=\"true\"]::down-arrow,QTimeEdit[calendarPopup=\"true\"]::down-arrow,"
                       "QDateTimeEdit[calendarPopup=\"true\"]::down-arrow{width:%1px;height:%1px;right:2px;}").arg(addHeight));
    qss.append(QString("QRadioButton::indicator{width:%1px;height:%1px;}").arg(rbtnWidth));
    qss.append(QString("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:%1px;height:%1px;}").arg(ckWidth));
    qss.append(QString("QScrollBar:horizontal{min-height:%1px;border-radius:%2px;}QScrollBar::handle:horizontal{border-radius:%2px;}"
                       "QScrollBar:vertical{min-width:%1px;border-radius:%2px;}QScrollBar::handle:vertical{border-radius:%2px;}").arg(scrWidth).arg(scrWidth / 2));
    qss.append(QString("QWidget#widget_top>QToolButton:pressed,QWidget#widget_top>QToolButton:hover,"
                       "QWidget#widget_top>QToolButton:checked,QWidget#widget_top>QLabel:hover{"
                       "border-width:0px 0px %1px 0px;}").arg(borderWidth));
    qss.append(QString("QWidget#widgetleft>QPushButton:checked,QWidget#widgetleft>QToolButton:checked,"
                       "QWidget#widgetleft>QPushButton:pressed,QWidget#widgetleft>QToolButton:pressed{"
                       "border-width:0px 0px 0px %1px;}").arg(borderWidth));
    this->setStyleSheet(qss.join(""));
    QSize icoSize(32, 32);
    int icoWidth = 85;
    //设置顶部导航按钮
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }
    ui->btnMain->click();
}

void UIDemo01::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        if (btn == b)
        {
            btn->setChecked(true);
        } else
        {
            btn->setChecked(false);
        }
    }
    if (name == "作品展示") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "用户指南") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "手势管理") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "动捕模式")
    {
       serialPort->write("VR\n");
       int response = QMessageBox::question(nullptr, "Question", "<font color='black'>确定开启动捕模式?</font>", QMessageBox::Yes | QMessageBox::No);
       if (response == QMessageBox::Yes)
       {
          QProcess *process = new QProcess(this);
                  // 设置要调用的外部程序路径
          QString program = "E:/dayiziliao/dayixia/softwaredesign/ruanshe/2048进阶版";
          process->start(program);
          if (!process->waitForStarted())
               {  // 启动失败，显示错误信息
                  QString error = process->errorString();
                  QMessageBox::critical(this, "Error", "Failed to start external program: " + error);
               }
                  // 用户选择了"是"
         } else{
                  // 用户选择了"否"
         }
    }
    else if (name == "用户退出")
    {
        int response = QMessageBox::question(nullptr, "Question", "<font color='black'>确定要退出吗?</font>", QMessageBox::Yes | QMessageBox::No);
            if (response == QMessageBox::Yes) {
                    exit(0);
                // 用户选择了"是"
            } else {
                // 用户选择了"否"
            }
    }
}

void UIDemo01::sendjingclose()
{
    serialPort->write("KeyMouse\n");
}

void UIDemo01::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void UIDemo01::on_btnMenu_Max_clicked()
{

}

void UIDemo01::on_btnMenu_Close_clicked()
{
    int response = QMessageBox::question(nullptr, "Question", "<font color='black'>确定要退出吗?</font>", QMessageBox::Yes | QMessageBox::No);
        if (response == QMessageBox::Yes) {
                close();
            // 用户选择了"是"
        } else {
            // 用户选择了"否"
        }
}

void UIDemo01::on_closechuankou_clicked()
{
    serialPort->close();
}

void UIDemo01::on_openchuankou_clicked()
{       /* 串口设置 */
        serialPort->setPortName(ui->comboBox->currentText());
        serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setParity(QSerialPort::NoParity);
        /* 打开串口提示框 */
        if (true == serialPort->open(QIODevice::ReadWrite))
        {
            QString successMessage = "<font color='black'>串口打开成功</font>";
            QMessageBox::information(this, "提示", successMessage);
        }
        else
        {
            QString failureMessage = "<font color='black'>串口打开失败</font>";
            QMessageBox::critical(this, "提示", failureMessage);
        }
}

void UIDemo01::comstate_update()
{      //QLabel *comNote=new QLabel(ui->stackedWidget->widget(2));
       if(serialPort->isOpen())
          {
            ui->label_4->setText("已连接");
            ui->label_4->setStyleSheet("color:green;font-size:21px");
          }
       else if(!serialPort->isOpen())
          {
            ui->label_4->setText("未连接");
            ui->label_4->setStyleSheet("color:red;font-size:21px");
          }
       //ui->gridLayout->addWidget(comNote);
}

void UIDemo01::on_pushButton_clickedggg(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *item0 =ui->treeWidget->topLevelItem(0);
    QTreeWidgetItem *item1 =ui->treeWidget->topLevelItem(1);
    QTreeWidgetItem *item2 =ui->treeWidget->topLevelItem(2);
    QTreeWidgetItem *item3 =ui->treeWidget->topLevelItem(3);

    if(item==item0->child(0))
    {
        ui->stackedWidget_2->setCurrentIndex(0);
        ui->stackedWidget_3->setCurrentIndex(0);}
    else if(item==item0->child(1))
    {
        ui->stackedWidget_2->setCurrentIndex(1);
        ui->stackedWidget_3->setCurrentIndex(1);
    }
    else if(item==item1->child(0))
    {
         ui->stackedWidget_2->setCurrentIndex(2);
         ui->stackedWidget_3->setCurrentIndex(2);
    }
    else if(item==item1->child(1))
    {
         ui->stackedWidget_2->setCurrentIndex(2);
         ui->stackedWidget_3->setCurrentIndex(2);
    }
    else if(item==item2->child(0))
    {
        ui->stackedWidget_2->setCurrentIndex(6);
        ui->stackedWidget_3->setCurrentIndex(6);
    }
    else if(item==item2->child(1))
    {
        ui->stackedWidget_2->setCurrentIndex(4);
        ui->stackedWidget_3->setCurrentIndex(4);
    }
    else if(item==item2->child(2))
    {
        ui->stackedWidget_2->setCurrentIndex(5);
        ui->stackedWidget_3->setCurrentIndex(5);
    }
    else if(item==item2->child(3))
    {
        ui->stackedWidget_2->setCurrentIndex(3);
        ui->stackedWidget_3->setCurrentIndex(3);
    }
    else if(item==item3)
    {
        ui->stackedWidget_2->setCurrentIndex(7);
        ui->stackedWidget_3->setCurrentIndex(7);
    }
    if (item->childCount() > 0)
    {
         if (item->isExpanded())
         {
            ui->treeWidget->collapseItem(item);
         }
         else
         {
            ui->treeWidget->expandItem(item);
         }
     }
}
//静态手势窗口
void UIDemo01::on_pushButton_clicked()
{
    windowone->initial_state();
    countdownValue=5;
    gesturejingTime=3;
    countdownTimer->start(1000);
    windowone->update_count_down(5);
    windowone->update_state(1);
    windowone->setModal(true);
    windowone->show();
    serialPort->write("jing\n");
}
void UIDemo01::on_pushButton_2_clicked()
{
    windowtwo->initial_stated();
    countdownValued=5;
    countdownTimer1->start(1000);
    windowtwo->update_count_downd(5);
    windowtwo->update_stated(1);
    windowtwo->setModal(true);
    windowtwo->show();
    serialPort->write("dong\n");
}
void UIDemo01::updatedongtime()
{

}
void UIDemo01::updateCountdownd()
{
    countdownValued--;
    windowtwo->update_count_downd(countdownValued);
    if(countdownValued==0)
    {
        serialPort->write("dongstart\n");
        windowtwo->update_stated(2);
        countdownTimer1->stop();
    }
}

//录制静态手势倒计时
void UIDemo01::updateCountdown()
{
    countdownValue--;
    windowone->update_count_down(countdownValue);
    if (countdownValue == 0)
    {
        serialPort->write("jingstart\n");
        gestureTimer->start(1000);
        windowone->update_state(2);
        countdownTimer->stop();
    }
}

void UIDemo01::updatejingtime()
{
  gesturejingTime--;
  windowone->update_jing_time(gesturejingTime);
  if(gesturejingTime==0)
  {
      serialPort->write("jingstop\n");
      windowone->update_state(3);
      gestureTimer->stop();
  }
}

void UIDemo01::on_pushButton_3_clicked()
{
    //serialPort->write("ON\n");
    if(!ui->checkBox->isChecked()&&!ui->checkBox_2->isChecked())
        {
            QString successMessage = "<font color='black'>您未选择任何手势</font>";
            QMessageBox::information(this, "提示", successMessage);
        }
    else
    {
      int response = QMessageBox::question(nullptr, "Question", "<font color='black'>确定要删除以上手势吗？</font>", QMessageBox::Yes | QMessageBox::No);
      if (response == QMessageBox::Yes)
         {
          if( ui->checkBox->isChecked())
           {
              ui->label_32->hide();
              ui->label_33->hide();
              ui->checkBox->hide();
              ui->checkBox->setChecked(false);
           }
              QString successMessage = "<font color='black'>删除成功</font>";
              QMessageBox::information(this, "提示", successMessage);  // 用户选择了"是"
          }else
      { // 用户选择了"否"
      }
   }
}

void UIDemo01::on_pushButton_4_clicked()
{
    //serialPort->write("OFF\n");//回收站
    huiShou *hui= new huiShou(this);
    hui->setModal(true);
    hui->show();
}

void UIDemo01::onWindowOneButtonPressed()
{
    QString jingtext=windowone->GetjingText();
    //qDebug()<<jingtext<<"hhh";
    ui->label_46->setText(jingtext);
}

void UIDemo01::onWindowOneButtonPressed1()
{
    QString jingCombo=windowone->GetjingCombo();
    //qDebug()<<jingtext<<"hhh";
    ui->label_47->setText(jingCombo);
}

//ok手势
void UIDemo01::on_comboBox_6_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_6->currentText();
    com6=ui->comboBox_6->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("OK",com6);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
//大拇指
void UIDemo01::on_comboBox_7_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_7->currentText();
    com7=ui->comboBox_7->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("THUMB",com7);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
void UIDemo01::onWindowTwoButtonPressed()
{
    QString dongtext=windowtwo->GetdongText();
    //qDebug()<<jingtext<<"hhh";
    ui->label_48->setText(dongtext);
}
void UIDemo01::onWindowTwoButtonPressed1()
{
    QString dongCombo=windowtwo->GetdongCombo();
    //qDebug()<<jingtext<<"hhh";
    int index=windowtwo->GetdongIndex();
    QString str="gesture1"+QString::number(index)+"\n";
    QByteArray byteArray = str.toUtf8();
    const char* sstr=byteArray.constData();
    serialPort->write(sstr);
    ui->label_49->setText(dongCombo);
}
void UIDemo01::senddongclose()
{

}
//食指顺时针
void UIDemo01::on_comboBox_17_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_17->currentText();
    com17=ui->comboBox_17->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("SHISHUN",com17);
    settings->endGroup();
    QString str="01"+QString::number(com17)+"\n";
    QByteArray byteArray = str.toUtf8();
    const char* sstr=byteArray.constData();
    serialPort->write("jingstop\n");
    serialPort->write(sstr);
    qDebug()<<"changed to"<<text;
}
//食指逆时针
void UIDemo01::on_comboBox_15_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_15->currentText();
    com15=ui->comboBox_15->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("SHINI",com15);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
//食指打勾
void UIDemo01::on_comboBox_9_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_9->currentText();
    com9=ui->comboBox_9->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("SHIGOU",com9);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
//二指顺时针
void UIDemo01::on_comboBox_8_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_8->currentText();
    com8=ui->comboBox_8->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("ERSHUN",com8);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
//二指逆时针
void UIDemo01::on_comboBox_11_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_11->currentText();
    com11=ui->comboBox_11->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("ERNI",com11);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}
//二指打勾
void UIDemo01::on_comboBox_16_currentIndexChanged(int index)
{
    QSettings *settings = new QSettings("/uidemo01/setting.ini",QSettings::IniFormat);
    QString text = ui->comboBox_16->currentText();
    com16=ui->comboBox_16->currentIndex();

    settings->beginGroup("JING");
    settings->setValue("ERGOU",com16);
    settings->endGroup();
    qDebug()<<"changed to"<<text;
}

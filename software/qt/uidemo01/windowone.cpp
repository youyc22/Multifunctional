#include "windowone.h"
#include "ui_windowone.h"
#include "uidemo01.h"

windowOne::windowOne(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowOne)
{
    ui->setupUi(this);
}

windowOne::~windowOne()
{
    delete ui;
}
void windowOne::update_jing_time(int count)
{
    QString labelText = "请在 <font color='red'>"+QString::number(0) + "</font> 秒之后做出手势，并保持<font color='red'>"+
            QString::number(count)+"</font>秒左右";
    ui->label_6->setText(labelText);
}
void windowOne::update_count_down(int countdownValue)
{
    QString labelText = "请在 <font color='red'>"+QString::number(countdownValue) + "</font> 秒之后做出手势，并保持<font color='red'>"+
            QString::number(3)+"</font>秒左右";
    ui->label_6->setText(labelText);
}

void windowOne::update_state(int i)
{
   if(i==1)
   {
     ui->label_5->setText("<font color='red'>录制准备中</font> ");
   }
   else if(i==2)
   {
     ui->label_5->setText("<font color='red'>录制中</font> ");
   }
   else if(i==3)
   {
     ui->label_5->setText("<font color='red'>录制结束</font> ");
   }
}

void windowOne::on_pushButton_2_clicked()
{
    this->close();
    //this->show();
    //UIDemo01::on_pushButton_clicked();
}

void windowOne::on_pushButton_clicked()
{
    emit buttonPressed();
    emit buttonPressed1();
    this->close();
    QString successMessage = "<font color='black'>手势添加成功！</font>";
    QMessageBox::information(this, "提示", successMessage);
}

QString windowOne::GetjingText()
{
    return ui->lineEdit->text();
}

QString windowOne::GetjingCombo()
{
    return ui->comboBox_17->currentText();
}

void windowOne::initial_state()
{
    ui->lineEdit->clear();
    ui->comboBox_17->setCurrentIndex(0);
}

void windowOne::closeEvent(QCloseEvent *event)
{
    emit closewindow();
    event->accept(); // 接受关闭事件，允许窗口关闭
}

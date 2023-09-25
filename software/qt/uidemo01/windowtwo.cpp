#include "windowtwo.h"
#include "ui_windowtwo.h"
#include "uidemo01.h"

windowTwo::windowTwo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowTwo)
{
    ui->setupUi(this);
}

windowTwo::~windowTwo()
{
    delete ui;
}

void windowTwo::update_count_downd(int countdownValue)
{
    QString labelText = "请在 <font color='red'>"+QString::number(countdownValue) + "</font> 秒之后做出手势，以握拳状态作为结束";
    ui->label_6->setText(labelText);
}

void windowTwo::on_pushButton_clicked()
{
    emit buttonPressed();
    emit buttonPressed1();
    this->close();
    QString successMessage = "<font color='black'>手势添加成功！</font>";
    QMessageBox::information(this, "提示", successMessage);
}

void windowTwo::on_pushButton_2_clicked()
{
   this->close();
}

QString windowTwo::GetdongText()
{
    return ui->lineEdit->text();
}

QString windowTwo::GetdongCombo()
{
    return ui->comboBox->currentText();
}

int windowTwo::GetdongIndex()
{
    return ui->comboBox->currentIndex();
}
void windowTwo::update_stated(int i)
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
void windowTwo::initial_stated()
{
    ui->lineEdit->clear();
    ui->comboBox->setCurrentIndex(0);
}

void windowTwo::closeEvent(QCloseEvent *event)
{
    emit closewindow();
    event->accept(); // 接受关闭事件，允许窗口关闭
}

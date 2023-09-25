#include "huishou.h"
#include "ui_huishou.h"

huiShou::huiShou(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::huiShou)
{
    ui->setupUi(this);
}

huiShou::~huiShou()
{
    delete ui;
}

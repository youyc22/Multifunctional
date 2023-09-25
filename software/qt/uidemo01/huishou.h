#ifndef HUISHOU_H
#define HUISHOU_H

#include <QDialog>

namespace Ui {
class huiShou;
}

class huiShou : public QDialog
{
    Q_OBJECT

public:
    explicit huiShou(QWidget *parent = nullptr);
    ~huiShou();

private:
    Ui::huiShou *ui;
};

#endif // HUISHOU_H

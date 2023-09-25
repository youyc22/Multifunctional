#ifndef WINDOWTWO_H
#define WINDOWTWO_H

#include <QDialog>

namespace Ui {
class windowTwo;
}

class windowTwo : public QDialog
{
    Q_OBJECT

public:
    explicit windowTwo(QWidget *parent = nullptr);
    ~windowTwo();
public:
    QString GetdongText();
    QString GetdongCombo();
    int GetdongIndex();
    void update_count_downd(int count);
    void update_stated(int i);
    void initial_stated();
private:
    Ui::windowTwo *ui;

signals:
    void buttonPressed();
    void buttonPressed1();
    void closewindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void closeEvent(QCloseEvent *event);

};

#endif // WINDOWTWO_H

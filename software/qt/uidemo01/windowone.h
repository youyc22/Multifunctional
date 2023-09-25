#ifndef WINDOWONE_H
#define WINDOWONE_H

#include <QDialog>

namespace Ui {
class windowOne;

}
class windowOne : public QDialog
{
    Q_OBJECT

public:
    explicit windowOne(QWidget *parent = nullptr);
    ~windowOne();
public:
    QString GetjingText();
    QString GetjingCombo();
    void update_count_down(int count);
    void update_jing_time(int count);
    void update_state(int i);
    void initial_state();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::windowOne *ui;
signals:
    void buttonPressed();
    void buttonPressed1();
    void closewindow();
};

#endif // WINDOWONE_H

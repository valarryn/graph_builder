#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPushButton>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton * button;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void myClicked();

};

#endif // MAINWINDOW_H

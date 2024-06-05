#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "poland.h"
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    button = new QPushButton(this);
    button->setGeometry(width()-5-100, 295, 100, 30);
    button->setStyleSheet("color: green");
    button->setText("Build graph");
    connect(button, SIGNAL(clicked()), this, SLOT(myClicked()));

    ui->lineEdit->setGeometry(width()-5-100, 25, 100, 30);
    ui->label_2->setGeometry(width()-5-100, 0, 100, 30);
    ui->label_2->setStyleSheet("Segoe UI: 20pt");
    ui->label_2->setText("f(x)");

    ui->label_3->setGeometry(width()-5-100, 60, 100, 30);
    ui->label_3->setStyleSheet("Segoe UI: 20pt");
    ui->label_3->setText("x minimum");
    ui->lineEdit_2->setGeometry(width()-5-100, 85, 100, 30);

    ui->label_4->setGeometry(width()-5-100, 110, 100, 30);
    ui->label_4->setStyleSheet("Segoe UI: 20pt");
    ui->label_4->setText("y minimum");
    ui->lineEdit_3->setGeometry(width()-5-100, 135, 100, 30);

    ui->label_5->setGeometry(width()-5-100, 170, 100, 30);
    ui->label_5->setStyleSheet("Segoe UI: 20pt");
    ui->label_5->setText("x maximum");
    ui->lineEdit_4->setGeometry(width()-5-100, 195, 100, 30);

    ui->label_6->setGeometry(width()-5-100, 220, 100, 30);
    ui->label_6->setStyleSheet("Segoe UI: 20pt");
    ui->label_6->setText("y maximum");
    ui->lineEdit_5->setGeometry(width()-5-100, 245, 100, 30);
}

void MainWindow::myClicked()
{
    ui->label->setGeometry(5,5,width()-5-5-5-button->width(), height()-5-5);
    QPixmap pm(width()-5-5-5-button->width(), height()-5-5);
    pm.fill(Qt::black);

    QString xmi = ui->lineEdit_2->text(), xma = ui->lineEdit_4->text();
    QString ymi = ui->lineEdit_3->text(), yma = ui->lineEdit_5->text();

    double xmin, xmax, ymin, ymax;
    if (xmi == "") xmin = -10;
    else xmin = xmi.toDouble();
    if (xma == "") xmax = 10;
    else xmax = xma.toDouble();
    if (ymi == "") ymin = -10;
    else ymin = ymi.toDouble();
    if (yma == "") ymax = 10;
    else ymax = yma.toDouble();


    int xgmin = 0;
    int xgmax = pm.width();
    int ygmin = 0;
    int ygmax = pm.height();

    double kx = (xgmax-xgmin)/(xmax-xmin);
    double ky = (ygmin-ygmax)/(ymax-ymin);

    double step = 0.01*((xmax-xmin)/(xgmax-xgmin));

    QPainterPath path;
    QPainter painter;

    painter.begin(&pm);

    long double ans = 0;
    double x = xmin; double y = 0;
    int def = 0; //is a dot defined or not
    QString s; string s_std;

    s = ui->lineEdit->text();
    for (int j = 0; j < s.length(); j++){
        s.replace('x', '(' + QString::number(x) + ')');
    }
    s_std = s.toStdString();
    if (calculate(s_std, ans)){
        y = ans;
        double xg = xgmin + kx*(x-xmin);
        double yg = ygmin + ky*(y-ymax);
        path.moveTo(xg, yg);
    }

    for (double x = xmin; x < xmax; x += step){
        s = ui->lineEdit->text();
        for (int j = 0; j < s.size(); j++){
            s.replace('x', '(' + QString::number(x) + ')');
        }
        s_std = s.toStdString();
        calculate(s_std, ans);
        y = ans;
        double xg = xgmin + kx*(x-xmin);
        double yg = ygmin + ky*(y-ymax);
        if (calculate(s_std, ans) && (ymin <= y && y <= ymax)) {
            if (def == 0) path.lineTo(xg, yg);
            if (def == 1) {
                path.moveTo(xg, yg);
                def = 0;
            }
        }
        else def = 1;
    }

    painter.setPen(QPen(QColor(41, 41, 41), 1, Qt::SolidLine));
    //x
    int j = ymax;
    for (double i = 0; i <= ygmax; i+= ygmax/(abs(ymax) + abs(ymin))){
        painter.drawLine(xgmin, i, xgmax, i);
        if (j != 0) painter.drawText(xgmin - kx * xmin + 5, i - 2, QString::number(j));
        j--;
    }
    //y
    j = xmin;
    for (double i = 0; i <= xgmax; i+= xgmax/(abs(xmax) + abs(xmin))){
        painter.drawLine(i, ygmin, i, ygmax);
        painter.drawText(i + 5, ygmin - ky * ymax - 5, QString::number(j));
        j++;
    }
    painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));

    painter.drawLine(xgmin - kx * xmin, ygmin, xgmin - kx * xmin, ygmax);
    //arrows
    painter.drawLine(xgmin - kx * xmin, ygmin, xgmin - kx * xmin - 7, ygmin+7);
    painter.drawLine(xgmin - kx * xmin, ygmin, xgmin - kx * xmin + 7, ygmin + 7);

    painter.drawLine(xgmin, ygmin - ky * ymax, xgmax, ygmin - ky * ymax);
    //arrows
    painter.drawLine(xgmax - 7, ygmin - ky * ymax - 7, xgmax, ygmin - ky * ymax);
    painter.drawLine(xgmax - 7, ygmin - ky * ymax + 7, xgmax, ygmin - ky * ymax);

    painter.drawText(xgmin - kx * xmin + 10, 15, "X");
    painter.drawText(xgmax - 15, ygmin - ky * ymax - 7, "Y");

    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    painter.drawPath(path);

    ui->label->setPixmap(pm);
}
MainWindow::~MainWindow()
{
    delete ui;
}



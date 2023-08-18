#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QThread>
#include <QSqlError>
#include <QtConcurrent>
#include <QSqlRecord>
#include <QBarSeries>
#include <QObject>
#include <QLineSeries>
#include <QtCharts>
#include <QChartView>
#include "database.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString airportCurrentName;
    QStringList categories;

    int iMin = 0;
    int iMax = 31;

    int dataInGraph[365];
    int dataOutGraph[365];

public slots:
    void airportCurrentNameSetup(QString name);
    void yearGraph(int dataIn[12], int dataOut[12]);
    void monthGraph(int dataIn[365], int dataOut[365]);

private slots:
    void on_pb_closeDialog_clicked();

    void on_cb_month_currentIndexChanged(int index);

signals:
    void sig_sendAirportNameToGraph(QString airport);

private:
    Ui::Dialog *ui;
    DataBase* dataBaseGraph;
    Dialog* dialog;
    QChart* chart;
    QChartView* chartView;
    QGridLayout *layout;
    QChart* chart2;
    QChartView* chartView2;
    QGridLayout *layout2;


    QBarSeries *series;
    QBarSet *set0;
    QBarSet *set1;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
};

#endif // DIALOG_H

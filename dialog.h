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

    int iMin = 122;
    int iMax = 31;

    int dataInGraph[365];
    int dataOutGraph[365];

public slots:
    void airportCurrentNameSetup(QString name);
    void yearGraph(int dataIn[12], int dataOut[12]);
    void monthGraph(int dataIn[365], int dataOut[365]);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pb_closeDialog_clicked();

    void on_cb_month_currentIndexChanged(int index);

signals:
    void sig_sendAirportNameToGraph(QString airport);
    void sig_sendClose();

private:
    Ui::Dialog *ui;
    DataBase* dataBaseGraph;
    Dialog* dialog;
    QChart* chartYear;
    QChartView* chartView;
    QGridLayout *layout;
    QChart* chartMonth;
    QChartView* chartViewMonth;
    QGridLayout *layoutMonth;
    QLineSeries *seriesMonthIn;
    QLineSeries *seriesMonthOut;


    QBarSeries *series;
    QBarSet *setIn;
    QBarSet *setOut;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
};

#endif // DIALOG_H

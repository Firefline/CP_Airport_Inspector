#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QThread>
#include <QSqlError>
#include <QtConcurrent>
#include "database.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ScreenDataFromDB(QSqlQueryModel *widget);
    void statusConnection(bool status);

signals:
    void sig_startConnection();
    void sig_sendAirportName(QString name);
    void sig_sendYearData(int dataIn[12], int dataOut[12]);
    void sig_sendMonthData(int dataIn[365], int dataOut[365]);

private slots:
    void on_pb_receiveData_clicked();

    void on_rb_arrival_clicked();

    void on_rb_departure_clicked();

    void on_cb_airport_currentIndexChanged(int index);

    void on_de_date_userDateChanged(const QDate &date);

    void on_pb_reseiveLoad_clicked();

    void on_cb_dateSelect_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    DataBase* dataBase;
    Dialog* dialog;

    QString airportCode;
    QString dateFilter;

    bool dateSelect = false;

    QString requestWhereSummary;

    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                      "WHERE ad.airport_code = " + airportCode;
};
#endif // MAINWINDOW_H

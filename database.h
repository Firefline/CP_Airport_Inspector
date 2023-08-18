#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void startConnection();
    void AddDataBase(QString driver, QString nameDB);
    void RequestToDB(QString request);
    void receiveAirportList();
    void receiveYearData(QString airport);
    void receiveMonthData(QString airport);

    QStringList airports;
    QStringList airportCodes;
    bool airportIn = true;

    int yearDataIn[12];
    int yearDataOut[12];
    int monthDataIn[365];
    int monthDataOut[365];

signals:
    void sig_connectionStatus(bool);
    void sig_SendDataFromDB(QSqlQueryModel * model, QString typeR);

private:

    QSqlDatabase* dataBase;
    QSqlQueryModel* simpleQuery;
    QTableView* tableWinget;
    QSqlQueryModel* simpleTable;
    QSqlQueryModel* simpleTableGraph;
};

#endif // DATABASE_H

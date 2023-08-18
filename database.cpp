#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();
    simpleQuery = new QSqlQueryModel();
    simpleTable = new QSqlQueryModel();
    simpleTableGraph = new QSqlQueryModel();
    tableWinget = new QTableView();

}

DataBase::~DataBase()
{
    delete dataBase;
    delete simpleQuery;
    delete tableWinget;
    delete simpleTable;
    delete simpleTableGraph;
}

void DataBase::startConnection()
{
    dataBase->setDatabaseName("demo");
    dataBase->setHostName("981757-ca08998.tmweb.ru");
    dataBase->setUserName("netology_usr_cpp");
    dataBase->setPassword("CppNeto3");
    dataBase->setPort(5432);


    bool status;
    status = dataBase->open();
    emit sig_connectionStatus(status);
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::RequestToDB(QString request)
{
    simpleQuery->setQuery (request, *dataBase);
    if (simpleQuery->lastError().isValid())
    {
        qDebug() << simpleQuery->lastError();
    }
    simpleQuery->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    simpleQuery->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    if (airportIn == true)
    {
        simpleQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
    } else if (airportIn == false)
    {
        simpleQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
    }

    emit sig_SendDataFromDB(simpleQuery, request);
}

void DataBase::receiveAirportList()
{
    simpleTable->setQuery ("SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data ", *dataBase);

    for (int i = 0; i < simpleTable->rowCount(); ++i)
    {
        airports.append(simpleTable->record(i).value(0).toString());
        airportCodes.append(simpleTable->record(i).value(1).toString());
    }
}

void DataBase::receiveYearData(QString airport)
{
    simpleTableGraph->setQuery("SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" FROM bookings.flights f "
                               "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                               "AND departure_airport = " + airport + " GROUP BY \"Month\" ", *dataBase);
    for (int i = 0; i < 12; ++i)
    {
        yearDataOut[i] = simpleTableGraph->record(i).value(0).toInt();
    }
    simpleTableGraph->setQuery("SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" FROM bookings.flights f "
                               "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                               "AND arrival_airport = " + airport + " GROUP BY \"Month\" ", *dataBase);
    for (int i = 0; i < 12; ++i)
    {
        yearDataIn[i] = simpleTableGraph->record(i).value(0).toInt();
    }
}

void DataBase::receiveMonthData(QString airport)
{
    simpleTableGraph->setQuery("SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" FROM bookings.flights f "
                               "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                               "AND departure_airport = " + airport + " GROUP BY \"Day\" ", *dataBase);
    for (int i = 0; i < 365; ++i)
    {
        monthDataOut[i] = simpleTableGraph->record(i).value(0).toInt();
    }
    simpleTableGraph->setQuery("SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" FROM bookings.flights f "
                               "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                               "AND arrival_airport = " + airport + " GROUP BY \"Day\" ", *dataBase);
    for (int i = 0; i < 365; ++i)
    {
        monthDataIn[i] = simpleTableGraph->record(i).value(0).toInt();
    }
}

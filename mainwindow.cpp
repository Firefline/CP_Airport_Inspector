#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataBase = new DataBase(this);
    dataBase -> AddDataBase("QPSQL", "demo");

    ui->lb_statusConnection->setStyleSheet("color:red");
    ui->rb_arrival->setChecked(true);

    ui->de_date->setDateRange(QDate(2016,8,15), QDate(2017,9,14));

    ui->cb_airport->setDisabled(true);
    ui->rb_arrival->setDisabled(true);
    ui->rb_departure->setDisabled(true);
    ui->cb_dateSelect->setDisabled(true);
    ui->de_date->setDisabled(true);
    ui->pb_receiveData->setDisabled(true);
    ui->tb_result->setDisabled(true);
    ui->pb_reseiveLoad->setDisabled(true);

    connect(this, &MainWindow::sig_startConnection, dataBase, &DataBase::startConnection);
    connect(dataBase, &DataBase::sig_connectionStatus, this, &MainWindow::statusConnection);
    emit sig_startConnection();

    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    dataBase->receiveAirportList();
    ui->cb_airport->addItems(dataBase->airports);
    request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f "
              "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
              "WHERE ad.airport_code  = " + airportCode;

    dialog = new Dialog(this);
    connect(this, &MainWindow::sig_sendAirportName, dialog, &Dialog::airportCurrentNameSetup);
    connect(this, &MainWindow::sig_sendYearData, dialog, &Dialog::yearGraph);
    connect(this, &MainWindow::sig_sendMonthData, dialog, &Dialog::monthGraph);
    connect(dialog, &Dialog::sig_sendClose, this, &MainWindow::unlockMain);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dataBase;
    delete dialog;
}

void MainWindow::statusConnection(bool status)
{
    if (status == true)
        {
            ui->lb_statusConnection->setText("Подключено");
            ui->lb_statusConnection->setStyleSheet("color:green");

            ui->cb_airport->setDisabled(false);
            ui->rb_arrival->setDisabled(false);
            ui->rb_departure->setDisabled(false);
            ui->cb_dateSelect->setDisabled(false);
            ui->pb_receiveData->setDisabled(false);
            ui->tb_result->setDisabled(false);
            ui->pb_reseiveLoad->setDisabled(false);
        }
        else
        {
            QMessageBox::information(this, "Соединение с БД", "Ошибка подключения"); //dataBase.lastError().text()
            QThread::sleep(5);
            emit sig_startConnection();
        };
}

void MainWindow::on_rb_arrival_clicked()
{
    dataBase->airportIn = true;
}

void MainWindow::on_rb_departure_clicked()
{
    dataBase->airportIn = false;
}

void MainWindow::on_cb_airport_currentIndexChanged(int index)
{
    airportCode = "'" + dataBase->airportCodes.value(index) + "'";
}

void MainWindow::on_de_date_userDateChanged(const QDate &date)
{
    dateFilter = "'" + date.toString("yyyy-MM-dd") + "'";
}

void MainWindow::ScreenDataFromDB(QSqlQueryModel *widget)
{
    ui->tb_result->setModel(widget);
    ui->tb_result->show();
}

void MainWindow::on_pb_receiveData_clicked()
{
    if(dataBase->airportIn == true)
    {
        if (dateSelect == false)
        {
            requestWhereSummary = "WHERE ad.airport_code = " + airportCode;
        }
        else if (dateSelect == true)
        {
            requestWhereSummary = "WHERE ad.airport_code = " + airportCode + " AND scheduled_arrival::date = date(" + dateFilter + ") ";
        }

        request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                + requestWhereSummary;
    }

    if(dataBase->airportIn == false)
    {
        if (dateSelect == false)
        {
            requestWhereSummary = "WHERE ad.airport_code = " + airportCode;
        }
        else if (dateSelect == true)
        {
            requestWhereSummary = "WHERE ad.airport_code = " + airportCode + " AND scheduled_departure::date = date(" + dateFilter + ") ";
        }

        request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                + requestWhereSummary;
    }

    auto req = [&]{dataBase->RequestToDB(request);};
    QtConcurrent::run(req);
}

void MainWindow::on_pb_reseiveLoad_clicked()
{
    emit sig_sendAirportName(ui->cb_airport->currentText());
    dataBase->receiveYearData(airportCode);
    emit sig_sendYearData(dataBase->yearDataIn, dataBase->yearDataOut);
    dataBase->receiveMonthData(airportCode);
    emit sig_sendMonthData(dataBase->monthDataIn, dataBase->monthDataOut);
    dialog->show();
    ui->cb_airport->setDisabled(true);
    ui->rb_arrival->setDisabled(true);
    ui->rb_departure->setDisabled(true);
    ui->cb_dateSelect->setDisabled(true);
    ui->pb_receiveData->setDisabled(true);
    ui->tb_result->setDisabled(true);
    ui->pb_reseiveLoad->setDisabled(true);
}


void MainWindow::on_cb_dateSelect_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        dateSelect = false;
        ui->de_date->setDisabled(true);
    } else if (arg1 == 2)
    {
        dateSelect = true;
        ui->de_date->setDisabled(false);
    }
}

void MainWindow::unlockMain()
{
    ui->cb_airport->setDisabled(false);
    ui->rb_arrival->setDisabled(false);
    ui->rb_departure->setDisabled(false);
    ui->cb_dateSelect->setDisabled(false);
    ui->pb_receiveData->setDisabled(false);
    ui->tb_result->setDisabled(false);
    ui->pb_reseiveLoad->setDisabled(false);
}

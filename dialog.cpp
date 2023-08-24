#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    chartYear = new QChart( );
    chartYear->legend()->setVisible(false);
    chartView = new QChartView(chartYear);
    layout = new QGridLayout;
    setIn = new QBarSet("Прибытие");
    setOut = new QBarSet("Отправление");

    chartMonth = new QChart( );
    chartMonth->legend()->setVisible(false);
    chartViewMonth = new QChartView(chartMonth);
    layoutMonth = new QGridLayout;
    seriesMonthIn = new QLineSeries;
    seriesMonthOut = new QLineSeries;

    series = new QBarSeries;
    chartYear->setTitle("Загруженность аэропорта за год");
    categories << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    axisX = new QBarCategoryAxis(this);
    axisX->append(categories);
    chartYear->addSeries(series);
    chartYear->addAxis(axisX, Qt::AlignBottom);
    axisY = new QValueAxis(this);
    chartYear->addAxis(axisY, Qt::AlignLeft);

    ui->tabWidget->setTabText(0, "Загруженность за год");
    ui->tabWidget->setTabText(1, "Загруженность за выбранный месяц");
    ui->tabWidget->setCurrentIndex(0);
    ui->le_airportCurrent->setEnabled(false);
    ui->cb_month->addItems({"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"});
}

Dialog::~Dialog()
{
    chartYear->removeSeries(series);
    series->remove(setIn);
    series->remove(setOut);
    chartMonth->removeSeries(seriesMonthIn);
    chartMonth->removeSeries(seriesMonthOut);

    delete ui;
    delete chartYear;
    delete chartView;
    delete series;
    //delete axisX;
    //delete axisY;
    //delete setIn;
    //delete setOut;
    delete chartMonth;
    delete chartViewMonth;
    delete layoutMonth;
    delete seriesMonthIn;
    delete seriesMonthOut;

}

void Dialog::on_pb_closeDialog_clicked()
{
    emit sig_sendClose();
    this->close();
}

void Dialog::airportCurrentNameSetup(QString name)
{
   ui->le_airportCurrent->setText(name);
   airportCurrentName = name;
}

void Dialog::yearGraph(int dataIn[12], int dataOut[12])
{
    if (chartYear->isEmpty() == false)
    {
        chartYear->removeSeries(series);
        series->remove(setIn);
        series->remove(setOut);
    }
    series->clear();
    chartYear->removeAxis(axisY);
    setIn = new QBarSet("Прибытие");
    setOut = new QBarSet("Отправление");
    axisY = new QValueAxis(axisY);
    for (int i = 0; i < 12; ++i)
    {
        *setIn << dataIn[i];
    }
    for (int i = 0; i < 12; ++i)
    {
        *setOut << dataOut[i];
    }
    series->append(setIn);
    series->append(setOut);
    chartYear->addSeries(series);
    series->setLabelsVisible(true);
    chartYear->addAxis(axisY, Qt::AlignLeft);
    chartYear-> setAnimationOptions (QChart :: AllAnimations);
    series->attachAxis(axisY);
    chartYear->legend()->setVisible(true);
    chartYear->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->wd_yearGraph->setLayout(layout);
    layout->addWidget(chartView);
    chartView->show( );
}

void Dialog::monthGraph(int dataIn[365], int dataOut[365])
{
    for (int i = 0; i < 365; ++i)
    {
        dataInGraph[i] = dataIn[i];
        dataOutGraph[i] = dataOut[i];
    }
    if (chartMonth->series().isEmpty() == false )
    {
        chartMonth->removeSeries(seriesMonthIn);
        chartMonth->removeSeries(seriesMonthOut);
    }
    seriesMonthIn->clear();
    seriesMonthOut->clear();
    for (int i = 0; i < iMax; ++i)
    {
        seriesMonthIn->append(i+1, dataIn[iMin + i]);
        seriesMonthOut->append(i+1, dataOut[iMin + i]);
    }
    seriesMonthIn->setName("Прибытие");
    seriesMonthOut->setName("Отправление");
    chartMonth->addSeries(seriesMonthIn);
    chartMonth->addSeries(seriesMonthOut);
    chartMonth->createDefaultAxes();
    chartMonth->setTitle("Загруженность за выбранный месяц");
    chartMonth->legend()->setVisible(true);
    ui->wd_monthGraph->setLayout(layoutMonth);
    layoutMonth->addWidget(chartViewMonth);
    chartViewMonth->show();
}

void Dialog::on_cb_month_currentIndexChanged(int index)
{
    if (index == 8)
    {
        iMin = 0;
        iMax = 30;
    }
    if (index == 9)
    {
        iMin = 30;
        iMax = 31;
    }
    if (index == 10)
    {
        iMin = 61;
        iMax = 30;
    }
    if (index == 11)
    {
        iMin = 91;
        iMax = 31;
    }
    if (index == 0)
    {
        iMin = 122;
        iMax = 31;
    }
    if (index == 1)
    {
        iMin = 153;
        iMax = 28;
    }
    if (index == 2)
    {
        iMin = 188;
        iMax = 31;
    }
    if (index == 3)
    {
        iMin = 212;
        iMax = 30;
    }
    if (index == 4)
    {
        iMin = 242;
        iMax = 31;
    }
    if (index == 5)
    {
        iMin = 273;
        iMax = 30;
    }
    if (index == 6)
    {
        iMin = 303;
        iMax = 31;
    }
    if (index == 7)
    {
        iMin = 334;
        iMax = 31;
    }
    monthGraph(dataInGraph, dataOutGraph);
}

void Dialog::closeEvent(QCloseEvent *event)
{
    emit sig_sendClose();
    this->close();
}

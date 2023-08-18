#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    chart = new QChart( );
    chart->legend()->setVisible(false);
    chartView = new QChartView(chart);
    layout = new QGridLayout;

    chart2 = new QChart( );
    chart2->legend()->setVisible(false);
    chartView2 = new QChartView(chart2);
    layout2 = new QGridLayout;




    series = new QBarSeries;
    chart->setTitle("Загруженность аэропорта за год");
    categories << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);




    ui->tabWidget->setTabText(0, "Загруженность за год");
    ui->tabWidget->setTabText(1, "Загруженность за выбранный месяц");
    ui->tabWidget->setCurrentIndex(0);
    ui->le_airportCurrent->setEnabled(false);
    ui->cb_month->addItems({"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"});
}

Dialog::~Dialog()
{
    delete ui;
    delete chart;
    delete chartView;
    delete chart2;
    delete chartView2;
    delete set0;
    delete set1;
}

void Dialog::on_pb_closeDialog_clicked()
{
    this->close();
}

void Dialog::airportCurrentNameSetup(QString name)
{
   ui->le_airportCurrent->setText(name);
   airportCurrentName = name;
}

void Dialog::yearGraph(int dataIn[12], int dataOut[12])
{
    set0 = new QBarSet("Прибытие");
    set1 = new QBarSet("Отправление");
    chart->removeSeries(series);
    chart->removeAxis(axisY);
    axisY = new QValueAxis();
    for (int i = 0; i < 12; ++i)
    {
        *set0 << dataIn[i];
    }
    for (int i = 0; i < 12; ++i)
    {
        *set1 << dataOut[i];
    }
    series->append(set0);
    series->append(set1);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addSeries(series);
    chart-> setAnimationOptions (QChart :: AllAnimations);

    series->attachAxis(axisY);







    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->wd_yearGraph->setLayout(layout);
    layout->addWidget(chartView);
    chartView->show( );
}

void Dialog::monthGraph(int dataIn[365], int dataOut[365])
{
    QLineSeries *series2 = new QLineSeries;
    QLineSeries *series3 = new QLineSeries;
    for (int i = iMin; i < iMax; ++i)
    {
        //series2->append(i+1, dataIn[i]);
        dataInGraph[i] = dataIn[i];
        series2->append(i+1, dataInGraph[i]);
    }
    for (int i = iMin; i < iMax; ++i)
    {
        //series2->append(i+1, dataOut[i]);
        dataOutGraph[i] = dataOut[i];
        series3->append(i+1, dataOutGraph[i]);
    }
    chart2->addSeries(series2);
    chart2->addSeries(series3);
    chart2->createDefaultAxes();
    chart2->setTitle("Загруженность за выбранный месяц");
    ui->wd_monthGraph->setLayout(layout2);
    layout2->addWidget(chartView2);
    chartView2->show();
}

void Dialog::on_cb_month_currentIndexChanged(int index)
{
    if (index == 0)
    {
        iMin = 0;
        iMax = 31;
    }
    if (index == 1)
    {
        iMin = 31;
        iMax = 59;
    }
    if (index == 2)
    {
        iMin = 59;
        iMax = 90;
    }
    if (index == 3)
    {
        iMin = 90;
        iMax = 120;
    }
    if (index == 4)
    {
        iMin = 120;
        iMax = 151;
    }
    if (index == 5)
    {
        iMin = 151;
        iMax = 181;
    }
    if (index == 6)
    {
        iMin = 181;
        iMax = 212;
    }
    if (index == 7)
    {
        iMin = 212;
        iMax = 243;
    }
    if (index == 8)
    {
        iMin = 243;
        iMax = 273;
    }
    if (index == 9)
    {
        iMin = 273;
        iMax = 304;
    }
    if (index == 10)
    {
        iMin = 304;
        iMax = 334;
    }
    if (index == 11)
    {
        iMin = 334;
        iMax = 365;
    }
    monthGraph(dataInGraph, dataOutGraph);
}


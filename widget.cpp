#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QLineSeries>
#include <QtCharts>
#include <vector>
#include <algorithm>


int Widget::sum_day = 0;
Day Widget::myday[366];
QSqlDatabase Widget::database;
using namespace QtCharts;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(show_gra()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(show_data()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(input()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(clear()));
    //创建数据库
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Claire.db");
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {

        QString create_sql = "create table claire (id int primary key, date varchar(30), morning varchar(30), night varchar(30))";
        QSqlQuery sql_query;
        if(!sql_query.exec(create_sql))
        {
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

        model = new QSqlTableModel(this, database);
        model->setTable("claire");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        model->setHeaderData(0, Qt::Horizontal, tr("id"));
        model->setHeaderData(1, Qt::Horizontal, tr("日期"));
        model->setHeaderData(2, Qt::Horizontal, tr("清晨体重"));
        model->setHeaderData(3, Qt::Horizontal, tr("晚饭后体重"));



    }
    ui->label->setStyleSheet("color:#0080ff;");
    ui->label_2->setStyleSheet("color:#1f9baa;");
    ui->label_3->setStyleSheet("color:#3366cc;");
    ui->pushButton->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_2->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                "QPushButton:hover{background-color:white; color: black;}"
                                "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_3->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_4->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->label_8->setPixmap(QPixmap(":/image/1.ico"));
}

Widget::~Widget()
{
    delete model;
    delete ui;
}

void Widget::show_gra()
{
    QPen p1, p2;
    p1.setWidth(3);
    p1.setColor(Qt::red);
    p2.setWidth(3);
    p2.setColor(Qt::blue);
    QLineSeries *series_1 = new QLineSeries();
    QLineSeries *series_2 = new QLineSeries();
    QScatterSeries *series_3 = new QScatterSeries();
    QScatterSeries *series_4 = new QScatterSeries();
    QChart * chart = new QChart();
    chart->addSeries(series_1);
    chart->addSeries(series_2);
    chart->addSeries(series_3);
    chart->addSeries(series_4);
    series_1->setPen(p1);
    series_2->setPen(p2);
    series_3->setPen(p1);
    series_4->setPen(p2);
    series_3->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series_3->setMarkerSize(10);
    series_4->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series_4->setMarkerSize(10);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle(tr("红线为清晨体重，蓝线为晚饭后体重"));
    std::vector<double> morning(366);
    std::vector<double> night(366);
    QSqlQuery sql_query;
    QString select_sql = "SELECT morning, night FROM claire";
    int j = 0;
    sql_query.exec(select_sql);
    while (sql_query.next())
    {

        morning[j] = sql_query.value(0).toDouble();
        night[j] = sql_query.value(1).toDouble();
        ++j;
    }
    for (int k = 0; k < j; ++k)
    {
        series_1->append(k + 1, morning[k]);
        series_2->append(k + 1, night[k]);
        series_3->append(k + 1, morning[k]);
        series_4->append(k + 1, night[k]);

    }
    QValueAxis *axisX = new QValueAxis; //定义X轴
    axisX->setRange(0 , j + 1); //设置范围
    axisX->setLabelFormat("%g"); //设置刻度的格式//设置X轴的标题
    axisX->setGridLineVisible(true); //设置是否显示网格线
    axisX->setTickCount(j + 2);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(90, 140);
    axisY->setTitleText("体重/斤");
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(6);
    axisY->setMinorTickCount(10); //设置小刻度线的数目
    axisY->setGridLineVisible(true);
    chart->setAxisX(axisX, series_1);
    chart->setAxisY(axisY, series_1);
    chart->setAxisX(axisX, series_2);
    chart->setAxisY(axisY, series_2);
    chart->setAxisX(axisX, series_3);
    chart->setAxisY(axisY, series_3);
    chart->setAxisX(axisX, series_4);
    chart->setAxisY(axisY, series_4);
    chart->legend()->hide();

    /*
    QScatterSeries* scatseries_1 = new QScatterSeries;
    scatseries_1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatseries_1->setMarkerSize(10);
    scatseries_1->setPen(p1);
    QScatterSeries* scatseries_2 = new QScatterSeries;
    scatseries_2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatseries_2->setMarkerSize(10);
    scatseries_2->setPen(p2);
    */

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(1200, 800);
    view->show();

}

void Widget::show_data()
{

    ui->tableView->setModel(model);
    for (int j = 1; j < 4; ++j)
        ui->tableView->setColumnWidth(j, 315);
    ui->tableView->hideColumn(0);
    ui->tableView->verticalHeader()->setVisible(false);

}

void Widget::input()
{
    myday[sum_day].l1 = ui->lineEdit;
    myday[sum_day].l2 = ui->lineEdit_2;
    myday[sum_day].l3 = ui->lineEdit_3;
    myday[sum_day].l4 = ui->lineEdit_4;
    myday[sum_day].get_info();
    int rowCount = model->rowCount();
    model->insertRow(rowCount);
    model->setData(model->index(rowCount,0), rowCount);
    model->setData(model->index(rowCount,1), myday[sum_day].get_date());
    model->setData(model->index(rowCount,2), myday[sum_day].get_morning());
    model->setData(model->index(rowCount,3), myday[sum_day].get_night());
    //提交到数据库
    model->submitAll();
    qDebug() << "日期： " << myday[sum_day].get_date() << endl
                 << "清晨： " << myday[sum_day].get_morning() << " kg" << endl
                 << "晚饭后： " << myday[sum_day].get_night() << " kg" << endl
                 << sum_day << endl;
    ++sum_day;

}

void Widget::clear()
{
    if(sum_day > 0)
        --sum_day;
    //获取选中行
    int curRow = ui->tableView->currentIndex().row();
    //删除该行
    model->removeRow(curRow);

    int ok = QMessageBox::warning(this,QStringLiteral("delete"),
                                  QStringLiteral("是否删除当前行？"),
                                  QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
        model->revertAll();
    else
        model->submitAll();
}


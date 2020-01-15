#include "mainwindow.h"
#include "ui_mainwindow.h"

//QSqlQuery sql_query;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    creatDatabase();
    on_btn_update_clicked();
    connect(this,SIGNAL(dialog_singnal(QSqlQuery,QString)),&dWin,SLOT(dialog_slot(QSqlQuery,QString)));    /*连接按钮和对话框*/
    connect(&dWin,SIGNAL(update_singnal()),this,SLOT(on_btn_update_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creatDatabase(void)
{
    bool res;

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("user.db");

    if(!mydb.open())
    {
        ui->statusbar->showMessage(mydb.lastError().text());
    }
    else
    {
        ui->statusbar->showMessage("数据库打开成功！");

        sql_query = QSqlQuery(mydb);
        res = sql_query.exec("create table if not exists student(name text,num int unique,score float)");
        if(res == 0)
        {
            ui->statusbar->showMessage("数据表打开失败！");
        }
        else
        {
//            sql_query.exec("create index if not exists name_index on student(name)");   //创建索引
//            sql_query.exec("create index if not exists num_index on student(num)");   //创建索引
            /*数据量小，无需创建索引，直接查就可以*/
            ui->statusbar->showMessage("数据表打开成功！");
        }
    }
}

void MainWindow::on_btn_update_clicked()
{
    bool res;
    int index = 0;
    QString name;
    uint num;
    double score;

    res = sql_query.exec("select * from student");
    if(!res)
    {
        ui->statusbar->showMessage("更新失败！");
    }
    else
    {
        while(sql_query.next())
        {
            name = sql_query.value(0).toString();
            num = sql_query.value(1).toUInt();
            score = sql_query.value(2).toDouble();

            ui->tableWidget->setRowCount(index+1);

            ui->tableWidget->setItem(index,0,new QTableWidgetItem(name));
            ui->tableWidget->setItem(index,1,new QTableWidgetItem(QString::number(num)));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString("%1").arg(score)));

            index++;
        }
        ui->statusbar->showMessage("更新成功！");
    }
}

void MainWindow::on_btn_add_clicked()
{
    dialog_singnal(sql_query,"0");   //调用函数，触发信号，传递数据表
}

void MainWindow::on_btn_change_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();

    if(!items.empty())
    {
        QTableWidgetItem *item = items.at(1);
        from = item->text();
        dialog_singnal(sql_query,from);   //调用函数，触发信号，传递数据表，form代表所选行的学号，学号唯一用于区分
    }
}

void MainWindow::on_btn_del_clicked()
{
    int curRow;
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();

    if(!items.empty())
    {
        QTableWidgetItem *item = items.at(1);
        from = item->text();
        sql_query.exec("delete from student where num="+from);

        curRow = ui->tableWidget->currentRow();
        if(curRow > 0)  //当前不是第0条
        {
            on_btn_update_clicked();
            ui->tableWidget->selectRow(curRow-1);  //指向删除的上一条
        }
        else if(ui->tableWidget->rowCount() <= 1)   //是第0条，且只有这一条数据
        {
            ui->tableWidget->setRowCount(0);
        }
        else        //是第0条，但后面还有数据，指向新的第0条
        {
            on_btn_update_clicked();
            ui->tableWidget->selectRow(curRow);
        }
    }
}

void MainWindow::on_btn_search_clicked()
{
    QString search_text;
    uint search_num;
    bool res;
    int index = 0;
    QString name;
    uint num;
    double score;

    search_text = ui->lineEdit->text();
    search_num = ui->lineEdit->text().toUInt();

    sql_query.exec(QString("select * from student where name like '%%1%'").arg(search_text));
    res = sql_query.next();
    if(res == 0)
    {
        res = sql_query.exec(QString("select * from student where num like '%%1%'").arg(search_num));
    }
    if(res == 0)
    {
        ui->tableWidget->setRowCount(0);
        ui->statusbar->showMessage("未找到内容！");
    }
    else
    {
        sql_query.previous();
        while(sql_query.next())
        {
            name = sql_query.value(0).toString();
            num = sql_query.value(1).toUInt();
            score = sql_query.value(2).toDouble();

            ui->tableWidget->setRowCount(index+1);

            ui->tableWidget->setItem(index,0,new QTableWidgetItem(name));
            ui->tableWidget->setItem(index,1,new QTableWidgetItem(QString::number(num)));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString("%1").arg(score)));

            index++;
        }
        ui->statusbar->showMessage("搜索结果已更新！");
    }
}

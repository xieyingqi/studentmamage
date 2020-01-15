#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    uint res;
    QString name;
    QString num;
    QString score;

    name = ui->lineEdit->text();
    num = ui->lineEdit_2->text();
    score = ui->lineEdit_3->text();

    if((name == "") || (num == "") || (score == ""))
    {
        QMessageBox::information(this,"提示","信息不完整！","确定");
    }
    else
    {
        if(sigle_from == "0")   //增加
        {
            res = sql_query.exec(QString("insert into student values('%1',%2,%3)").arg(name).arg(num).arg(score));
        }
        else        //修改学号为sig_from的数据
        {
            res = sql_query.exec(QString("update student set name='%1',num=%2,score=%3 where num=%4").arg(name).arg(num).arg(score).arg(sigle_from.toInt()));
        }

        if(res == 1)    //成功，关闭窗口
        {
            update_singnal();
            this->close();
        }
        else        //学号重复，弹窗提示
        {
            QMessageBox::information(this,"提示","学号重复！","确定");
            //QMessageBox::information(this,"提示",sql_query.lastError().text(),"确定");
        }
    }
}

void Dialog::dialog_slot(QSqlQuery query,QString sig_from)
{
    this->show();
    sql_query = query;
    sigle_from = sig_from;

    QString name;
    uint num;
    double score;

    if(sigle_from == "0")
    {
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        this->setWindowTitle("增加");
    }
    else
    {
        sql_query.exec(QString("select * from student where num=%1").arg(sigle_from.toInt()));
        sql_query.next();
        name = sql_query.value(0).toString();
        num = sql_query.value(1).toUInt();
        score = sql_query.value(2).toDouble();

        ui->lineEdit->setText(sql_query.value(0).toString());
        ui->lineEdit_2->setText(sql_query.value(1).toString());
        ui->lineEdit_3->setText(sql_query.value(2).toString());
        this->setWindowTitle("修改");
    }
}

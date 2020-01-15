#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void creatDatabase(void);

private slots:
    void on_btn_update_clicked();

    void on_btn_add_clicked();

    void on_btn_change_clicked();

    void on_btn_del_clicked();
    void on_btn_search_clicked();

signals:
    void dialog_singnal(QSqlQuery query,QString from);

private:
    Ui::MainWindow *ui;
    Dialog dWin;
    QSqlDatabase mydb;
    QSqlQuery sql_query;
    QString from;
};
//extern QSqlQuery sql_query;

#endif // MAINWINDOW_H

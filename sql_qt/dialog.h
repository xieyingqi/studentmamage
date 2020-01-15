#ifndef DIALOG_H
#define DIALOG_H

#include <QMessageBox>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void dialog_slot(QSqlQuery query,QString from);

signals:
    void update_singnal();

private:
    Ui::Dialog *ui;
    QSqlQuery sql_query;
    QString sigle_from;
};

#endif // DIALOG_H

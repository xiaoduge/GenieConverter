#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

void initSql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    db.setHostName("DCJ");
    db.setDatabaseName("DCJ");
    db.setUserName("DCJ");
    db.setPassword("DCJ");
    if(!db.open())
    {
        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Init Database Failed"), QMessageBox::Ok);
        return;
    }

    QString CREATE_TABLE_LOG = "CREATE TABLE IF NOT EXISTS LOG (log_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                       "fileNmae VARCHAR(128) NOT NULL,"
                                                                       "remarks VARCHAR(128),"
                                                                       "log_Time VARCHAR(20) NOT NULL)";
    QSqlQuery query;
    bool success = query.exec(CREATE_TABLE_LOG);
    if(success)
        qDebug()<< QObject::tr("create Log table successed:");
    else
        qDebug()<< QObject::tr("create Log table failed");


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

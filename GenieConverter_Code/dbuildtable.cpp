#include "dbuildtable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

DBuildTable::DBuildTable(QObject *parent) : QObject(parent)
{
    m_type = -1;
}

void DBuildTable::setString(const QString &str)
{
    m_str.clear();
    m_strList.clear();
    m_list.clear();

    m_str = str;
    m_row = 0;
    initData();
    buildTable();
}

//返回数据行数
int DBuildTable::rows()
{
    return m_row;
}

//返回数据列数
int DBuildTable::columns()
{
    if(m_list.at(0).isEmpty())
    {
        return 0;
    }
    return m_list.at(0).size() - 1;
}

//建表
void DBuildTable::buildTable()
{
//    for(int i = 1; i < m_list.size(); i++)
//    {
//        for(int k = 0; k < m_list.at(1).size(); k++)
//        {
//            qDebug() << QString("Cell(%1, %2) :").arg(i).arg(k) <<m_list.at(i).at(k);
//        }

//    }

    bool ok;

    QString tableName = m_list.at(0).at(0);
    if(tableName == QString("Alarm"))
    {
        ok = createAlarmTable();
        m_type = ALARM_TALBE;
    }
    else if(tableName == QString("GetW"))
    {
        ok = createGWaterTable();
        m_type = GWATER_TALBE;
    }
    else if(tableName == QString("PWater"))
    {
        ok = createPWaterTable();
        m_type = PWATER_TALBE;
    }
    else if(tableName == QString("Log"))
    {
        ok = createLogTable();
        m_type = LOG_TALBE;
    }
    else
    {
        m_type = -1;
        QMessageBox::warning(NULL, tr("warning"), tr("Read file error"), QMessageBox::Ok);
        return;
    }

    if(!ok)
    {
        QMessageBox::warning(NULL, tr("warning"), tr("Failed to create data table"), QMessageBox::Ok);
    }

}

//返回数据
const QList<QStringList>& DBuildTable::getDataList() const
{
    return m_list;
}

int DBuildTable::getTableType()
{
    return m_type;
}

void DBuildTable::initData()
{
    m_strList = m_str.split("\n");
    for(int i = 0; i < m_strList.size(); i++)
    {
        QString str = m_strList.at(i);
        QStringList tmpList = str.split(",");

        QString strTmp = tmpList.at(0);
        if(!strTmp.isEmpty())
        {
            m_list << tmpList;
            m_row++;
        }
    }

}

bool DBuildTable::createAlarmTable()
{
    QSqlQuery query;
    QString DELETE_TABLE_ALARM = "Drop Table if exists Alarm";
    bool ok = query.exec(DELETE_TABLE_ALARM);
    if(ok)
    {
        qDebug()<<"delete alarm table successed";
    }
    else
    {
        qDebug()<<"delete alarm table failed";
    }

    QString CREATE_TABLE_Alarm = "CREATE TABLE IF NOT EXISTS  Alarm (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                      "type VARCHAR(20) NOT NULL,"
                                                                      "status INTEGER NULL,"
                                                                      "time VARCHAR(20) NOT NULL)";
    ok = query.exec(CREATE_TABLE_Alarm);
    if(ok)
    {
        qDebug()<<"create alarm table successed";
    }
    else
    {
        qDebug()<<"create alarm table failed";
        return false;
    }

    QString INSERT_sql_Alarm = "INSERT INTO Alarm (id, type, status ,time) VALUES (:id, :type, :status, :time)";
    for(int i = 1; i < m_list.size(); i++)
    {
        query.prepare(INSERT_sql_Alarm);

        query.bindValue(":id", m_list.at(i).at(0).toInt());
        query.bindValue(":type", m_list.at(i).at(1));
        query.bindValue(":status", m_list.at(i).at(2).toInt());
        query.bindValue(":time", m_list.at(i).at(3));

        query.exec();

    }
    return true;
}

bool DBuildTable::createGWaterTable()
{
    QSqlQuery query;
    QString DELETE_TABLE_GetW = "Drop Table if exists GetW";
    bool ok = query.exec(DELETE_TABLE_GetW);
    if(ok)
    {
        qDebug()<<"delete GetW table successed";
    }
    else
    {
        qDebug()<<"delete GetW table failed";
    }

    QString CREATE_TABLE_GetW = "CREATE TABLE IF NOT EXISTS  GetW (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                      "name VARCHAR(20) NOT NULL,"
                                                                      "quantity double NULL,"
                                                                      "quality double NULL,"
                                                                      "tmp double NULL,"
                                                                      "time VARCHAR(20) NOT NULL)";
    ok = query.exec(CREATE_TABLE_GetW);
    if(ok)
    {
        qDebug()<<"create GetW table successed";
    }
    else
    {
        qDebug()<<"create GetW table failed";
        return false;
    }

    QString INSERT_sql_GetW  = "INSERT INTO GetW (id, name, quantity, quality, tmp, time) VALUES (:id, :name, :quantity, :quality, :tmp, :time)";
    for(int i = 1; i < m_list.size(); i++)
    {
        query.prepare(INSERT_sql_GetW);
        query.bindValue(":id", m_list.at(i).at(0).toInt());
        query.bindValue(":name", m_list.at(i).at(1));
        query.bindValue(":quantity",m_list.at(i).at(2).toDouble());
        query.bindValue(":quality",m_list.at(i).at(3).toDouble());
        query.bindValue(":tmp",m_list.at(i).at(4).toDouble());
        query.bindValue(":time", m_list.at(i).at(5));
        query.exec();
    }
    return true;
}

bool DBuildTable::createPWaterTable()
{
    QSqlQuery query;
    QString DELETE_TABLE_PWater = "Drop Table if exists PWater";
    bool ok = query.exec(DELETE_TABLE_PWater);
    if(ok)
    {
        qDebug()<<"delete PWater table successed";
    }
    else
    {
        qDebug()<<"delete PWater table failed";
    }

    QString CREATE_TABLE_Produce_Water = "CREATE TABLE IF NOT EXISTS  PWater (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                      "duration INTEGER NULL,"
                                                                      "ecoroin double NULL,"
                                                                      "tmproin double NULL,"
                                                                      "ecorores double NULL,"
                                                                      "ecoropw double NULL,"
                                                                      "tmpropw double NULL,"
                                                                      "ecoedi double NULL,"
                                                                      "tmpedi double NULL,"
                                                                      "time VARCHAR(20) NOT NULL)";
    ok = query.exec(CREATE_TABLE_Produce_Water);
    if(ok)
    {
        qDebug()<<"create Produce_Water table successed";
    }
    else
    {
        qDebug()<<"create Produce_Water table failed";
        return false;
    }

    QString INSERT_sql_PW    = "INSERT INTO PWater (id, duration, ecoroin, tmproin, ecorores, ecoropw, tmpropw, ecoedi, tmpedi, time) VALUES (:id, :duration, :ecoroin, :tmproin, :ecorores, :ecoropw, :tmpropw, :ecoedi, :tmpedi, :time)";
    for(int i = 1; i < m_list.size(); i++)
    {
        query.prepare(INSERT_sql_PW);
        query.bindValue(":id", m_list.at(i).at(0).toInt());
        query.bindValue(":duration", m_list.at(i).at(1).toInt());
        query.bindValue(":ecoroin",m_list.at(i).at(2).toDouble());
        query.bindValue(":tmproin", m_list.at(i).at(3).toDouble());
        query.bindValue(":ecorores",m_list.at(i).at(4).toDouble());
        query.bindValue(":ecoropw",m_list.at(i).at(5).toDouble());
        query.bindValue(":tmpropw", m_list.at(i).at(6).toDouble());
        query.bindValue(":ecoedi", m_list.at(i).at(7).toDouble());
        query.bindValue(":tmpedi", m_list.at(i).at(8).toDouble());
        query.bindValue(":time", m_list.at(i).at(9));
        query.exec();
    }
    return true;
}

bool DBuildTable::createLogTable()
{
    QSqlQuery query;
    QString DELETE_TABLE_Log = "Drop Table if exists Log";
    bool ok = query.exec(DELETE_TABLE_Log);
    if(ok)
    {
        qDebug()<<"delete PWater table successed";
    }
    else
    {
        qDebug()<<"delete PWater table failed";
    }

    QString CREATE_TABLE_Log = "CREATE TABLE IF NOT EXISTS  Log (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                      "name VARCHAR(20) NOT NULL,"
                                                                      "action VARCHAR(40) NOT NULL,"
                                                                      "info VARCHAR(40) NOT NULL,"
                                                                      "time VARCHAR(20) NOT NULL)";

    ok = query.exec(CREATE_TABLE_Log);
    if(ok)
    {
        qDebug()<<"create log table successed";
    }
    else
    {
        qDebug()<<"create log table failed";
        return false;
    }
    QString INSERT_sql_Log   = "INSERT INTO Log (id, name, action, info, time) VALUES (:id, :name, :action, :info, :time)";
    for(int i = 1; i < m_list.size(); i++)
    {
        query.prepare(INSERT_sql_Log);
        query.bindValue(":id", m_list.at(i).at(0).toInt());
        query.bindValue(":name"  ,m_list.at(i).at(1));
        query.bindValue(":action",m_list.at(i).at(2));
        query.bindValue(":info"  ,m_list.at(i).at(3));
        query.bindValue(":time"  ,m_list.at(i).at(4));

        query.exec();
    }
    return true;
}

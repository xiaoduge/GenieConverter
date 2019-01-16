#ifndef DBUILDTABLE_H
#define DBUILDTABLE_H

#include <QObject>

class DBuildTable : public QObject
{
    Q_OBJECT

public:
    enum TABLE_TYPE
    {
        ALARM_TALBE = 0,
        GWATER_TALBE,
        PWATER_TALBE,
        LOG_TALBE,
        TALBE_NUM,
    };

public:
    explicit DBuildTable(QObject *parent = 0);

    void setString(const QString &str);  //设置数据
    int rows();                          //返回数据行数
    int columns();                       //返回数据列数
    void buildTable();                    //建表
    const QList<QStringList>& getDataList() const; //返回数据
    int getTableType();

signals:

public slots:

private:
    void initData();
    bool createAlarmTable();
    bool createPWaterTable();
    bool createGWaterTable();
    bool createLogTable();

private:
    QString m_str;
    QStringList m_strList;
    QList<QStringList> m_list;

    int m_type;
    int m_row;
};

#endif // DBUILDTABLE_H

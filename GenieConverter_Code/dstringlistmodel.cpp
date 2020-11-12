#include "dstringlistmodel.h"

#include <QDebug>

DStringListModel::DStringListModel(QObject *parent)
    :QAbstractListModel(parent)
{
    m_row = 0;
    m_oldRow = 0;
    m_col = 0;
    m_oldCol = 0;
}

int DStringListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_row - 1; //第一行为表头，去除
}

int DStringListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_col - 1; //第一列为ID，去除
}

QVariant DStringListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row() >= m_list.size())
        return QVariant();
    if(role == Qt::DisplayRole)
    {
        QStringList tmpList = m_list.at(index.row() + 1);
        return tmpList.at(index.column() + 1);
    }
    return QVariant();
}

QVariant DStringListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
        return QString("%1").arg(m_list[0][section + 1]);
    else
        return QString("%1").arg(m_list[section + 1][0]);
}

bool DStringListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return QAbstractListModel::insertRows(row, count, parent);
}

bool DStringListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return QAbstractListModel::removeRows(row, count, parent);
}

void DStringListModel::setString(const QString &str)
{
    m_str.clear();
    m_strList.clear();
    m_list.clear();

    m_oldRow = m_row;
    m_oldCol = m_col;
    m_row = 0;

    m_str = str;

    beginResetModel();
    initDataModel();
    endResetModel();
}

//ce
void DStringListModel::traverModel()
{
    for(int i = 0; i < m_list.size(); i++)
    {
        for(int k = 0; k < m_list.at(0).size(); k++)
        {
            qDebug() << QString("Cell(%1, %2) :").arg(i).arg(k) <<m_list.at(i).at(k);
        }
    }
}

const QList<QStringList>& DStringListModel::getDataList() const
{
    return m_list;
}

int DStringListModel::rows()
{
    return m_row;
}
int DStringListModel::columns()
{
    return m_col;
}

void DStringListModel::initDataModel()
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

    if(m_list.at(0).isEmpty())
    {
        m_col = 0;
    }
    m_col = m_list.at(0).size();

//    traverModel();
}

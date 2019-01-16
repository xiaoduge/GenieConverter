#ifndef DSTRINGLISTMODEL_H
#define DSTRINGLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class DStringListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DStringListModel(QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    void setString(const QString &str);
    int rows();
    int columns();
    void traverModel();
    const QList<QStringList>& getDataList() const;

signals:
    void colsChanged(int oldCount, int newCount);
    void rowsChanged(int oldCount, int newCount);

private:
    void initDataModel();

private:
    QString m_str;
    QStringList m_strList;
    QList<QStringList> m_list;

    int m_row, m_oldRow;
    int m_col, m_oldCol;
};

#endif // DSTRINGLISTMODEL_H

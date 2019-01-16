#ifndef DPRINTTOPDF_H
#define DPRINTTOPDF_H

#include <QObject>

class QPrinter;

class DPrintToPDF : QObject
{
    Q_OBJECT
public:
    DPrintToPDF(QObject *parent = Q_NULLPTR);
    ~DPrintToPDF();
    void setContent(const QList<QStringList>& list, int rows, int cols);
    void setPDFName(const QString &fileName);
    void toPDF();

    const QString getHtml();

private:
    void toHtml(int type);

private:
    QString m_pdfName;
    QList<QStringList> m_list;
    QString m_html;
    QPrinter *m_pPrinter;
    int m_rows, m_cols;
};

#endif // DPRINTTOPDF_H

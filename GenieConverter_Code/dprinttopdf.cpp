#include "dprinttopdf.h"
#include <QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include <QDateTime>
#include <QDebug>

DPrintToPDF::DPrintToPDF(QObject *parent):QObject(parent)
{
    m_pPrinter = new QPrinter;
}

DPrintToPDF::~DPrintToPDF()
{
    if(m_pPrinter != Q_NULLPTR)
    {
        delete m_pPrinter;
        m_pPrinter = Q_NULLPTR;
    }
}

void DPrintToPDF::setContent(const QList<QStringList>& list, int rows, int cols)
{
    m_list.clear();
    m_list = list;
    m_rows = rows;
    m_cols = cols;
}

void DPrintToPDF::setPDFName(const QString &fileName)
{
    m_pdfName = fileName;
}

void DPrintToPDF::toPDF()
{
    toHtml(1);
    m_pPrinter->setPageSize(QPrinter::A4);
    m_pPrinter->setOutputFormat(QPrinter::PdfFormat);
    m_pPrinter->setOutputFileName(m_pdfName);

    QTextDocument textDoc;
    textDoc.setHtml(m_html);
    textDoc.print(m_pPrinter);
    textDoc.end();
}

const QString DPrintToPDF::getHtml()
{
    toHtml(0);
    return m_html;
}

void DPrintToPDF::toHtml(int type)
{
    QTime t;
    t.start();
    m_html.clear();
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QStringList titleList = m_list.at(0);

    if(1 == type)
    {
        m_html += "<style>\
                table.gridtable {\
                font-size:10px;\
                color:#333333;\
                border-width: 1px;\
                border-color: #666666;\
                border-collapse: collapse;\
                }\
                table.gridtable th {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #C7EDCC;\
                }\
                table.gridtable td {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #ffffff;\
                }\
                </style>";

        m_html += "<h1 align=\"center\">" + QString("History") + "</h1>";
        m_html += "<h2 align=\"center\">" + currentTime + "</h2>";
    }
    else
    {
        m_html += "<style>\
                table.gridtable {\
                font-size:18px;\
                color:#333333;\
                border-width: 1px;\
                border-color: #666666;\
                border-collapse: collapse;\
                }\
                table.gridtable th {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #C7EDCC;\
                }\
                table.gridtable td {\
                border-width: 1px;\
                padding: 8px;\
                border-style: solid;\
                border-color: #666666;\
                background-color: #ffffff;\
                }\
                </style>";
         m_html += "<h2 align=\"center\">" + tr("Time:")+currentTime + "</h2>";
    }

    m_html += "<table class=\"gridtable\" align=\"center\">";
    m_html += "<tr>";

    for(int i = 0; i < m_cols; i++)
    {
        m_html += QString("<th>%1</th>").arg(titleList.at(i)); //绘制表头
    }
    m_html += "</tr>";
    for(int i = 1; i < m_rows; i++)
    {
        m_html += "<tr>";
        QStringList contentList = m_list.at(i);

        for(int k = 0; k < m_cols; k++)
        {
            QString tmp = contentList.at(k);
            m_html += QString("<td>%1</td>").arg(tmp);
        }
        m_html += "</tr>";
    }
    m_html += "</table>";

    qDebug("Time elapsed: %d ms", t.elapsed());
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dstringlistmodel.h"
#include "daboutdialog.h"
#include "dsetdialog.h"
#include "config.h"
#include <QListWidget>
#include <QTableView>
#include <QGridLayout>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QLabel>
#include <QTranslator>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextBlock>
#include <QTime>
#include <QWebEngineView>
#include <QDebug>
#include <QTableView>
#include <QHeaderView>

QString version = QString("0.1.1.190116_release");
QString builtTime = QString("Jan 16 2019");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(800, 600);
    initMenuAndTool();
    initStatusBar();
    initUI();
    isConverted = false;
    initSubDialog();

    int iLan = MainRetriveConfig();
    switchLanguage(iLan);
    setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setInitFile(const QString &fileName)
{
    if(!fileName.isEmpty())
    {
        m_pListWidget->clear();
        m_pListWidget->addItem(fileName);
        m_pStatusLabel->setText(tr("Ready"));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragEnterEvent";
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent";
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QStringList fileList;
        QList<QUrl> urlList = mimeData->urls();
        for(int i = 0; i < urlList.size(); i++)
        {
            fileList << urlList.at(i).toLocalFile();
            qDebug() << QString("urlList.at(%1).toLocalFile()").arg(i)
                     << urlList.at(i).toLocalFile();
        }
        m_pListWidget->addItems(fileList);
    }
}

void MainWindow::onOpenActionTriggered()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this,
                                                         tr("Open File"),
                                                         "C:/Users/ADMIN/Desktop/");

    m_pListWidget->clear();
    if(fileList.size() > 0)
    {
        m_pListWidget->addItems(fileList);
        m_pStatusLabel->setText(tr("Ready"));
    }
}

void MainWindow::onSaveActionTriggered()
{
    if(!isConverted)
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save"),
                                                    "C:/Users/ADMIN/Desktop/",
                                                    "Text files (*.txt)");
    if(fileName.isEmpty())
    {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Waring"), tr("Failed to save file"), QMessageBox::Ok);
        return;
    }
    file.write(m_byteArray);
    file.close();

    m_pStatusLabel->setText(tr("Saved"));
}

void MainWindow::onSetActionTriggered()
{
    m_pSetDialog->exec();
}

void MainWindow::onConverterActionTriggered()
{
    if(m_currentFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Waring"), tr("No valid file selected"), QMessageBox::Ok);
        return;
    }
    m_pStatusLabel->setText(tr("Processing data"));
    m_pStatusLabel->repaint();

    QFile sourceFile(m_currentFileName);
    if(!sourceFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Waring"), tr("Failed to open source file"), QMessageBox::Ok);
        return;
    }

    m_byteArray = QByteArray::fromBase64(sourceFile.readAll());
    isConverted = true;
    QString strText(m_byteArray);

    m_pStringModel->setString(strText);

    m_pTableView->setModel(m_pStringModel);

    int cols = m_pStringModel->columnCount();
    for(int i = 0; i < cols - 1; i++)
    {
        m_pTableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    m_pTableView->setColumnWidth(cols - 1, 180);

    m_pStatusLabel->setText(tr("Data conversion completed"));

}

void MainWindow::onExportActionTriggered()
{
    if(!isConverted)
    {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save PDF"),
                                                    "C:/Users/ADMIN/Desktop/",
                                                    "PDF (*.PDF)");

    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printDocument(&printer);

    m_pStatusLabel->setText(tr("Export completed"));
}

void MainWindow::onDirActionToggled(bool isChecked)
{
    if(isChecked)
    {
        m_pDockWidget->show();
    }
    else
    {
        m_pDockWidget->hide();
    }
}

void MainWindow::onAboutActionTriggered()
{
    m_pAboutDlg->exec();
}

void MainWindow::onListWidgetCurrentTextChanged(const QString &currentText)
{
    m_currentFileName = currentText;
    m_pStatusLabel->setText(m_currentFileName);
}

void MainWindow::refresh()
{
    this->setWindowTitle(tr("Converter"));
    m_pMenu[FILE_MENU]->setTitle(tr("File"));
    m_pAction[OPEN_ACTION]->setText(tr("Open(&O)"));
    m_pAction[SAVE_ACTION]->setText(tr("Save(&S)"));

    m_pAction[PRINT_ACTION]->setText(tr("Print"));
    m_pAction[PRINTPRE_ACION]->setText(tr("Print Preview"));

    m_pMenu[TOOL_MENU]->setTitle(tr("Tool"));
    m_pAction[SET_ACTION]->setText(tr("Setting"));


    m_pMenu[OPERATE_MENU]->setTitle(tr("Operate"));
    m_pAction[CONVERTER_ACTION]->setText(tr("Convert"));
    m_pAction[EXPORT_ACTION]->setText(tr("Export"));

    m_pMenu[WINDOW_MENU]->setTitle(tr("Window"));
    m_pAction[DIR_ACTION]->setText(tr("Directory Window"));

//    m_pMenu[HELP_MENU]->setTitle(tr("Help"));
//    m_pAction[ABOUT_ACTION]->setText(tr("About"));

    m_pDockWidget->setWindowTitle(tr("File"));
    m_pStatusLabel->setText(tr("Welcome"));
}

void MainWindow::initMenuAndTool()
{
    m_pMenu[FILE_MENU] = ui->menuBar->addMenu(tr("File"));
    m_pAction[OPEN_ACTION] = new QAction(QIcon(":/images/fileopen.png"), tr("Open(&O)"), this);
    m_pAction[SAVE_ACTION] = new QAction(QIcon(":/images/filesave.png"), tr("Save(&S)"), this);

    m_pAction[PRINT_ACTION] = new QAction(QIcon(":/images/print.png"), tr("Print"), this);
    m_pAction[PRINTPRE_ACION] = new QAction(QIcon(":/images/printpre.png"), tr("Print Preview"), this);

    QList<QAction*> fileMenuList;
    fileMenuList << m_pAction[OPEN_ACTION] << m_pAction[SAVE_ACTION] << m_pAction[PRINT_ACTION] << m_pAction[PRINTPRE_ACION];
    m_pMenu[FILE_MENU]->addActions(fileMenuList);

    m_pMenu[TOOL_MENU] = ui->menuBar->addMenu(tr("Tool"));
    m_pAction[SET_ACTION] = new QAction(QIcon(":/images/set.png"), tr("Setting"), this);
    QList<QAction*> toolMenuList;
    toolMenuList << m_pAction[SET_ACTION];
    m_pMenu[TOOL_MENU]->addActions(toolMenuList);

    m_pMenu[OPERATE_MENU] = ui->menuBar->addMenu(tr("Operate"));
    m_pAction[CONVERTER_ACTION] = new QAction(QIcon(":/images/converter.png"), tr("Convert"), this);
    m_pAction[EXPORT_ACTION] = new QAction(QIcon(":/images/export.png"), tr("Export"), this);
    QList<QAction*> operateMenuList;
    operateMenuList << m_pAction[CONVERTER_ACTION] << m_pAction[EXPORT_ACTION];
    m_pMenu[OPERATE_MENU]->addActions(operateMenuList);

    m_pMenu[WINDOW_MENU] = ui->menuBar->addMenu(tr("Window"));
    m_pAction[DIR_ACTION] = new QAction(tr("Directory Window"), this);
    m_pAction[DIR_ACTION]->setCheckable(true);
    m_pAction[DIR_ACTION]->setChecked(true);
    QList<QAction*> windowMenuList;
    windowMenuList << m_pAction[DIR_ACTION];
    m_pMenu[WINDOW_MENU]->addActions(windowMenuList);

//    m_pMenu[HELP_MENU] = ui->menuBar->addMenu(tr("Help"));
//    m_pAction[ABOUT_ACTION] = new QAction(QIcon(":/images/about.png"), tr("About"), this);
//    QList<QAction*> helpMenuList;
//    helpMenuList << m_pAction[ABOUT_ACTION];
//    m_pMenu[HELP_MENU]->addActions(helpMenuList);

    ui->mainToolBar->addActions(fileMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(toolMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(operateMenuList);
    ui->mainToolBar->addSeparator();
//    ui->mainToolBar->addActions(helpMenuList);

    connect(m_pAction[OPEN_ACTION], SIGNAL(triggered()), this, SLOT(onOpenActionTriggered()));
    connect(m_pAction[SAVE_ACTION], SIGNAL(triggered()), this, SLOT(onSaveActionTriggered()));

    connect(m_pAction[PRINT_ACTION], SIGNAL(triggered()), this, SLOT(doPrint()));
    connect(m_pAction[PRINTPRE_ACION], SIGNAL(triggered()), this, SLOT(doPrintPreview()));

    connect(m_pAction[SET_ACTION], SIGNAL(triggered()), this, SLOT(onSetActionTriggered()));
    connect(m_pAction[CONVERTER_ACTION], SIGNAL(triggered()), this, SLOT(onConverterActionTriggered()));
    connect(m_pAction[EXPORT_ACTION], SIGNAL(triggered()), this, SLOT(onExportActionTriggered()));
//    connect(m_pAction[ABOUT_ACTION], SIGNAL(triggered()), this, SLOT( onAboutActionTriggered()));

    connect(m_pAction[DIR_ACTION], SIGNAL(toggled(bool)), this, SLOT(onDirActionToggled(bool)));

}

void MainWindow::initStatusBar()
{
//    QLabel *statusLabel = new QLabel;
//    statusLabel->setFont(QFont("Arial", 10, QFont::Bold, true));
//    statusLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
//    statusLabel->setText(tr("<a href=\"http://www.rephile.cn\" style=\"color:black;text-decoration:none;\">RephiLe Bioscience, Ltd.</a>"));
//    statusLabel->setStyleSheet("link-color:#FF0000;text-decoration:underline;");
//    statusLabel->setTextFormat(Qt::RichText);
//    statusLabel->setOpenExternalLinks(true);
//    QPalette palette = statusLabel->palette();
//    palette.setBrush(QPalette::Text, QBrush(Qt::white));
//    statusLabel->setPalette(palette);
//    ui->statusBar->addPermanentWidget(statusLabel);

    m_pStatusLabel = new QLabel;
    m_pStatusLabel->setFont(QFont("Arial", 10, QFont::Bold, true));
    ui->statusBar->addWidget(m_pStatusLabel);
}

void MainWindow::initUI()
{
    m_pDockWidget = new QDockWidget(tr("File"), this);
    m_pListWidget = new QListWidget(m_pDockWidget);

    m_pDockWidget->setWidget(m_pListWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    m_pStringModel = new DStringListModel(this);

    m_pTextEdit = new QTextEdit(this);
    QHBoxLayout *layout = new QHBoxLayout;
    m_pTextEdit->setReadOnly(true);
    layout->addWidget(m_pTextEdit);
    m_pTextEdit->hide();

    m_pTableView = new QTableView(this);
    layout->addWidget(m_pTableView);

    ui->centralWidget->setLayout(layout);

    connect(m_pListWidget, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onListWidgetCurrentTextChanged(const QString &)));
}

void MainWindow::initSubDialog()
{
    m_pAboutDlg = new DAboutDialog(this);

    m_pSetDialog = new DSetDialog(this);
    connect(m_pSetDialog, SIGNAL(languageChanged(int)), this, SLOT(switchLanguage(int)));

}

//切换语言
void MainWindow::switchLanguage(int iLan)
{
    QTranslator *pTranslator = new QTranslator(this);
    bool ok = false;
    switch (iLan)
    {
    case CN_LAN:
        qDebug() << "Load cn";
        ok = pTranslator->load(QLatin1String(":/language/cn.qm"));
        if(!ok)
        {
            qDebug() << "Load cn language failed";
        }
        break;
    case EN_LAN:
        qDebug() << "Load en";
        ok = pTranslator->load(QLatin1String(":/language/en.qm"));
        if(!ok)
        {
            qDebug() << "Load en language failed";
        }
        break;
    }

    if(!ok)
    {
        qDebug() << "Load language failed";
    }

    ok = qApp->installTranslator(pTranslator);
    if(!ok)
    {
        qDebug() << "qApp->installTranslator falied";
    }

    this->refresh();
    m_pSetDialog->refresh();
    m_pAboutDlg->refresh();
}

//打印
void MainWindow::doPrint()
{
    if(!isConverted)
    {
        return;
    }

    QPrinter printer;
    QPrintDialog printDlg(&printer, this);
    printDlg.resize(800, 600);
    if(printDlg.exec() == QDialog::Accepted)
    {
        printDocument(&printer);
    }
}

//预览
void MainWindow::doPrintPreview()
{
    if(!isConverted)
    {
        return;
    }

    QPrinter printer;
    QPrintPreviewDialog printPreviewDlg(&printer, this);
    connect(&printPreviewDlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printDocument(QPrinter*)));

    printPreviewDlg.exec();
}

//打印、打印预览、导出PDF时调用
void MainWindow::printDocument(QPrinter* printer)
{
    QPainter painter(printer);
    painter.setFont(QFont("Arial", 8));
    int rows = m_pStringModel->rows();
    int cols = m_pStringModel->columns();

    int startX = 10;
    int colWidth = (painter.device()->width() - startX*2) / cols;

    int startY = 24;
    int rowHeight = 24;

    int curRow = 0;
    int xPos = 0;
    int yPos = 0;

    painter.drawLine(startX, startY, painter.device()->width() - startX, startY); //画第一页的起始线

    int offset = colWidth / 2;
    for(int i = 0; i < rows; i++)
    {
        //判断是否需要新建一页
        if((yPos + startY + rowHeight) > painter.device()->height())
        {
            printer->newPage();
            curRow = 0;
            painter.drawLine(startX, startY, painter.device()->width() - startX, startY);
        }

        QString strRow;
        QStringList contentList = m_pStringModel->getDataList().at(i);
        yPos = 40 + curRow*24;

        for(int k = 0; k < cols; k++)
        {
            QString tmp = contentList.at(k);
            switch (k)
            {
            case 0:
            case 1:
                xPos = startX + k * offset;
                break;
            default:
                xPos = startX + k * colWidth - offset;
                break;
            }
            painter.drawText(xPos, yPos, tmp);
        }
        painter.drawLine(startX, startY + (curRow + 1) * rowHeight,
                        painter.device()->width() - startX, startY + (curRow + 1) * rowHeight);
        curRow++;
    }
}




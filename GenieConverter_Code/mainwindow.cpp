#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dstringlistmodel.h"
#include "daboutdialog.h"
#include "dsetdialog.h"
#include "dprinttopdf.h"
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

#include <QDebug>


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

void MainWindow::on_openAction_triggered()
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

void MainWindow::on_saveAction_triggered()
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

void MainWindow::on_setAction_triggered()
{
    m_pSetDialog->exec();
}

void MainWindow::on_converterAction_triggered()
{
    m_pStatusLabel->setText(tr("Processing data"));
    m_pStatusLabel->repaint();

    if(m_currentFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Waring"), tr("No valid file selected"), QMessageBox::Ok);
        return;
    }

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

    m_pPrintToPDF->setContent(m_pStringModel->getDataList(), m_pStringModel->rows(), m_pStringModel->columns());
    QString html = m_pPrintToPDF->getHtml();
    m_pTextEdit->setHtml(html);
    m_pStatusLabel->setText(tr("Data conversion completed"));

}

void MainWindow::on_exportAction_triggered()
{
    if(!isConverted)
    {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save PDF"),
                                                    "C:/Users/ADMIN/Desktop/",
                                                    "PDF (*.PDF)");
    m_pPrintToPDF->setPDFName(fileName);
    m_pPrintToPDF->toPDF();
    m_pStatusLabel->setText(tr("Export completed"));
}

void MainWindow::on_dirAction_toggled(bool isChecked)
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

void MainWindow::on_aboutAction_triggered()
{
    m_pAboutDlg->exec();
}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
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

    m_pMenu[TOOL_MENU]->setTitle(tr("Tool"));
    m_pAction[SET_ACTION]->setText(tr("Setting"));


    m_pMenu[OPERATE_MENU]->setTitle(tr("Operate"));
    m_pAction[CONVERTER_ACTION]->setText(tr("Converter"));
    m_pAction[EXPORT_ACTION]->setText(tr("Export"));

    m_pMenu[WINDOW_MENU]->setTitle(tr("Window"));
    m_pAction[DIR_ACTION]->setText(tr("Directory Window"));

    m_pMenu[HELP_MENU]->setTitle(tr("Help"));
    m_pAction[ABOUT_ACTION]->setText(tr("About"));

    m_pDockWidget->setWindowTitle(tr("File"));
    m_pStatusLabel->setText(tr("Welcome"));
}

void MainWindow::initMenuAndTool()
{
    m_pMenu[FILE_MENU] = ui->menuBar->addMenu(tr("File"));
    m_pAction[OPEN_ACTION] = new QAction(QIcon(":/images/fileopen.png"), tr("Open(&O)"), this);
    m_pAction[SAVE_ACTION] = new QAction(QIcon(":/images/filesave.png"), tr("Save(&S)"), this);
    QList<QAction*> fileMenuList;
    fileMenuList << m_pAction[OPEN_ACTION] << m_pAction[SAVE_ACTION];
    m_pMenu[FILE_MENU]->addActions(fileMenuList);

    m_pMenu[TOOL_MENU] = ui->menuBar->addMenu(tr("Tool"));
    m_pAction[SET_ACTION] = new QAction(QIcon(":/images/set.png"), tr("Setting"), this);
    QList<QAction*> toolMenuList;
    toolMenuList << m_pAction[SET_ACTION];
    m_pMenu[TOOL_MENU]->addActions(toolMenuList);

    m_pMenu[OPERATE_MENU] = ui->menuBar->addMenu(tr("Operate"));
    m_pAction[CONVERTER_ACTION] = new QAction(QIcon(":/images/converter.png"), tr("Converter"), this);
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

    m_pMenu[HELP_MENU] = ui->menuBar->addMenu(tr("Help"));
    m_pAction[ABOUT_ACTION] = new QAction(QIcon(":/images/about.png"), tr("About"), this);
    QList<QAction*> helpMenuList;
    helpMenuList << m_pAction[ABOUT_ACTION];
    m_pMenu[HELP_MENU]->addActions(helpMenuList);


    ui->mainToolBar->addActions(fileMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(toolMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(operateMenuList);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions(helpMenuList);

    connect(m_pAction[OPEN_ACTION], SIGNAL(triggered()), this, SLOT(on_openAction_triggered()));
    connect(m_pAction[SAVE_ACTION], SIGNAL(triggered()), this, SLOT(on_saveAction_triggered()));
    connect(m_pAction[SET_ACTION], SIGNAL(triggered()), this, SLOT(on_setAction_triggered()));
    connect(m_pAction[CONVERTER_ACTION], SIGNAL(triggered()), this, SLOT(on_converterAction_triggered()));
    connect(m_pAction[EXPORT_ACTION], SIGNAL(triggered()), this, SLOT(on_exportAction_triggered()));
    connect(m_pAction[ ABOUT_ACTION], SIGNAL(triggered()), this, SLOT( on_aboutAction_triggered()));

    connect(m_pAction[DIR_ACTION], SIGNAL(toggled(bool)), this, SLOT(on_dirAction_toggled(bool)));

}

void MainWindow::initStatusBar()
{
    QLabel *statusLabel = new QLabel;
    statusLabel->setFont(QFont("Arial", 10, QFont::Bold, true));
    statusLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    statusLabel->setText(tr("<a href=\"http://www.rephile.cn\" style=\"color:black;text-decoration:none;\">RephiLe Bioscience, Ltd.</a>"));
    statusLabel->setStyleSheet("link-color:#FF0000;text-decoration:underline;");
    statusLabel->setTextFormat(Qt::RichText);
    statusLabel->setOpenExternalLinks(true);
    QPalette palette = statusLabel->palette();
    palette.setBrush(QPalette::Text, QBrush(Qt::white));
    statusLabel->setPalette(palette);
    ui->statusBar->addPermanentWidget(statusLabel);

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
    m_pPrintToPDF = new DPrintToPDF(this);

    m_pTextEdit = new QTextEdit;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_pTextEdit);
    m_pTextEdit->setReadOnly(true);

    ui->centralWidget->setLayout(layout);

    connect(m_pListWidget, SIGNAL(currentTextChanged(const QString &)), this, SLOT(on_listWidget_currentTextChanged(const QString &)));
}

void MainWindow::initSubDialog()
{
    m_pAboutDlg = new DAboutDialog(this);

    m_pSetDialog = new DSetDialog(this);
    connect(m_pSetDialog, SIGNAL(languageChanged(int)), this, SLOT(switchLanguage(int)));

}

void MainWindow::switchLanguage(int iLan)
{
    QTranslator *pTranslator = new QTranslator(this);
    bool ok;
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
    default:
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




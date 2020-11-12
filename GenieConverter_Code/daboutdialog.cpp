#include "daboutdialog.h"
#include "mainwindow.h"
#include <QLabel>
#include <QPushButton>

DAboutDialog::DAboutDialog(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    initUI();
    this->resize(350, 150);
}

void DAboutDialog::refresh()
{
    this->setWindowTitle(tr("About"));
    m_pCloseBtn->setText(tr("Close"));
}

void DAboutDialog::on_closeBtn_clicked()
{
    this->close();
}

void DAboutDialog::initUI()
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, Qt::white);
    this->setPalette(palette);

    for(int i = 0; i < LAB_NUM; i++)
    {
        m_pLabel[i] = new QLabel(this);
    }

    m_pCloseBtn = new QPushButton(tr("Close"), this);
    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(on_closeBtn_clicked()));

    buildTranslation();

    QPixmap pixmap = QPixmap(":/images/logo.png");
    m_pLabel[LAB_LOGO]->setPixmap(pixmap);
    m_pLabel[LAB_LOGO]->setScaledContents(true);

    m_pLabel[LAB_LOGO]->setGeometry(5, 30, 120, 43);
    m_pLabel[LAB_VERSION]->setGeometry(140, 20, 180, 20);
    m_pLabel[LAB_BUILTTIME]->setGeometry(140, 50, 150, 20);
    m_pLabel[LAB_AUTHOR]->setGeometry(140, 80, 150, 20);
    m_pCloseBtn->setGeometry(200, 110, 100, 25);

}

void DAboutDialog::buildTranslation()
{
    m_pLabel[LAB_VERSION]->setText(tr("Version: %1").arg(version));
    m_pLabel[LAB_BUILTTIME]->setText(tr("Built on %1").arg(builtTime));
    m_pLabel[LAB_AUTHOR]->setText(tr("RephiLe Bioscience, Ltd."));

}

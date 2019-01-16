#include "dsetdialog.h"
#include "ui_dsetdialog.h"
#include "config.h"

DSetDialog::DSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSetDialog)
{
    ui->setupUi(this);

    this->setFixedSize(250, 150);

    initUI();
}

DSetDialog::~DSetDialog()
{
    delete ui;
}

void DSetDialog::refresh()
{
    this->setWindowTitle(tr("Option"));
    ui->pushButton_save->setText(tr("Save"));
    ui->label_Lan->setText(tr("Language"));
    ui->comboBox_Lan->setItemText(0, tr("Chinese"));
    ui->comboBox_Lan->setItemText(1, tr("Englist"));
}

void DSetDialog::on_pushButton_save_clicked()
{
    int index = ui->comboBox_Lan->currentIndex();
    int iLan = MainRetriveConfig();
    if(index != iLan)
    {
        MainSaveConfig(index);
        emit languageChanged(index);
    }


    this->close();
}

void DSetDialog::initUI()
{
    ui->pushButton_save->setText(tr("Save"));
    ui->label_Lan->setText(tr("Language"));
    QStringList comList;
    comList << tr("Chinese") << tr("Englist");
    ui->comboBox_Lan->addItems(comList);

    ui->comboBox_Lan->setMinimumWidth(80);

    int iLan = MainRetriveConfig();
    ui->comboBox_Lan->setCurrentIndex(iLan);
}

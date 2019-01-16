#include "config.h"
#include <QSettings>

int MainRetriveConfig()
{
    QString fileName = "config.ini";
    QSettings *config = new QSettings(fileName, QSettings::IniFormat);

    QString strV = "/language/";
    int iLan = config->value(strV, 1).toInt();

    if (config)
    {
        delete config;
        config = NULL;
    }
    return iLan;
}

void MainSaveConfig(int iLan)
{
    QString fileName = "config.ini";
    QSettings *config = new QSettings(fileName, QSettings::IniFormat);

    QString strV = "/language/";
    QString strTmp = QString::number(iLan);
    config->setValue(strV, strTmp);

    if (config)
    {
        delete config;
        config = NULL;
    }
}

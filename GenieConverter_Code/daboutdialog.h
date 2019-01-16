#ifndef DABOUTDIALOG_H
#define DABOUTDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;

class DAboutDialog : public QDialog
{
    Q_OBJECT

public:
    enum UI_LABEL
    {
        LAB_LOGO,
        LAB_VERSION,
        LAB_BUILTTIME,
        LAB_AUTHOR,
        LAB_NUM
    };
public:
    DAboutDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void refresh();

protected slots:
    void on_closeBtn_clicked();

private:
    void initUI();
    void buildTranslation();

private:
    QLabel *m_pLabel[LAB_NUM];
    QPushButton *m_pCloseBtn;
};

#endif // DABOUTDIALOG_H

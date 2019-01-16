#ifndef DSETDIALOG_H
#define DSETDIALOG_H

#include <QDialog>

namespace Ui {
class DSetDialog;
}

class DSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSetDialog(QWidget *parent = 0);
    ~DSetDialog();

    void refresh();

signals:
    void languageChanged(int iLan);

private slots:
    void on_pushButton_save_clicked();

private:
    void initUI();

private:
    Ui::DSetDialog *ui;
};

#endif // DSETDIALOG_H

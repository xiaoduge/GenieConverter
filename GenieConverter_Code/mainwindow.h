#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QListWidget;
class QTableView;
class QDockWidget;
class QTextEdit;
class DStringListModel;
class DPrintToPDF;
class QLabel;
class DAboutDialog;
class DBuildTable;
class QSqlTableModel;
class DSetDialog;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum LANGUAGE
    {
        CN_LAN = 0,
        EN_LAN,
        LAN_NUM
    };

    enum ACTION_NAME
    {
        OPEN_ACTION = 0,
        SAVE_ACTION,
        SET_ACTION,
        CONVERTER_ACTION,
        EXPORT_ACTION,
        DIR_ACTION,
        ABOUT_ACTION,
        ACTION_NUM
    };

    enum MENU_NAME
    {
        FILE_MENU = 0,
        TOOL_MENU,
        OPERATE_MENU,
        WINDOW_MENU,
        HELP_MENU,
        MENU_NUM
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event);  //拖动进入事件
    void dropEvent(QDropEvent *event);            //拖动放下事件

protected slots:
    void on_openAction_triggered();
    void on_saveAction_triggered();
    void on_setAction_triggered();
    void on_converterAction_triggered();
    void on_exportAction_triggered();

    void on_dirAction_toggled(bool);

    void on_aboutAction_triggered();

    void on_listWidget_currentTextChanged(const QString &);
    void refresh();
    void switchLanguage(int iLan);

private:
    void initMenuAndTool();
    void initStatusBar();
    void initUI();

    void initSubDialog();


private:
    Ui::MainWindow *ui;
    QListWidget *m_pListWidget;
    QTextEdit *m_pTextEdit;
    QDockWidget *m_pDockWidget;
    QByteArray m_byteArray;

    QString m_currentFileName;
    DStringListModel *m_pStringModel;

    DPrintToPDF *m_pPrintToPDF;
    QLabel *m_pStatusLabel;
    bool isConverted;

    DSetDialog *m_pSetDialog;
    DAboutDialog *m_pAboutDlg;

    QAction *m_pAction[ACTION_NUM];
    QMenu *m_pMenu[MENU_NUM];
};

#endif // MAINWINDOW_H

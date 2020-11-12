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
class QLabel;
class DAboutDialog;
class DBuildTable;
class QSqlTableModel;
class DSetDialog;
class QAction;
class QMenu;
class QPrinter;
class QWebEngineView;
class QTextDocument;
class QTableView;

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
        PRINT_ACTION,
        PRINTPRE_ACION,
        SET_ACTION,
        CONVERTER_ACTION,
        EXPORT_ACTION,
        DIR_ACTION,
//        ABOUT_ACTION,
        ACTION_NUM
    };

    enum MENU_NAME
    {
        FILE_MENU = 0,
        TOOL_MENU,
        OPERATE_MENU,
        WINDOW_MENU,
//        HELP_MENU,
        MENU_NUM
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setInitFile(const QString& fileName);
protected:
    void dragEnterEvent(QDragEnterEvent *event);  //拖动进入事件
    void dropEvent(QDropEvent *event);            //拖动放下事件

protected slots:
    void onOpenActionTriggered();
    void onSaveActionTriggered();
    void onSetActionTriggered();
    void onConverterActionTriggered();
    void onExportActionTriggered();

    void onDirActionToggled(bool);

    void onAboutActionTriggered();

    void onListWidgetCurrentTextChanged(const QString &);
    void refresh();
    void switchLanguage(int iLan);

    //print
    void doPrint();
    void doPrintPreview();
    void printDocument(QPrinter* printer);

private:
    void initMenuAndTool();
    void initStatusBar();
    void initUI();

    void initSubDialog();


private:
    Ui::MainWindow *ui;
    QListWidget *m_pListWidget;
    QTextEdit *m_pTextEdit;
    QTextDocument *m_pTextDoc;
    QDockWidget *m_pDockWidget;
    QByteArray m_byteArray;

    QTableView *m_pTableView;

    QString m_currentFileName;
    DStringListModel *m_pStringModel;

    QLabel *m_pStatusLabel;
    bool isConverted;
    QString m_html;

    DSetDialog *m_pSetDialog;
    DAboutDialog *m_pAboutDlg;

    QAction *m_pAction[ACTION_NUM];
    QMenu *m_pMenu[MENU_NUM];
};

extern QString version;
extern QString builtTime;

#endif // MAINWINDOW_H

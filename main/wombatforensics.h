#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatdatabase.h"
#include "wombatframework.h"
#include "wombatfunctions.h"
#include "ui_wombatforensics.h"
#include "progresswindow.h"
#include "exportdialog.h"
#include "globals.h"

namespace Ui {
class WombatForensics;
}

class WombatForensics : public QMainWindow
{
    Q_OBJECT

public:
    explicit WombatForensics(QWidget *parent = 0);
    ~WombatForensics();
    WombatDatabase* wombatdatabase;
    WombatVariable wombatvariable;
    WombatVariable* wombatvarptr;
    TskObject tskobject;
    TskObject* tskobjptr;
    WombatFramework* wombatframework;
    ProgressWindow* wombatprogresswindow;
    ExportDialog* exportdialog;

signals:
    void LogVariable(WombatVariable* wombatVariable);
private slots:
    void AddEvidence();
    void RemEvidence();
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered(bool checked);
    void UpdateProgress(int count, int processcount);
    void UpdateMessageTable();
    void dirTreeView_selectionChanged(const QModelIndex &index);
    void HideProgressWindow(bool checkstate);
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void PopulateProgressWindow(WombatVariable* wvariable);
    void ResizeColumns(void);
    void OpenParentImage(int imgid);
    void ResizeViewColumns(const QModelIndex &index)
    {
        ResizeColumns();
    }
    void FileExport(FileExportData* exportdata);
    void setScrollBarRange(off_t low, off_t high);
    void setScrollBarValue(off_t pos);
    void setOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
    void ViewGroupTriggered(QAction* curaction);
    void LoadComplete(bool isok);
    void InitializeQueryModel(void);
    void UpdateTree(void);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void SetupToolbar(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeEvidenceStructure(void);
    void InitializeOpenCase(void);
    void InitializeWombatFramework(void);
    void InitializeDirModel(void);
    //void InitializeQueryModel(void);
    void UpdateViewer(void);
    void UpdateOmniValue(void);
    void LoadHexContents(void);
    void LoadTxtContents(void);
    void LoadWebContents(void);
    void LoadImgContents(void);
    void LoadVidContents(void);

    void RemoveTmpFiles(void);

    //QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    //QThreadPool* threadpool;
    int ReturnVisibleViewerID();
    int StandardItemCheckState(QStandardItem* tmpitem, int checkcount);
    int StandardItemListCount(QStandardItem* tmpitem, int listcount);
    QVector<FileExportData> SetFileExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData>);
    QVector<FileExportData> SetListExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData>);
    int DetermineOmniView(QString currentSignature);
    QModelIndex curselindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    QFuture<void> openfuture;
    QFutureWatcher<void> openwatcher;

    off_t offset() const;
    HexEditor* hexwidget;
    QActionGroup* viewgroup;
    QScrollBar* hexvsb;
    QStatusBar* hstatus;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* selectedascii;
    QLabel* selectedinteger;
    QLabel* selectedfloat;
    QLabel* selecteddouble;

    // NEED TO REMOVE THESE ONCE I GET MY FUNCTIONS DONE AND REMOVE THE OLD STRUCTURE
    //QStandardItemModel* currenttreemodel;
    //QStandardItemModel* wombatdirmodel;
    //QStandardItemModel* wombattypmodel;
};
/*
class ThreadRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ThreadRunner(QObject* object, QString input, WombatVariable* wVariable)
    {
        method = input;
        //caller = (SleuthKitPlugin*)object;
        wombatvariable = wVariable;
    };
    void run()
    {
        if(method.compare("initialize") == 0)
            caller->Initialize(wombatvariable);
        if(method.compare("openevidence") == 0)
            caller->OpenEvidence(wombatvariable);
        if(method.compare("populatecase") == 0)
            caller->PopulateCase(wombatvariable);
        if(method.compare("showfile") == 0)
            caller->ShowFile(wombatvariable);
        if(method.compare("refreshtreeviews") == 0)
            caller->RefreshTreeViews(wombatvariable);
        if(method.compare("exportfiles") == 0)
            caller->ExportFiles(wombatvariable);
    };
private:
    QString method;
    //SleuthKitPlugin* caller;
    WombatVariable* wombatvariable;
};
*/

#endif // WOMBATFORENSICS_H

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QtCore>
#include <QVariant>
#include <QList>
#include <QMap>
//#include "progresswindow.h"
#include "wombatvariable.h"

class QStringList;
class QObject;
class QWidget;
class QStandardItem;

// might not need.
template <class T> class VPtr
{
public:
    static T* asPtr(QVariant v)
    {
        return  (T *) v.value<void *>();
    }
    static QVariant asQVariant(T* ptr)
    {
        return qVariantFromValue((void *) ptr);
    }
};

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    Q_INVOKABLE virtual PluginMap Initialize(WombatVariable wombatvariable) = 0;
    Q_INVOKABLE virtual void Run(QString input) = 0;
    virtual void Finalize() = 0;
};

class ViewerInterface
{
public:
    virtual ~ViewerInterface() {}

    virtual QList<ViewerMap> Initialize() = 0;
    virtual void Run(QString input) = 0;
    virtual void Finalize() = 0;
};
/*
class ListInterface
{
public:
    virtual ~ViewerInterface() {}

    virtual QVariantMap Initialize() = 0;
    virtual void Run(QString input) = 0;
    virtual void Finalize() = 0;
};
*/
/*
class BasicToolsInterface
{
public:
    virtual ~BasicToolsInterface() {}

    virtual QWidget* setupTypTab() = 0;
    virtual QWidget* setupDirTab() = 0;
    virtual QWidget* setupHexTab() = 0;
    virtual QWidget* setupTxtTab() = 0;
    virtual QStringList evidenceToolboxIcons() const = 0;
    virtual void LoadHexModel(QString tmpFilePath) = 0;
    virtual void LoadTxtContent(QString asciiText) = 0;
};
*/
class SleuthKitInterface
{
public:
    virtual ~SleuthKitInterface() {}

    virtual void Initialize(WombatVariable wombatVariable) = 0;
    //virtual void Run(QString input);
    //virtual void Finalize() {} = 0;
    virtual void SetupSystemProperties() = 0;
    virtual void SetupLog() = 0;
    virtual void OpenEvidence(WombatVariable wombatVariable) = 0;
    //virtual void SetupSystemProperties(QString settingsPath, QString configFilePath) = 0;
    //virtual void SetupSystemLog(QString dataPath, QString logFilePath, ProgressWindow* progressWindow, WombatVariable *wombatVariable) = 0;
    virtual QString SetupImageDatabase(QString imgDBPath, QString evidenceFilePath) = 0;
    virtual void OpenImageDatabase(QString imgDBPath, QString evidenceFilePath) = 0;
    virtual void SetupSystemBlackboard() = 0;
    //virtual void SetupSystemSchedulerQueue() = 0;
    //virtual void SetupSystemFileManager() = 0;
    virtual void SetupScheduler() = 0;
    virtual void SetupFileManager() = 0;
    //virtual void OpenEvidence(QString evidencePath, ProgressWindow* progressWindow) = 0;
    virtual void LogEntry(QString logMsg) = 0;
    virtual QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName) = 0;
    virtual QString GetFileContents(int fileID) = 0;
    virtual QString GetFileTxtContents(int fileID) = 0;

};

#define ViewerInterface_iid "wombat.ViewerInterface"
Q_DECLARE_INTERFACE(ViewerInterface, ViewerInterface_iid)

#define PluginInterface_iid "wombat.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

//#define EvidenceInterface_iid "wombat.EvidenceInterface"
//Q_DECLARE_INTERFACE(EvidenceInterface, EvidenceInterface_iid)

//#define BasicToolsInterface_iid "wombat.BasicToolsInterface"
//Q_DECLARE_INTERFACE(BasicToolsInterface, BasicToolsInterface_iid)

#define SleuthKitInterface_iid "wombat.SleuthKitInterface"
Q_DECLARE_INTERFACE(SleuthKitInterface, SleuthKitInterface_iid)

#endif
